/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ4ÈÕ
* Compiler   : gcc
* FILE NAME  : STM_drv.c
*
* DESCRIPTION:
*			System Timer Module (STM)
*/

#include "derivative.h"

#define SYSTEM_TIMER_STOP_IN_DEBUG	1
#define SYSTEM_TIMER_RUN_IN_DEBUG	0

void system_timer_init(uint8_t divide_value){
	volatile struct STM_tag *p_STM = &STM_0;

	p_STM->CR.B.CPS = divide_value;
	p_STM->CR.B.FRZ = SYSTEM_TIMER_RUN_IN_DEBUG;
}

void system_timer_start(void){
	volatile struct STM_tag *p_STM = &STM_0;
	p_STM->CR.B.TEN = 1;
}


void system_timer_set_channel(uint8_t channel, uint32_t compare_value){
	volatile struct STM_tag *p_STM = &STM_0;
	if(channel < 4){
		p_STM->CHANNEL[channel].CCR.B.CEN = 1;
		p_STM->CHANNEL[channel].CMP.R = compare_value;
	}else{
		/* do nothing */
	}
}

void system_timer_delay(uint32_t delay_time){
	uint32_t timer_end = 0;
	volatile struct STM_tag *p_STM = &STM_0;
	timer_end = p_STM->CNT.R + delay_time;

	while(p_STM->CNT.R != timer_end){
		;
	}
}

uint32_t system_timer_counter(void){
	volatile struct STM_tag *p_STM = &STM_0;
	return p_STM->CNT.R;
}

