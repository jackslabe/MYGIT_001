/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê3ÔÂ17ÈÕ
* Compiler   : gcc
* FILE NAME  : me25pe40_test.c
*
* DESCRIPTION:
*
*/

#include "M25PE40_interface.h"

#define FLASH_WRITE_BUFFER		258


void m25pe40_test(void){

	int32_t ret = 0;
	int32_t i = 0;
	uint32_t addr = 0x0000;
	uint32_t write_buf[FLASH_WRITE_BUFFER] = {0x11111111, 0x22222222, 0x33333333, 0x44444444,\
							 0x55555555, 0x66666666, 0x77777777, 0x88888888};

	uint32_t read_buf[FLASH_WRITE_BUFFER] = {0};


	//spi_bus_init();

	m25pe40_init();


	for(;;) {

		//sys_test();
		ret = m25pe40_write(addr, (uint8_t *)write_buf, sizeof(write_buf));

		ret = m25pe40_read(addr, (uint8_t *)read_buf, sizeof(write_buf));

		addr += sizeof(write_buf);

		for(i = 0; i < FLASH_WRITE_BUFFER; i ++){
			write_buf[i] += 1;
		}

		if(addr > 0x7FFFF){
			for(i = 0; i < M25PE40_SUBSECTOR_NUM*M25PE40_SECTOR_NUM; i ++){

				addr = i * M25PE40_SUBSECTOR_SIZE;
				m25pe40_read(addr, (uint8_t *)read_buf, sizeof(read_buf));
				m25pe40_erase(2, i);
				m25pe40_read(addr, (uint8_t *)read_buf, sizeof(read_buf));
			}

			addr = 0x00;
			write_buf[0] = 0x11111111;
			write_buf[1] = 0x22222222;
			write_buf[2] = 0x33333333;
			write_buf[3] = 0x44444444;
			write_buf[4] = 0x55555555;
			write_buf[5] = 0x66666666;
			write_buf[6] = 0x77777777;
			write_buf[7] = 0x88888888;
			write_buf[257] = 0x88888888;
		}

	}


}
