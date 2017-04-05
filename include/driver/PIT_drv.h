/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ11ÈÕ
* Compiler   : gcc
* FILE NAME  : PIT_drv.h
*
* DESCRIPTION:
*
*/
#ifndef PIT_DRV_H_
#define PIT_DRV_H_
//offset definitions
#define PITCH0_OFF			0x100
#define PITCH1_OFF			0x110
#define PITCH2_OFF			0x120
#define PITCH3_OFF			0x130


void PIT_Init(void);
void PIT_Setup(int8_t Channel, uint32_t Clock_Freq, double ExpectedTimeBase);
void PIT_EnableInt(int8_t Channel, void (*p_timer_irq_handler)(void));
void PIT_EnableChannel(int8_t Channel);
void PIT_Disable(int8_t Channel);
int8_t PIT_GetFlag(int8_t Channel);
void PIT_ClearFlag(int8_t Channel);

void PIT_DisableChannel(int8_t Channel);
uint32_t PIT_IsChannelEnabled(int8_t Channel);


#endif /* PIT_DRV_H_ */
