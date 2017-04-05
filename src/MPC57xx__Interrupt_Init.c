/**************************************************************************/
/* FILE NAME:                              COPYRIGHT (c) Freescale 2016   */
/* MPC57xx__Interrupt_Init.c                      All Rights Reserved     */
/*                                                                        */
/* DESCRIPTION:                                                           */
/* This is the interrupt controller configuration file.                   */
/*=========================================================================*/
/*                                                                        */
/*========================================================================*/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE               */
/* ---   -----------    ----------    ---------------------               */
/* 0.1    R. Moran       10/12/2008    Initial Version                    */
/* 1.1    K. Zemskov     24/02/2016    GHS support                        */
/**************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "compiler_api.h"

#define GetCoreID() ((uint16_t) MFSPR(286))

#if defined(MPC574xP) || defined(MPC574xR) || defined(MPC5777M)
#define INTC             INTC_0
#endif

/* Hardware vector mode bit */
#define INTC_BCR_HVEN(coreId)   ( (uint32_t) (0x1UL << ((coreId) << 3UL)) )  /* coreId - hw coreId (value that is read from PIR) */

/*========================================================================*/
/*                      GLOBAL VARIABLES                                  */
/*========================================================================*/
extern const uint32_t IntcIsrVectorTable[];

/*========================================================================*/
/*                      EXTERNAL PROTOTYPES                               */
/*========================================================================*/
extern void VTABLE (void);

/*========================================================================*/
/*                          FUNCTIONS                                     */
/*========================================================================*/
/**************************************************************************/
/* FUNCTION     : SetIVPR                                                 */
/* PURPOSE      : Initialise Core IVPR                                    */
/**************************************************************************/
void SetIVPR (register unsigned int x)
{
    MTSPR(63, x);
}


/**************************************************************************/
/* FUNCTION     : InitINTC                                                */
/* PURPOSE      : This function intializes the INTC for software vector   */
/*                mode.                                                   */
/**************************************************************************/
void InitINTC(void) {   

	uint32_t __attribute__((unused)) temp;


    /* Choose software (HVEN=0) or hardware (HVEN=1) vector mode */
    INTC.BCR.R   = 0;     /* Software vector mode   */


    temp = INTC.IACKR0.R;
    /* Set INTC ISR vector table base addr. */
    INTC.IACKR0.R = (uint32_t) &IntcIsrVectorTable[0];

    INTC_0.PSR[226].B.PRIN = 1;		/* PIT timer0 */
    INTC_0.PSR[227].B.PRIN = 1;		/* PIT timer1 */
    INTC_0.PSR[228].B.PRIN = 1;		/* PIT timer2 */
    INTC_0.PSR[229].B.PRIN = 1;		/* PIT timer3 */

    INTC_0.PSR[496].B.PRIN = 2;		/* Vector # 496 ADC_EOC ADC_0 */
    INTC_0.PSR[500].B.PRIN = 2;		/* Vector # 500 ADC_EOC ADC_1 */

}


/**************************************************************************/
/* FUNCTION     : enableIrq                                               */
/* PURPOSE      : This function sets INTC's current priority to 0.        */
/*                External interrupts to the core are enabled.            */
/**************************************************************************/
void enableIrq(void) {


    /* Ensure INTC's current priority is 0 */
    INTC.CPR0.R = 0U;
    /* Enable external interrupts */
    PPCASM (" wrteei 1 ");
}


/**************************************************************************/
/* FUNCTION     : xcptn_xmpl                                              */
/* PURPOSE      : This function sets up the necessary functions to raise  */
/*                and handle a Interrupt in software vector mode          */
/**************************************************************************/
void xcptn_xmpl(void) {
    
    /* Initialise Core IVPR */
    SetIVPR ((unsigned int) &VTABLE);
    /* Initialize INTC for SW vector mode */
    InitINTC();               
    
    /* Enable interrupts */
    enableIrq();

}


