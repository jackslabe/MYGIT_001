/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ4ÈÕ
* Compiler   : gcc
* FILE NAME  : PIT_drv.c
*
* DESCRIPTION:
*
*/

#include "derivative.h"
#include "PIT_drv.h"

void (*p_timer_irq_handler[4])(void);

void PIT_timer0_irq_handler(void){
	p_timer_irq_handler[0]();
	PIT_ClearFlag(0);
}

void PIT_timer1_irq_handler(void){
	p_timer_irq_handler[1]();
	PIT_ClearFlag(1);
}

void PIT_timer2_irq_handler(void){
	p_timer_irq_handler[2]();
	PIT_ClearFlag(2);
}

void PIT_timer3_irq_handler(void){
	p_timer_irq_handler[3]();
	PIT_ClearFlag(3);
}
/***************************************************************************//*!
*   @brief The function PIT_Init initializes the Periodic Interrupt Timer.
*	@par Include
*					PIT.h
* 	@par Description
*					This function turns on clock for the PIT module.
*	@remarks 	In debug mode is the PIT module frozen by default.
*	@par Code sample
*			PIT_Init();
*			- Command activates clock for the PIT module.
********************************************************************************/
void PIT_Init(void)
{
	PIT_0.MCR.B.MDIS=0; 	// Module clock on
	PIT_0.MCR.B.FRZ=1;  	// Freeze timer in debug
}

/***************************************************************************//*!
*   @brief The function PIT_Setup computes and sets up a period for the
*			specified channel.
*	@par Include
*					PIT.h
* 	@par Description
*					This function computes register settings from clock frequency and expected time base.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@param[in] Clock_Freq
*					System clock frequency.
*	@param[in] ExpectedTimeBase
*					Desired time period for the specified channel.
*	@remarks 	Maximal setable time is given by the maximal value of the 32-bit LDVAL register divided
*				by system the clock frequency ((2^32)/fsys). PIT module should be initialized before
*				(see PIT_Init function).
*	@par Code sample
*			PIT_Setup(0, 16000000, 0.010);
*			- Command sets up channel no. 0 of the timer with expected time
*			base 10 ms assuming clock frequency 16 MHz.
********************************************************************************/
void PIT_Setup(int8_t Channel, uint32_t Clock_Freq, double ExpectedTimeBase)
{
	PIT_0.TIMER[Channel].LDVAL.R=(int32_t)Clock_Freq*ExpectedTimeBase-1;
}

/***************************************************************************//*!
*   @brief The function PIT_EnableInt enables an interrupt for a specified channel.
*	@par Include
*					PIT.h
* 	@par Description
*					This function enables an interrupt for a specified channel
*					(sets TIE bit) and clears
*					interrupt flag (TIF) in the same time.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_EnableInt(1);
*			- Command enables interruption for the first channel of the PIT.
********************************************************************************/
void PIT_EnableInt(int8_t Channel, void (*p_timer_irq_func)(void))
{
	if(Channel < 4){
		p_timer_irq_handler[Channel] = p_timer_irq_func;
		PIT_0.TIMER[Channel].TCTRL.B.TIE = 1;
		PIT_0.TIMER[Channel].TFLG.R = 1;
	}
}

/***************************************************************************//*!
*   @brief The function PIT_EnableChannel enables a channel of the PIT.
*	@par Include
*					PIT.h
* 	@par Description
*					This function enables specified channel by setting TEN bit.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_EnableInt(2);
*			- Command enables second channel of the PIT.
********************************************************************************/
void PIT_EnableChannel(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TEN=1;
}

/***************************************************************************//*!
*   @brief The function PIT_Disable disables a channel of the PIT
*			(including interrupts).
*	@par Include
*					PIT.h
* 	@par Description
*					This function disables the whole  PIT channel. The associated
*					interrupt flag is cleared and the interrupt deisabled in the
*					same time.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_Disable(3);
*			- Command disables channel no. 3 of the PIT.
********************************************************************************/
void PIT_Disable(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TIE=0;
    PIT_0.TIMER[Channel].TCTRL.B.TEN=0;
    PIT_0.TIMER[Channel].TFLG.R=1;
}

/***************************************************************************//*!
*   @brief The function PIT_GetFlag returns an interrupt flag.
*	@par Include
*					PIT.h
* 	@par Description
*					This function returns the interrupt flag (TIF) of the
*					specified channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@return Time Interrupt Flag.
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			while(PIT_GetFlag(3)==0);
*			- Command waits until the PIT channel no. 3 will overflow (Simple
*			waiting without any interruption).
********************************************************************************/
int8_t PIT_GetFlag(int8_t Channel)
{
   return (int8_t)PIT_0.TIMER[Channel].TFLG.B.TIF;
}

/***************************************************************************//*!
*   @brief The function PIT_ClearFlag clears an interrupt flag.
*	@par Include
*					PIT.h
* 	@par Description
*					This function clears Time Interrupt Flag (TIF) for the specified channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_ClearFlag(0);
*			- Command clears TIF flag of the channel no. 0.
********************************************************************************/
void PIT_ClearFlag(int8_t Channel)
{
    PIT_0.TIMER[Channel].TFLG.R=1;
}

/***************************************************************************//*!
*   @brief The function PIT_DisableChannel disables a PIT channel.
*	@par Include
*					PIT.h
* 	@par Description
*					This function clears Timer Enable Bit (TEN) for the specified PIT channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			PIT_DisableChannel(1);
*			- Command disables first PIT channel.
********************************************************************************/
void PIT_DisableChannel(int8_t Channel)
{
    PIT_0.TIMER[Channel].TCTRL.B.TEN=0;
}

/***************************************************************************//*!
*   @brief The function PIT_IsChannelEnabled checks if a specified PIT channel
*			is enabled.
*	@par Include
*					PIT.h
* 	@par Description
*					This function returns content of the Timer Enable Bit (TEN) for the specified PIT channel.
*	@param[in] Channel
*					Number of the PIT's channel (from 0 till 3).
*	@return Number of the PIT's channel (from 0 till 3).
*	@remarks 	PIT module should be initialized before (see PIT_Init function).
*	@par Code sample
*			res = PIT_IsChannelEnabled(0);
*			- Command returns state of PIT channel number 0.
********************************************************************************/
uint32_t PIT_IsChannelEnabled(int8_t Channel)
{
	return PIT_0.TIMER[Channel].TCTRL.B.TEN;			//returns 1 if PIT channel is enabled or 0
}






