/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê1ÔÂ4ÈÕ
* Compiler   : gcc
* FILE NAME  : cell_app.c
*
* DESCRIPTION:
*
*/
#include "derivative.h"

struct CellData{
	uint16_t cell_v;		/* voltage, unit: 1mV */
	 int16_t cell_t;		/* temperature, unit: 0.1 degree */
	uint32_t cell_r;		/* resister, unit: 0.1mOhm */
	uint32_t cell_vol;		/* volume/capacity, unit: 1 mAh */	/* 96Ah = 96000mAh */
	uint16_t cell_soc;		/* state of charge, unit: 0.1%  */
	uint16_t cell_soh;		/* state of health */
	uint16_t cell_sop;		/* state of power  */
	uint16_t cell_status;	/* 0: normal, 1: open wire, */
	uint16_t cell_enable;	/* cell enable or disable */
};

#define LTC6804_NUM		4
#define CHANNEL_NUM		12


struct CellData cell_dat[LTC6804_NUM][CHANNEL_NUM];


