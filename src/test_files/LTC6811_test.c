/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê3ÔÂ1ÈÕ
* Compiler   : gcc
* FILE NAME  : LTC6811_test.c
*
* DESCRIPTION:
*
*/
#include "derivative.h" /* include peripheral declarations */
#include "LTC6811_interface.h"

struct LTC6811Reg LTC6811_dev[LTC6811_MAX_CHIP_NUM];

uint16_t cell_v[24];




int32_t get_cell_voltage(void){
	int i = 0, j = 1;

	for(i = 0; i < 12; i += 3){

		cell_v[i] = LTC6811_dev[0].reg[j].dat[0] + LTC6811_dev[0].reg[j].dat[1]*256;
		cell_v[i+1] = LTC6811_dev[0].reg[j].dat[2] + LTC6811_dev[0].reg[j].dat[3]*256;
		cell_v[i+2] = LTC6811_dev[0].reg[j].dat[4] + LTC6811_dev[0].reg[j].dat[5]*256;
		j += 1;
	}

	j = 1;
	for(i = 12; i < 24; i += 3){

		cell_v[i] = LTC6811_dev[1].reg[j].dat[0] + LTC6811_dev[1].reg[j].dat[1]*256;
		cell_v[i+1] = LTC6811_dev[1].reg[j].dat[2] + LTC6811_dev[1].reg[j].dat[3]*256;
		cell_v[i+2] = LTC6811_dev[1].reg[j].dat[4] + LTC6811_dev[1].reg[j].dat[5]*256;
		j += 1;
	}

	return 0;
}


void ltc6811_test(void){

		LT6811_START_ADCV(MD_10 | DCP_OFF | CH_000);

		LTC6811_RDCV(2, LTC6811_dev);

		get_cell_voltage();

		//LTC6811_dev[0].reg[0].dat[0] = (mpc33972_sp_input.sp6 << 7) | REFON_1;
		//LTC6811_dev[1].reg[0].dat[0] = (mpc33972_sp_input.sp7 << 7) | REFON_1;

		//LTC6811_dev[0].reg[0].dat[4] = (uint8_t)discharge_ctrl;
		//LTC6811_dev[0].reg[0].dat[5] = (uint8_t)(discharge_ctrl >>8) | 0xF0;

		//LTC6811_dev[1].reg[0].dat[4] = (uint8_t)discharge_ctrl;
		//LTC6811_dev[1].reg[0].dat[5] = (uint8_t)(discharge_ctrl >>8) | 0xF0;

		LTC6811_SET_GPIO5(2, LTC6811_dev);

}
