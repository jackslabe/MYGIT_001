/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ16ÈÕ
* Compiler   : gcc
* FILE NAME  : STM_drv.h
*
* DESCRIPTION:
*
*/
#ifndef STM_DRV_H_
#define STM_DRV_H_

void system_timer_init(uint8_t divide_value);
void system_timer_start(void);
void system_timer_delay(uint32_t delay_time);
uint32_t system_timer_counter(void);

#endif /* STM_DRV_H_ */
