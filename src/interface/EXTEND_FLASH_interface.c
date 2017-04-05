/*
 * LICENSE:
 * Copyright (c) 2016 CheHeJia technology
 *
 * Composed By: chj1541
 * Dated      : 2017Äê3ÔÂ22ÈÕ
 * Compiler   : gcc
 * FILE NAME  : NV_FLASH_interface.c
 *
 * DESCRIPTION:
 *
 */

#include "M25PE40_interface.h"
#include "CRC32_checksum.h"
#include "EXTEND_FLASH_interface.h"

#define INDEX_TABLE_LEN		32

/* error infomation */
/* too many id that beyong INDEX_TABLE_LEN */
/* data check sum error */
/* flash busy */
/* data area error, need format */
/* flash no enough memory to store data */
/* id and dat_len mismatch */
/* set prepare error */
/* set acive error */
/* communication error */
/* section status error, need init */

struct AddressIndexTable {
	uint32_t addr;
	uint16_t dat_id;
	uint16_t dat_len;
};

struct AddressIndexTable addr_index_table[INDEX_TABLE_LEN];
uint16_t index_num = 0;

struct SectionHead{
	uint32_t prepare_status;
	uint32_t active_status;
	uint32_t swap_flag;
	uint32_t erase_counter;
	uint32_t erase_status;
};


struct DataHead{
	uint16_t dat_len;
	uint16_t dat_id;
};


#define SECTION0_DATA_START_ADDRESS		0x00000000
#define SECTION0_DATA_END_ADDRESS		0x0003FFE0

#define SECTION0_PREPARE_START_ADDRESS	0x0003FFEC
#define SECTION0_ACTIVE_START_ADDRESS	0x0003FFF0
#define SECTION0_SWAP_FLAG_ADDRESS		0x0003FFF4
#define SECTION0_ERASE_COUNTER_ADDRESS	0x0003FFF8
#define SECTION0_ERASE_STATUS_ADDRESS	0x0003FFFC
#define SECTION0_END_ADDRESS			0x0003FFFF
#define SECTION0_HEAD_START_ADDRESS		SECTION0_PREPARE_START_ADDRESS

#define SECTION1_DATA_START_ADDRESS		0x00040000
#define SECTION1_DATA_END_ADDRESS		0x0007FFE0
#define SECTION1_PREPARE_START_ADDRESS	0x0007FFEC
#define SECTION1_ACTIVE_START_ADDRESS	0x0007FFF0
#define SECTION1_SWAP_FLAG_ADDRESS		0x0007FFF4
#define SECTION1_ERASE_COUNTER_ADDRESS	0x0007FFF8
#define SECTION1_ERASE_STATUS_ADDRESS	0x0007FFFC
#define SECTION1_END_ADDRESS			0x0007FFFF
#define SECTION1_HEAD_START_ADDRESS		SECTION1_PREPARE_START_ADDRESS

#define SECTION_NUM		2

#define SECTION_IN_ERASING			0x0000FFFF
#define SECTION_ERASE_COMPLETE		0xFFFFFFFF

#define SECTION_PREPARE_STATUS		0x0F0F0F0F
#define SECTION_ACTIVE_STATUS		0x5A5A5A5A
#define SECTION_INACTIVE_STATUS		0x00000000
#define SECTION_SWAP_ON_GOING		0xFFFF0000
#define SECTION_SWAP_COMPLETE		0x00000000
#define SECTION_NONE_STATUS			0xFFFFFFFF
#define SECTION_ERROR_STATUS		0xEEEEEEEE

#define SECTION_SIZE				SECTION0_DATA_END_ADDRESS

#define FLASH_SECTION0	0
#define FLASH_SECTION1	1

#define NO_SUCH_DATA_ID		0xFFFFFFFF



uint32_t ext_flash_section_status_check(struct SectionHead section_head){
	uint32_t status = 0;


	if((section_head.active_status == SECTION_ACTIVE_STATUS) && (section_head.prepare_status == SECTION_PREPARE_STATUS)){
		if((section_head.swap_flag == SECTION_SWAP_COMPLETE) && (section_head.erase_status == SECTION_ERASE_COMPLETE)){
			status = SECTION_ACTIVE_STATUS;
		}else{
			/* error */
			status = SECTION_ERROR_STATUS;
		}
	}else if((section_head.active_status == SECTION_NONE_STATUS) && (section_head.prepare_status == SECTION_PREPARE_STATUS)){
		if((section_head.swap_flag == SECTION_NONE_STATUS) && (section_head.erase_status == SECTION_ERASE_COMPLETE)){
			status = SECTION_PREPARE_STATUS;
		}else{
			status = SECTION_ERROR_STATUS;
		}
	}else{
		status = SECTION_ERROR_STATUS;
	}


	return status;
}

uint32_t ext_flash_prepare(uint8_t section_num) {
	int32_t ret = 0;
	uint32_t status = 0;
	uint32_t start_address = 0xFFFFFFFF;
	uint32_t prepare_data = SECTION_PREPARE_STATUS;
	uint32_t erase_status_address = 0xFFFFFFFF;
	uint32_t erase_status = SECTION_IN_ERASING;
	uint32_t erase_counter_addr = 0xFFFFFFFF;
	uint32_t erase_counter = 0;
	uint32_t sector_start_num = 0;
	int32_t i = 0;

	if (section_num < SECTION_NUM) {
		if (section_num == 0) {
			start_address = SECTION0_PREPARE_START_ADDRESS;
			erase_status_address = SECTION0_ERASE_STATUS_ADDRESS;
			erase_counter_addr = SECTION0_ERASE_COUNTER_ADDRESS;
			sector_start_num = 0;
		} else if (section_num == 1) {
			start_address = SECTION1_PREPARE_START_ADDRESS;
			erase_status_address = SECTION1_ERASE_STATUS_ADDRESS;
			erase_counter_addr = SECTION1_ERASE_COUNTER_ADDRESS;
			sector_start_num = 4;
		} else {
			/* do nothing */
		}
		/* set active section erase status as 0x0000FFFF */
		ret = m25pe40_write(erase_status_address, (uint8_t *)&erase_counter, sizeof(erase_counter));
		/* fomat section */
		if(ret == 0){
			/* read erase counter */
			ret = m25pe40_read(erase_counter_addr, (uint8_t *)&erase_status, sizeof(erase_status));
			for(i = sector_start_num; i < (sector_start_num + 4); i ++){
				ret = m25pe40_erase(M25PE40_SECTOR_ERASE_CMD, i);
			}
			/* check erase status */
			ret = m25pe40_read(erase_status_address, (uint8_t *)&erase_status, sizeof(erase_status));

			if(ret == 0){
				if(erase_status == SECTION_ERASE_COMPLETE){
					/* write erase counter */
					erase_counter += 1;
					ret = m25pe40_write(erase_counter_addr, (uint8_t *)&erase_counter, sizeof(erase_counter));
					/* set section prepare */
					ret = m25pe40_write(start_address, (uint8_t *)&prepare_data, sizeof(prepare_data));
					/* check prepare status */
					prepare_data = 0;
					ret = m25pe40_read(start_address, (uint8_t *)&prepare_data, sizeof(prepare_data));
					if(prepare_data != SECTION_PREPARE_STATUS){
						/* set prepare status error */
					}
				}else{
					/* erase error */
				}
			}else{
				/* flash busy or spi communication error */
			}
		}else{
			/* flash busy or spi communication error */
		}

	} else {
		/* section_num error */
	}

	return ret;
}

uint32_t ext_flash_active(uint8_t section_num) {
	int32_t ret = 0;
	uint32_t status = 0;
	uint32_t active_address = 0xFFFFFFFF;
	uint32_t active_dat = 0;
	uint32_t swap_flag = 0;
	uint32_t swap_addr = 0xFFFFFFFF;

	if (section_num < SECTION_NUM) {
		if (section_num == 0) {
			active_address = SECTION0_ACTIVE_START_ADDRESS;
			swap_addr = SECTION0_SWAP_FLAG_ADDRESS;
		} else if (section_num == 1) {
			active_address = SECTION1_ACTIVE_START_ADDRESS;
			swap_addr = SECTION0_SWAP_FLAG_ADDRESS;
		} else {
			/* section error  */
		}

		/* set section active */
		active_dat = SECTION_ACTIVE_STATUS;
		ret = m25pe40_write(active_address, (uint8_t *)&active_dat, sizeof(active_dat));
		ret = m25pe40_read(active_address, (uint8_t *)&active_dat, sizeof(active_dat));

		ret = m25pe40_write(swap_addr, (uint8_t *)&swap_flag, sizeof(swap_flag));
		ret = m25pe40_read(swap_addr, (uint8_t *)&swap_flag, sizeof(swap_flag));

		if(active_dat != SECTION_ACTIVE_STATUS){
			/* set section active error */
		}else{
			/* do nothing */
		}
	}

	return ret;
}

uint32_t ext_flash_inactive(uint8_t section_num){
	int32_t ret = 0;
	uint32_t status = 0;
	uint32_t inactive_addr = 0xFFFFFFF;
	uint32_t inactive_dat = 0;

	if (section_num < SECTION_NUM) {
		if (section_num == 0) {
			inactive_addr  = SECTION1_ACTIVE_START_ADDRESS;
		} else if (section_num == 1) {
			inactive_addr  = SECTION0_ACTIVE_START_ADDRESS;
		} else {
			/* do nothing */
		}

		/* set section inactive */
		inactive_dat = 0;
		ret = m25pe40_write(inactive_addr, (uint8_t *)&inactive_dat, sizeof(inactive_dat));
		inactive_dat = 0xFF;
		ret = m25pe40_read(inactive_addr, (uint8_t *)&inactive_dat, sizeof(inactive_dat));
		if(inactive_dat != SECTION_INACTIVE_STATUS){
			/* set section inactive error */
		}

	}else{
		/* section number error */
	}

	return ret;
}

uint32_t ext_flash_copy(uint8_t active_section, uint8_t prepare_section){
	int32_t ret = 0;
	uint32_t status = 0;
	uint8_t page_buffer[256] = {0};
	uint16_t page_num = 0;
	uint16_t last_len = 0;
	uint16_t dat_len = 0;
	uint32_t dat_addr = 0;

	uint32_t prepare_addr = 0xFFFFFFFF;
	uint32_t prepare_dat_addr = 0xFFFFFFFF;
	uint32_t prepase_section_size = 0;
	uint32_t swap_flag = 0xFFFFFFFF;
	uint32_t swap_addr = 0xFFFFFFFF;
	uint32_t dat_crc = 0;
	uint32_t check_crc = 0;

	if((active_section < SECTION_NUM) && (prepare_section < SECTION_NUM)){

		prepare_addr = prepare_section * SECTION1_DATA_START_ADDRESS + SECTION0_PREPARE_START_ADDRESS;
		prepare_dat_addr = prepare_section * SECTION1_DATA_START_ADDRESS + SECTION0_DATA_START_ADDRESS;
		prepase_section_size = prepare_section * SECTION1_DATA_START_ADDRESS + SECTION0_DATA_END_ADDRESS;
		swap_addr = prepare_addr + (SECTION0_SWAP_FLAG_ADDRESS - SECTION0_PREPARE_START_ADDRESS);

		/* set swap_flag */
		swap_flag = SECTION_SWAP_ON_GOING;
		ret = m25pe40_write(swap_addr, (uint8_t *)&swap_flag, sizeof(swap_flag));
		swap_flag = 0xFF;
		ret = m25pe40_read(swap_addr, (uint8_t *)&swap_flag, sizeof(swap_flag));

		if(swap_flag == SECTION_SWAP_ON_GOING){
			/* read data id, length, addr from index_table */
			while(index_num --){
				dat_addr = addr_index_table[index_num].addr;
				dat_len = addr_index_table[index_num].dat_len;
				/* read data flash */
				/* check data length, compute number of pages to read */
				page_num = dat_len / M25PE40_PAGE_SIZE;
				last_len = dat_len % M25PE40_PAGE_SIZE;

				/* check data size */
				if((prepare_dat_addr + dat_len) > prepase_section_size){
					/* out of memory */
					break;
				}
				if(page_num == 0){
					/* dat_len < 256 */
					/* read data from active section */
					ret = m25pe40_read(dat_addr, (uint8_t *)&page_buffer, dat_len);
					/* get data crc */
					dat_crc = *(uint32_t *)&page_buffer[dat_len - 4];
					/* write data to prepare section */
					ret = m25pe40_write(prepare_dat_addr, (uint8_t *)&page_buffer, dat_len);
					/* read data to check crc */
					ret = m25pe40_read(prepare_dat_addr, (uint8_t *)&page_buffer, dat_len);
					/* compute crc */
					check_crc = compute_crc32(page_buffer, dat_len);

					if(check_crc != dat_crc){
						/* data write error */
						break;
					}

					prepare_dat_addr += dat_len;
				}else{
					while(page_num -- > 0){
						/* read data from active section */
						ret = m25pe40_read(dat_addr, (uint8_t *)&page_buffer, sizeof(page_buffer));
						/* write data to prepare section */
						ret = m25pe40_write(prepare_dat_addr, (uint8_t *)&page_buffer, sizeof(page_buffer));
						/* read data from prepare section */
						ret = m25pe40_read(prepare_dat_addr, (uint8_t *)&page_buffer, sizeof(page_buffer));
						/* compute data crc check sum */
						compute_crc32_step(page_buffer, sizeof(page_buffer), CRC_CHECK_CONTINUE);
						/* compute next address */
						prepare_dat_addr += M25PE40_PAGE_SIZE;
						dat_addr += M25PE40_PAGE_SIZE;
					}
					if(last_len != 0){
						/* read data from active section */
						ret = m25pe40_read(dat_addr, (uint8_t *)&page_buffer, last_len);
						/* get data crc */
						ret = m25pe40_read(dat_addr + last_len - 4, (uint8_t *)&dat_crc, sizeof(dat_crc));
						/* write data to prepare section */
						ret = m25pe40_write(prepare_dat_addr, (uint8_t *)&page_buffer, last_len);
						/* read data from prepare section */
						ret = m25pe40_read(prepare_dat_addr, (uint8_t *)&page_buffer, last_len);
						/* compute data crc check sum */
						check_crc = compute_crc32_step(page_buffer, last_len, CRC_CHECK_END);
						/* compare data crc check sum */
						if(check_crc != dat_crc){
							/* data check error */
							break;
						}
						prepare_dat_addr += last_len;
					}
				}/* check page number end */
			}/* while(index --) end */
		}/* check swap flag end */
	}else{
		/* section number error */
	}
	/* */
	return ret;
}

uint32_t ext_flash_section_swap(void) {
	int32_t ret = 0;
	uint32_t status = 0;
	uint32_t section0_status = 0xFF;
	uint32_t section1_status = 0xFF;

	struct SectionHead section0_head = {0,0,0,0};
	struct SectionHead section1_head = {0,0,0,0};

	/* search active section */
	/* read section head status */
	ret = m25pe40_read(SECTION0_HEAD_START_ADDRESS, (uint8_t *)&section0_head, sizeof(section0_head));
	ret = m25pe40_read(SECTION1_HEAD_START_ADDRESS, (uint8_t *)&section1_head, sizeof(section1_head));

	section0_status = ext_flash_section_status_check(section0_head);
	section1_status = ext_flash_section_status_check(section1_head);
	/* check section status */
	if((section0_status == SECTION_PREPARE_STATUS) && (section1_status == SECTION_ACTIVE_STATUS)){
		/* section1 active */
		/* copy active section to prepare section */
		status = ext_flash_copy(FLASH_SECTION1, FLASH_SECTION0);
		if(status == 0){
			/* set section active */
			status = ext_flash_active(FLASH_SECTION0);
			status = ext_flash_inactive(FLASH_SECTION1);
			/* set section prepare */
			if(status == 0){
				status = ext_flash_prepare(FLASH_SECTION1);
			}
		}
	}else if((section0_status == SECTION_ACTIVE_STATUS) && (section1_status == SECTION_PREPARE_STATUS)){
		/* section0 active */
		/* copy active section to prepare section, set prepare as active */
		status = ext_flash_copy(FLASH_SECTION0, FLASH_SECTION1);
		if(status == 0){
			/* set section active */
			status = ext_flash_active(FLASH_SECTION1);
			status = ext_flash_inactive(FLASH_SECTION0);
			/* set section prepare */
			if(status == 0){
				status = ext_flash_prepare(FLASH_SECTION0);
			}
		}
	}else{
		/* section error, need init */

	}

	return ret;
}

uint32_t index_table_search(uint16_t dat_id, uint16_t dat_len){
	uint32_t ret = NO_SUCH_DATA_ID;
	int32_t cnt = index_num;

	while(cnt --){
		if(addr_index_table[cnt].dat_id == dat_id){
			/* find data */
			ret = cnt;
			break;
		}
	}

	return ret;
}

uint32_t index_table_add(uint16_t dat_id, uint16_t dat_len, uint32_t addr) {
	uint32_t ret = 0;

	ret = index_table_search(dat_id, dat_len);

	if(ret == NO_SUCH_DATA_ID){
		/**/
		addr_index_table[index_num].addr = addr;
		addr_index_table[index_num].dat_id = dat_id;
		addr_index_table[index_num].dat_len = dat_len;
		index_num += 1;
	}else{
		addr_index_table[ret].addr = addr;
		addr_index_table[ret].dat_id = dat_id;
		addr_index_table[ret].dat_len = dat_len;
	}

	return ret;
}

uint32_t index_table_max_addr(void){
	uint32_t ret = 0;

	int32_t i = 0;
	uint32_t max_addr = 0;

	for(i = 0; i < index_num; i ++){
		if((addr_index_table[i].addr > max_addr) && (addr_index_table[i].addr != 0xFFFFFFFF)){
			max_addr = addr_index_table[i].addr;
			ret = i;
		}
	}

	return ret;
}


uint32_t index_table_delete(uint16_t dat_id, uint16_t dat_len) {
	uint32_t ret = 0;
	int32_t i = 0;
	ret = index_table_search(dat_id, dat_len);

	if(ret != NO_SUCH_DATA_ID){
		addr_index_table[ret].addr = 0xFFFFFFFF;
		addr_index_table[ret].dat_id = 0xFFFF;
		addr_index_table[ret].dat_len = 0xFFFF;

		for(i = index_num - ret; i < index_num; i ++){
			addr_index_table[ret].addr = addr_index_table[ret + 1].addr;
			addr_index_table[ret].dat_id = addr_index_table[ret + 1].dat_id;
			addr_index_table[ret].dat_len = addr_index_table[ret + 1].dat_len;
			ret ++;
		}
	}else{
		/* no id data */
	}

	return ret;
}


uint32_t index_table_init(void){
	uint32_t ret = 0;
	uint32_t i = 0;

	for(i = 0; i < INDEX_TABLE_LEN; i ++){
		addr_index_table[i].addr = 0xFFFFFFFF;
		addr_index_table[i].dat_id = 0xFFFF;
		addr_index_table[i].dat_len = 0xFFFF;
	}

	return ret;
}

uint32_t get_used_size(void){
	uint32_t ret = 0;
	uint32_t i = 0;

	for(i = 0; i < index_num; i ++){

		ret += addr_index_table[i].dat_len;
	}

	return ret;
}

uint32_t ext_flash_init(void) {
	uint32_t ret = 0;
	uint32_t status = 0;
	uint32_t i = 0;
	uint32_t section0_status = 0xFF;
	uint32_t section1_status = 0xFF;
	uint32_t active_dat_addr = SECTION_NONE_STATUS;
	uint32_t active_end_addr = SECTION_NONE_STATUS;

	struct DataHead dat_head = {0xFFFF, 0xFFFF};

	struct SectionHead section0_head = {0,0,0,0};
	struct SectionHead section1_head = {0,0,0,0};

	index_table_init();
	init_table();
	/* search active section */
	/* read section head status */
	ret = m25pe40_read(SECTION0_HEAD_START_ADDRESS, (uint8_t *)&section0_head, sizeof(section0_head));
	ret = m25pe40_read(SECTION1_HEAD_START_ADDRESS, (uint8_t *)&section1_head, sizeof(section1_head));

	section0_status = ext_flash_section_status_check(section0_head);
	section1_status = ext_flash_section_status_check(section1_head);

	/* check section status */
	if((section0_status == SECTION_PREPARE_STATUS) && (section1_status == SECTION_ACTIVE_STATUS)){
		/* section1 active */
		active_dat_addr = SECTION1_DATA_START_ADDRESS;
		active_end_addr = SECTION1_DATA_END_ADDRESS;

	}else if((section0_status == SECTION_ACTIVE_STATUS) && (section1_status == SECTION_PREPARE_STATUS)){
		/* section0 active */
		active_dat_addr = SECTION0_DATA_START_ADDRESS;
		active_end_addr = SECTION0_DATA_END_ADDRESS;
	}else{
		/* section error, need format */
		/* read erase counter */
		ext_flash_prepare(FLASH_SECTION0);
		ext_flash_prepare(FLASH_SECTION1);

		ext_flash_active(FLASH_SECTION0);
		addr_index_table[0].addr = SECTION0_DATA_START_ADDRESS;
		addr_index_table[0].dat_id = 0;
		addr_index_table[0].dat_len = 0;
		index_num = 0;
		dat_head.dat_id = 0;
		dat_head.dat_len = 0;
		ret = m25pe40_write(SECTION0_DATA_START_ADDRESS, (uint8_t *)&dat_head, sizeof(dat_head));
		ret = m25pe40_write(SECTION0_DATA_START_ADDRESS + sizeof(dat_head), (uint8_t *)&dat_head, sizeof(dat_head));
	}

	/* check section status */
	if(active_dat_addr != SECTION_NONE_STATUS){
		while(active_dat_addr < active_end_addr){
			/* read active section, and update addr_index_table */
			ret = m25pe40_read(active_dat_addr, (uint8_t *)&dat_head, sizeof(dat_head));
			/* check data id and length */
			if((dat_head.dat_id != 0xFFFF) && (dat_head.dat_len != 0xFFFF)){
				/* add to index_tabel */
				index_table_add(dat_head.dat_id, dat_head.dat_len, active_dat_addr);
				active_dat_addr += dat_head.dat_len;
				active_dat_addr += 8;		/* dat_head(4) + crc check sum(4) */
			}else{
				/* no data */
				break;
			}
		};
	}else{
		/* init error */
		/* read erase counter */
		//for(i = 0; i < 8; i ++){
		//	ret = m25pe40_erase(M25PE40_SECTOR_ERASE_CMD, i);
		//}
	}

	return ret;
}

uint32_t ext_flash_write(uint16_t id, uint8_t *p_dat, uint16_t dat_len) {
	uint32_t ret = 0;
	uint32_t status = 0;
	uint32_t start_addr = 0;
	uint32_t page_num = 0;
	uint32_t last_num = 0;
	uint32_t dat_crc = 0;
	uint32_t check_crc = 0;
	uint32_t temp = 0;
	uint32_t used_size = 0;
	struct DataHead dat_head;
	struct AddressIndexTable index_dat;
	uint8_t page_buffer[M25PE40_PAGE_SIZE] = {0x00};

	/* search max address in index table */
	ret = index_table_max_addr();
	/* compute start address */
	start_addr = addr_index_table[ret].addr + addr_index_table[ret].dat_len + sizeof(dat_crc) + sizeof(dat_head);
	/* check section boundary */
	used_size = get_used_size();

	if((dat_len + used_size) < SECTION_SIZE){

		temp = start_addr + dat_len;

		if(start_addr > SECTION0_END_ADDRESS){
			/* section1 is active */
			temp -= SECTION1_DATA_START_ADDRESS;
		}else{
			/* section0 is active */
		}

		if(temp > SECTION_SIZE){
			/* write byte length beyongd section size */
			status = ext_flash_section_swap();
		}else{
			/* do nothing */
		}

		if(status == 0){
			/* search max address in index table */
			ret = index_table_max_addr();
			/* compute start address */
			start_addr = addr_index_table[ret].addr + addr_index_table[ret].dat_len + sizeof(dat_crc) + sizeof(dat_head);
			/* compute page number and last write byte number */
			page_num = dat_len / M25PE40_PAGE_SIZE;
			last_num = dat_len % M25PE40_PAGE_SIZE;
			/* store data to index table */
			index_dat.addr = start_addr;
			index_dat.dat_id = id;
			index_dat.dat_len = dat_len;
			/* compute crc */
			dat_crc = compute_crc32(p_dat, dat_len);
			/* 1. write id and length */
			dat_head.dat_id = id;
			dat_head.dat_len = dat_len;
			ret = m25pe40_write(start_addr, (uint8_t *)&dat_head, sizeof(dat_head));
			/* compute data start address */
			start_addr += sizeof(dat_head);

			if(page_num == 0){
				/* dat_len < 256 */
				/* write data to section */
				ret = m25pe40_write(start_addr, p_dat, dat_len);
				/* write crc data */
				ret = m25pe40_write(start_addr + dat_len, (uint8_t *)&dat_crc, sizeof(dat_crc));
				/* read data to check crc */
				ret = m25pe40_read(start_addr, (uint8_t *)&page_buffer, dat_len);
				/* compute crc */
				check_crc = compute_crc32(page_buffer, dat_len);
				if(check_crc != dat_crc){
					/* data write error */
				}else{
					/* do nothing */
				}
			}else{
				while(page_num -- > 0){
					/* write data to prepare section */
					ret = m25pe40_write(start_addr, p_dat, M25PE40_PAGE_SIZE);
					/* read data from prepare section */
					ret = m25pe40_read(start_addr, (uint8_t *)&page_buffer, M25PE40_PAGE_SIZE);
					/* compute data crc check sum */
					compute_crc32_step(page_buffer, sizeof(page_buffer), CRC_CHECK_CONTINUE);
					/* compute next address */
					start_addr += M25PE40_PAGE_SIZE;
					p_dat += M25PE40_PAGE_SIZE;
				}
				if(last_num != 0){
					/* write data to prepare section */
					ret = m25pe40_write(start_addr, p_dat, last_num);
					/* write crc data */
					ret = m25pe40_write(start_addr + last_num, (uint8_t *)&dat_crc, sizeof(dat_crc));
					/* read data from prepare section */
					ret = m25pe40_read(start_addr, (uint8_t *)&page_buffer, last_num);
					/* compute data crc check sum */
					check_crc = compute_crc32_step(page_buffer, last_num, CRC_CHECK_END);
					/* compare data crc check sum */
					if(check_crc != dat_crc){
						/* data check error */
					}else{
						/* do nothing */
					}
				}
			}/* check page number end */

			/* add id in index table */
			if(status == 0){
				index_table_add(index_dat.dat_id, index_dat.dat_len, index_dat.addr);
			}
		}/* check status end */
	}else{
		/* data size beyond memory size */
	}

	return status;
}

uint32_t ext_flash_read(uint16_t id, uint8_t *p_dat, uint16_t dat_len) {
	uint32_t ret = 0;
	struct AddressIndexTable index_dat;

	ret = index_table_search(id, dat_len);

	if(ret != NO_SUCH_DATA_ID){
		index_dat = addr_index_table[ret];
		ret = m25pe40_read(index_dat.addr + 4, p_dat, dat_len);
	}

	return ret;
}

uint32_t ext_flash_del_record(uint16_t id) {
	uint32_t ret = 0;

	return ret;
}

uint32_t ext_flash_format(void) {
	uint32_t ret = 0;

	return ret;
}

uint32_t ext_flash_dat_recovery(uint16_t id){
	uint32_t ret = 0;

	return ret;
}

