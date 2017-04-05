/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê2ÔÂ8ÈÕ
* Compiler   : gcc
* FILE NAME  : ETimer_drv.h
*
* DESCRIPTION:
*
*/
#ifndef ETIMER_DRV_H_
#define ETIMER_DRV_H_

#define ETIMER_NUM		3
#define ETIMER_CH_NUM	6
#define ETIMER_INT_PRIORITY		7
#define ETIMER_CLOCK_FREQ	10000000		/* 10Mhz */


enum COUNT_SOURCE{
	CNT_0_INPUT,
	CNT_1_INPUT,
	CNT_2_INPUT,
	CNT_3_INPUT,
	CNT_4_INPUT,
	CNT_5_INPUT,
	RESERVED1,
	RESERVED2,
	AUX_0_INPUT,
	AUX_1_INPUT,
	AUX_2_INPUT,
	AUX_3_INPUT,
	AUX_4_INPUT,
	RESERVED3,
	RESERVED4,
	RESERVED5,
	CNT_0_OUTPUT,
	CNT_1_OUTPUT,
	CNT_2_OUTPUT,
	CNT_3_OUTPUT,
	CNT_4_OUTPUT,
	CNT_5_OUTPUT,
	RESERVED6,
	RESERVED7,
	CLK_DIV_1,
	CLK_DIV_2,
	CLK_DIV_4,
	CLK_DIV_8,
	CLK_DIV_16,
	CLK_DIV_32,
	CLK_DIV_64,
	CLK_DIV_128
};

enum COUNT_MODE{
	NO_OPT,
	CNT1_RISING_EDGE,
	CNT1_BOTH_EDGES,
	CNT1_RISING_EDGE_WITH_CNT2_HING_INPUT,
	CNT_QUADRATURE,
	CNT1_RISING_EDGE_CNT2_FREE_EDGE,
	CNT2_DEGE_TRIG_CNT1,
	CNT_CASCADED
};

enum RELOAD_MODE{
	NOT_RELOAD,
	RELOAD_ON_CAPT1_EVENT,
	RELOAD_ON_CAPT2_EVENT,
	RELAOD_ON_BOTH_EVENT
};

enum COMPARE_MODE{
	COMP1_COMP2_CNT_UP,
	COMP1_CNT_DOWN_COMP2_CNT_UP,
	COMP1_CNT_UP_COMP2_CNT_DOWN,
	COMP1_CNT_DOWN_COMP2_CNT_DOWN
};

enum CAPT_MODE{
	DISABLE,
	CAPT_FALLING_EDGE,
	CAPT_RISING_EDGE,
	CAPT_BOTH_EDGE
};


void eTimer_init_all(void);
void eTimer_start_all(void);

void eTimer_init(uint8_t timer, uint8_t channel);
void eTimer_start(uint8_t timer, uint8_t channel);
void eTimer_stop(uint8_t timer, uint8_t channel);

uint32_t eTimer_get_freq(uint8_t timer, uint8_t channel);
uint32_t eTimer_get_period(uint8_t timer, uint8_t channel);
uint32_t eTimer_get_duty(uint8_t timer, uint8_t channel);


#endif /* ETIMER_DRV_H_ */
