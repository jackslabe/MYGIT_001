/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ25ÈÕ
* Compiler   : gcc
* FILE NAME  : M25PE40_interface.c
*
* DESCRIPTION:
*
*/

#include "SPI_bus_drv.h"
#include "GPIO_interface.h"

#include "M25PE40_interface.h"

extern struct spi_master bus2;

struct spi_device m25pe40_spi;
struct spi_transfer m25pe40_txopt;

uint8_t m25pe40_txbuf[10];
uint8_t m25pe40_rxbuf[10];

void m25pe40_complete(void *context){

}

void m25pe40_init(void){
	char  __attribute__((unused))dev_name[DEV_NAME_LEN] = "M25PE40";

	/* configure mpc33908 device */
	m25pe40_spi.max_speed_hz = 75000000;		/* Max SPI operation frequency 75Mhz*/
	m25pe40_spi.cfg_select = 1;				/* Select Clock and Transfer Attributes Register*/
	m25pe40_spi.chip_select = 1;				/* chip select 0 */
	m25pe40_spi.bits_per_word = 8;			/* 16 bit perword */
	m25pe40_spi.dev_CPHA = 0;					/* data captured on the following edge */
	m25pe40_spi.dev_CPOL = 0;					/* inactive state value of SCK is low */
	m25pe40_spi.dev_single = 0;				/* multi device on bus2 */
	m25pe40_spi.dev_LSB_first = 0;				/* msb first */
	m25pe40_spi.actual_speed_hz = 10000000;		/* 10Mhz */
	m25pe40_spi.master = &bus2;
	m25pe40_spi.start_delay_nsecs = 50;			/* delay 50 ns */
	m25pe40_spi.end_delay_nsecs = 50;
	m25pe40_spi.next_frame_delay_nsecs = 50;
	/* SPI bus setup */
	m25pe40_spi.master->setup(&m25pe40_spi);

	/* transfer option configure */
	m25pe40_txopt.spi_bus_num = 2;
	m25pe40_txopt.bits_per_word = 8;
	m25pe40_txopt.cfg_select = 1;
	m25pe40_txopt.cs_num = 1;
	m25pe40_txopt.rx_buf = (void *)m25pe40_rxbuf;
	m25pe40_txopt.tx_buf = (void *)m25pe40_txbuf;
	m25pe40_txopt.rx_len = 3;
	m25pe40_txopt.tx_len = 3;
	m25pe40_txopt.complete = m25pe40_complete;
	m25pe40_txopt.context = (void *)&m25pe40_spi;
	m25pe40_txopt.transfer_mode = TRANSFER_WRITE_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
	m25pe40_txopt.cs_mode = CS_CONT_MODE;	/* CS_SINGLE_MODE CS_CONT_MODE*/

	m25pe40_wp_ctrl(1);
}


#define WRITE_ENABLE_CMD			0x06
#define WRITE_DISABLE_CMD			0x04
#define READ_IDENTIFICATION_CMD		0x9F
#define READ_STATUS_REG_CMD			0x05
#define WRITE_STATUS_REG_CMD		0x01
#define WRITE_LOCK_REG_CMD			0xE5
#define READ_LOCK_REG_CMD			0xE8
#define READ_DATA_BYTES_CMD			0x03
#define READ_DATA_BYTES_H_CMD		0x0B
#define WRITE_PAGE_CMD				0x0A
#define PROGRAM_PAGE_CMD			0x02
#define ERASE_PAGE_CMD				0xDB
#define ERASE_SUBSECTOR_CMD			0x20
#define ERASE_SECTOR_CMD			0xD8
#define ERASE_BULK_CMD				0xC7
#define DEEP_POWER_DOWN_CMD			0xB9
#define RELEASE_FORM_POWER_DOWN		0xAB

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		addr		:
**		dat_buff	:
**		tx_len		:
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
/* return value: SRWD 0 0 BP2 BP1 BP0 WEL WIP */
int32_t m25pe40_read_status_reg(void){
	int32_t ret = 0;

	m25pe40_txbuf[0] = READ_STATUS_REG_CMD;
	m25pe40_txbuf[1] = 0xFF;

	m25pe40_txopt.rx_len = 1;
	m25pe40_txopt.tx_len = 2;
	m25pe40_txopt.transfer_mode = TRANSFER_WRITE_READ_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
	m25pe40_txopt.cs_mode = CS_CONT_MODE;				/* CS_SINGLE_MODE CS_CONT_MODE*/

	m25pe40_txopt.rx_buf = (void *)m25pe40_rxbuf;
	m25pe40_txopt.tx_buf = (void *)m25pe40_txbuf;

	ret = spi_bus_write(&m25pe40_spi, &m25pe40_txopt);

	if(ret == 0){
		while(m25pe40_txopt.rx_new_dat_flag != 1){
			;
		}
		m25pe40_txopt.rx_new_dat_flag = 0;

		ret = m25pe40_rxbuf[0] & 0x01;
	}

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		addr		:
**		dat_buff	:
**		tx_len		:
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/

int32_t m25pe40_write_enable(void){
	int32_t ret = 0;
	m25pe40_txbuf[0] = WRITE_ENABLE_CMD;

	m25pe40_txopt.tx_len = 1;
	m25pe40_txopt.transfer_mode = TRANSFER_WRITE_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
	m25pe40_txopt.cs_mode = CS_SINGLE_MODE;				/* CS_SINGLE_MODE CS_CONT_MODE*/

	m25pe40_txopt.rx_buf = (void *)m25pe40_rxbuf;
	m25pe40_txopt.tx_buf = (void *)m25pe40_txbuf;

	ret = spi_bus_write(&m25pe40_spi, &m25pe40_txopt);

	if(ret == 0){
		while(m25pe40_txopt.tx_end_flag != 1){
			;
		}
		m25pe40_txopt.tx_end_flag = 0;
	}
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		addr		:
**		dat_buff	:
**		tx_len		:
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t m25pe40_read(uint32_t addr, uint8_t *dat_buff, uint32_t rx_len){
	int32_t ret = 0;

	/* read status, check busy status */
	ret = m25pe40_read_status_reg();

	if( ret == 0){
		m25pe40_txbuf[0] = READ_DATA_BYTES_CMD;
		m25pe40_txbuf[1] = (uint8_t)(addr >> 16);
		m25pe40_txbuf[2] = (uint8_t)(addr >> 8);
		m25pe40_txbuf[3] = (uint8_t)(addr);
		m25pe40_txbuf[4] = 0xFF;	/* dummy data */

		m25pe40_txopt.rx_len = rx_len;
		m25pe40_txopt.tx_len = 5;
		m25pe40_txopt.transfer_mode = TRANSFER_WRITE_READ_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
		m25pe40_txopt.cs_mode = CS_CONT_MODE;	/* CS_SINGLE_MODE CS_CONT_MODE*/

		m25pe40_txopt.rx_buf = (void *)dat_buff;

		ret = spi_bus_write(&m25pe40_spi, &m25pe40_txopt);

		if(ret == 0){
			while(m25pe40_txopt.rx_new_dat_flag != 1){
				;
			}
			m25pe40_txopt.rx_new_dat_flag = 0;
		}
	}else{
		/* busy */
	}


	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		addr		:
**		dat_buff	:
**		tx_len		:
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
/* dat_len <= 256 */

int32_t m25pe40_page_write(uint32_t addr, uint8_t * dat_buff, uint16_t dat_len){
	int32_t ret = 0;
	uint32_t page_offset = 0;
	uint32_t tx_len = 0;
	/* page program, first do page aligned */
	/* compute page base addr and start addr in page */
	page_offset = addr & 0xFF;
	tx_len = M25PE40_PAGE_SIZE - page_offset;

	if(tx_len > dat_len){
		tx_len = dat_len;
	}

	ret = m25pe40_write_enable();

	ret = m25pe40_read_status_reg();

	if(ret == 0){/* Flash free for write */
		/* send page programme address */
		m25pe40_txbuf[0] = PROGRAM_PAGE_CMD;
		m25pe40_txbuf[1] = (uint8_t)(addr >> 16);
		m25pe40_txbuf[2] = (uint8_t)(addr >> 8);
		m25pe40_txbuf[3] = (uint8_t)(addr);

		m25pe40_txopt.tx_len = 4;
		m25pe40_txopt.rx_len = 0;
		m25pe40_txopt.transfer_mode = TRANSFER_WRITE_CONTINUOUS;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
		m25pe40_txopt.cs_mode = CS_CONT_MODE;	/* CS_SINGLE_MODE CS_CONT_MODE*/

		m25pe40_txopt.rx_buf = (void *)m25pe40_rxbuf;
		m25pe40_txopt.tx_buf = (void *)m25pe40_txbuf;

		ret = spi_bus_write(&m25pe40_spi, &m25pe40_txopt);

		if(ret == 0){
			while(m25pe40_txopt.tx_end_flag != 1){
				;
			}
			m25pe40_txopt.tx_end_flag = 0;

			/* page write */
			m25pe40_txopt.tx_len = tx_len;
			m25pe40_txopt.tx_buf = (void *)dat_buff;
			m25pe40_txopt.transfer_mode = TRANSFER_WRITE_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/

			ret = spi_bus_write(&m25pe40_spi, &m25pe40_txopt);
			if(ret == 0){
				while(m25pe40_txopt.tx_end_flag != 1){
					;
				}
				m25pe40_txopt.tx_end_flag = 0;

				/* check status , flash is busy */
				do{
					ret = m25pe40_read_status_reg();
				}while((ret >= 0) && ((ret & 0x01)== 1));
			}
		}/* end ret check */
	}/* check Flash status free */

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		addr		:
**		dat_buff	:
**		tx_len		:
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/

int32_t m25pe40_write(uint32_t addr, uint8_t * dat_buff, uint32_t tx_len){

	int32_t ret = 0;
	uint32_t page_base_addr = 0;
	uint32_t page_offset_addr = 0;
	uint32_t first_len = 0;
	uint32_t last_len = 0;
	uint32_t write_page_num = 0;
	/* read status, check busy status */
	ret = m25pe40_read_status_reg();
	if(ret == 0){
		/* page program, first do page aligned */
		/* compute page base addr and start addr in page */
		page_base_addr = addr & 0xFFFFFF00;
		page_offset_addr = addr & 0xFF;
		first_len = M25PE40_PAGE_SIZE - page_offset_addr;
		/* compute tx_len how many pages can be divided */
		if(first_len < tx_len){
			/* send first page */
			ret = m25pe40_page_write(addr, dat_buff, first_len);
			last_len = (tx_len - first_len)% M25PE40_PAGE_SIZE;
			if(last_len == 0){
				last_len = M25PE40_PAGE_SIZE;
			}
			write_page_num = (tx_len - first_len)/M25PE40_PAGE_SIZE;

			page_base_addr = addr + first_len;
			dat_buff += first_len;
			/* send aligned page */
			while(write_page_num --){
				ret = m25pe40_page_write(page_base_addr, dat_buff, M25PE40_PAGE_SIZE);

				page_base_addr += M25PE40_PAGE_SIZE;
				dat_buff += M25PE40_PAGE_SIZE;
			};

			/* send last page */
			ret = m25pe40_page_write(page_base_addr, dat_buff, last_len);

		}else{
			/* tx_len can write in first page */
			ret = m25pe40_page_write(addr, dat_buff, tx_len);
		}

	}/* end status check */

	return ret;
}



/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		erase_cmd, 	1: page erase command
**				   	2: subsector erase command
**				   	3: sector erase command
**		num,		0 ~ 256
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/

int32_t m25pe40_erase(uint8_t erase_cmd, uint16_t num){
	int32_t ret = 0;
	uint32_t addr = 0;
	uint8_t cmd = 0;

	switch(erase_cmd){
	case M25PE40_PAGE_ERASE_CMD:
		addr = num * M25PE40_PAGE_SIZE;
		cmd = ERASE_PAGE_CMD;
		break;
	case M25PE40_SUBSECTOR_ERASE_CMD:
		addr = num * M25PE40_SUBSECTOR_SIZE;
		cmd = ERASE_SUBSECTOR_CMD;
		break;
	case M25PE40_SECTOR_ERASE_CMD:
		addr = num * M25PE40_SECTOR_SIZE;
		cmd = ERASE_SECTOR_CMD;
		break;
	default:
		break;
	}

	/* read status, check busy status */
	ret = m25pe40_read_status_reg();
	if(ret == 0){
		/* set write enable */
		ret = m25pe40_write_enable();
		if(ret == 0){
			m25pe40_txbuf[0] = cmd;
			m25pe40_txbuf[1] = (uint8_t)(addr >> 16);
			m25pe40_txbuf[2] = (uint8_t)(addr >> 8);
			m25pe40_txbuf[3] = (uint8_t)(addr);

			m25pe40_txopt.tx_len = 4;
			m25pe40_txopt.transfer_mode = TRANSFER_WRITE_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
			m25pe40_txopt.cs_mode = CS_CONT_MODE;	/* CS_SINGLE_MODE CS_CONT_MODE*/

			m25pe40_txopt.rx_buf = (void *)m25pe40_rxbuf;
			m25pe40_txopt.tx_buf = (void *)m25pe40_txbuf;

			ret = spi_bus_write(&m25pe40_spi, &m25pe40_txopt);
			if(ret == 0){
				while(m25pe40_txopt.tx_end_flag != 1){
					;
				}
				m25pe40_txopt.tx_end_flag = 0;
			}
		}
	}

	while(1){
		ret = m25pe40_read_status_reg();
		if((ret == 0) || (ret < 0)){
			break;
		}
	}


	return ret;
}



