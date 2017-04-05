/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê1ÔÂ4ÈÕ
* Compiler   : gcc
* FILE NAME  : pack_app.c
*
* DESCRIPTION:
*
*/
#include "derivative.h"

struct PackData{
	uint16_t pack_v;
	 int16_t pack_i;
	uint32_t pack_capacity;
	uint32_t pack_soc;
	uint32_t pack_soh;
	uint32_t pack_sop;
	uint32_t pack_insulation_p;
	uint32_t pack_insulation_n;
	uint16_t pack_max_cell_v;
	uint16_t pack_min_cell_v;
	 int16_t pack_max_cell_t;
	 int16_t pack_min_cell_t;
	 uint8_t pack_relay_p;			/* positive relay, 0x01: open         */
                                	/* 	 	 	 	   0x02: close        */
                                	/* 	 	 	 	   0x11: can't close  */
                                	/* 	 	 	 	   0x22: can't open, maybe adhesion */
	 uint8_t pack_relay_n;			/* same as pack_relay_p */
	 uint8_t pack_relay_precharge;	/* same as pack_relay_p */
	 uint8_t pack_relay_rev;	    /* reserve */
	 uint8_t pack_internal_lock;	/* */
};

