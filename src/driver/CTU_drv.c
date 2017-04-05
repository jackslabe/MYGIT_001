/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê9ÔÂ20ÈÕ
* Compiler   : gcc
* FILE NAME  : CTU_drv.c
*
* DESCRIPTION:
*
*/

#include "derivative.h"

/*******************************************************************************
Function Name : CTU_Init
Engineer      : b21190
Date          : Apr-13-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : Initialize CTU_0
Issues        : NONE
*******************************************************************************/
void CTU_Init(void)
{
   //CTU setting
    CTU_0.TGSISR.B.I13_RE= 0x1;		// Triggered by the rising edge of the eTimer_0 TO2
    CTU_0.CR.B.TGSISR_RE= 0x1;		// Reload the TGSISR double buffered register

    CTU_0.TGSCR.B.ET_TM	= 0x1;		// Enable Toggle Mode for external Trigger
    CTU_0.TGSCR.B.PRES	= 0x3;		// Prescaler selection bits for TGS and SU. This prescaler is used by the TGS counter
    CTU_0.TGSCR.B.TGS_M	= 0x0;		// Trigger Generator Subunit Mode = Triggered Mode
    //CTU_0.TGSCR.B.TGS_M	= 0x1;		// Trigger Generator Subunit Mode = Sequential Mode

    // Trigger compare values
    CTU_0.TCR[0].R = 0xF000;		// Trigger 0 Compare Value
    CTU_0.TGSCCR.R = 0x7FF0;		// When a match occurs, the TGS counter is stopped
    CTU_0.TGSCRR.R = 0x0;		// TGS Counter Reload Value

    // Enable triggers from CTU TGS to other modules
    CTU_0.THCR1.B.T0_ADCE = 0x1;	// Trigger 0 ADC command output Enable
    CTU_0.THCR1.B.T0_ETE = 0x0;		// Enable external trigger
    CTU_0.THCR1.B.T0_E = 0x1;		// Trigger 0 Enable

    // Configure Commands List Control Register 1
    CTU_0.CLCR1.B.T0_INDEX = 0;		// Commands List Control Register 1. Valid values range from 0 through 23

    //Configure Commands List Registers
    CTU_0.CLR[0].A.B.CH = 0;		// ADC_0 channel 0
    CTU_0.CLR[1].A.B.CH = 1;		// ADC_0 channel 1
    CTU_0.CLR[2].A.B.CH = 2;		// ADC_0 channel 2
    CTU_0.CLR[3].A.B.CH = 3;		// ADC_0 channel 3
    CTU_0.CLR[4].A.B.CH = 4;		// ADC_0 channel 4 - This command is not executed as LC=1
    CTU_0.CLR[4].A.B.LC = 1;		// Stops the execution of command list at command 4 (excluding this command)

    // Update changes
    CTU_0.CR.B.GRE= 0x1;		// Reload double buffered registers

}//CTU_Init

/*******************************************************************************
Function Name : eTimer_Init
Engineer      : b21190
Date          : Mar-19-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : eTimer will trigger CTU MSR(master reset signal)
Issues        : NONE
*******************************************************************************/
void eTimer_Init2(void)
{
    //Initialize eTimer0_ch0 to generate pwm output signal and CTU MSR signal
    ETIMER_0.CH[2].CNTR.R = 0x0000;		// Initial value
    ETIMER_0.CH[2].CTRL1.B.CNTMODE = 0x1;	// Counter mode = count rising edges

    ETIMER_0.CH[2].CTRL2.B.OEN = 0x1;		// Output enable
    ETIMER_0.CH[2].CTRL1.B.LENGTH = 0x0;	// Continue counting to roll over
    ETIMER_0.CH[2].LOAD.R = 0x1;		// Reload value used to initialize the counter
    ETIMER_0.CH[2].COMP1.R = 0x500;		// Compare value for counter
    ETIMER_0.CH[2].COMP2.R = 0x1000;		// Compare value for counter
    ETIMER_0.CH[2].CTRL2.B.OUTMODE = 0x3;	// Toggle OFLAG output on successful compare (COMP1 or COMP2)

    ETIMER_0.CH[2].CMPLD1.R = 0x500;
    ETIMER_0.CH[2].CMPLD2.R = 0x1000;
    ETIMER_0.CH[2].CCCTRL.R = 0x8800;

    //Enable interrupt on compare 1 match
    ETIMER_0.CH[2].INTDMA.B.TCF1IE = 0x1;	// Timer compare 1 flag interrupt enable
    ETIMER_0.CH[2].CTRL1.B.PRISRC = 0x18;       // Primary count source = IBBus clock devide by prescaler 1
}//eTimer_Init
