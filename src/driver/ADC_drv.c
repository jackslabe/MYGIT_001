/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê9ÔÂ7ÈÕ
* Compiler   : gcc
* FILE NAME  : ADC_drv.c
*
* DESCRIPTION:
*
*/

#include "ADC_drv.h"

/* configuration define */
#define EXT_TRIG_MODE	0
#define CTU_TRIG_MODE 	1
#define SW_TRIG_MODE	2

#define POLLING_MODE	0
#define INTERRUPT_MODE	1

#define ONE_SHOT_MODE	0
#define CAN_MODE		1

#define SELF_TEST_EN	1
#define CALIBRATION_EN	1

#define DAT_ACCESS_MODE		INTERRUPT_MODE
#define TRIG_MODE			SW_TRIG_MODE

/* Normal conversion mask register configuration */
#define ADC0_NCMR0_CFG	(CH_EN_ALL)
#define ADC1_NCMR0_CFG	(CH_EN_ALL)
/* injected conversion mask register configuration */
#define ADC0_JCMR0_CFG	(CH_DIS_ALL)
#define ADC1_JCMR0_CFG	(CH_DIS_ALL)

/* main configruation , scan mode, align, overwrite, powerdown */
#define ADC_MCR_CFG		(OWRITE_EN | ALIGN_R | ONE_SHOT | PWDN_DIS | ACKO_DIS | HALF_BUS_CLK | CTU_DIS | EXT_TRIG_DIS)

/* sample time configuration */
/* total conversion time = trigger time + sample time + compare time + data process time */
/* total conversion time = 2cycles      + CTRx        + (n+1)*4      + 2cyeles  */
/* ADC_CALBISTREG.OPMODE = NORMAL mode, n = 12cycles */
/* ADC_CALBISTREG.OPMODE = High accuracy mode, n = 13cycles */

/* CTR0 controls sampling time for precision channels */
#define ADC_CTR0_CFG	0x64
/* CTR1 controls sampling time for Temperature Sensor channel*/
#define ADC_CTR1_CFG	0x50

/* calibration configuration */
#define ADC_CALBISTREG_CFG	(CAL_HIGH_ACCURACY_MODE | CAL_22_CYCLES | CAL_512_SAMPLES | CAL_AVAG_EN | CAL_TEST_FAIL_CLEAR)

/* interrupt configuration */
#if (DAT_ACCESS_MODE == POLLING)
#define ADC_IMR_CFG		(EOCTU_DIS | JEOC_DIS | JECH_DIS | EOC_DIS | ECH_DIS)
#define ADC_CIMR0_CFG	(CH_DIS_ALL)
#define ADC_ISR_CLEAR	(0x000000FF)
#elif(DAT_ACCESS_MODE == INTERRUPT_MODE)
#define ADC_IMR_CFG		(EOCTU_DIS | JEOC_DIS | JECH_DIS | EOC_EN | ECH_EN)
#define ADC_CIMR0_CFG	(CH_EN_ALL)
#define ADC_ISR_CLEAR	(0x000000FF)
#endif


static uint32_t adc_buf[2][16] = {{0}};
uint32_t adc0_convert_over_flag = 0;
uint32_t adc1_convert_over_flag = 0;


void adc0_err_irq_handler(void){

}

void adc1_err_irq_handler(void){

}

#define ADC_MAX_CHANNEL_NUM		16

uint16_t adc_read(uint8_t ad_grp, uint8_t ad_ch){
	uint16_t adc_dat = 0;

	if(ad_ch >= ADC_MAX_CHANNEL_NUM){
		/* ad_ch error */
	}else{
		switch(ad_grp){
		case 0:
			adc_dat = adc_buf[0][ad_ch];
			break;
		case 1:
			adc_dat = adc_buf[1][ad_ch];
			break;
		default:
			break;
		}
	}

	return adc_dat;
}

uint16_t adc_read_synchron(uint8_t ad_grp, uint8_t ad_ch){
	uint16_t adc_dat = 0;
	/* disable interrupt */
	/* abort ad converter */
	/* start ad converter */
	/* wait convert over */
	/* get convert data */

	if(ad_ch >= ADC_MAX_CHANNEL_NUM){
		/* ad_ch error */
	}else{
		switch(ad_grp){
		case 0:
			adc_dat = adc_buf[0][ad_ch];
			break;
		case 1:
			adc_dat = adc_buf[1][ad_ch];
			break;
		default:
			break;
		}
	}

	return adc_dat;
}

int adc_init(uint8_t ad_grp){
	int ret = 0;
	volatile struct ADC_tag *p_ADC = 0;

	switch(ad_grp){
		case 0 :
			p_ADC = &ADC_0;

			SIUL2.MSCR[SIUL_PB7].R = SIUL_ANALOG | 0x10000000;		//AN0
			SIUL2.MSCR[SIUL_PB8].R = SIUL_ANALOG | 0x10000000;		//AN1
			SIUL2.MSCR[SIUL_PC1].R = SIUL_ANALOG | 0x10000000;		//AN2
			SIUL2.MSCR[SIUL_PC2].R = SIUL_ANALOG | 0x10000000;		//AN3
			SIUL2.MSCR[SIUL_PE6].R = SIUL_ANALOG | 0x10000000;		//AN4
			SIUL2.MSCR[SIUL_PE2].R = SIUL_ANALOG | 0x10000000;		//AN5
			SIUL2.MSCR[SIUL_PE7].R = SIUL_ANALOG | 0x10000000;		//AN6
			SIUL2.MSCR[SIUL_PE4].R = SIUL_ANALOG | 0x10000000;		//AN7
			SIUL2.MSCR[SIUL_PE5].R = SIUL_ANALOG | 0x10000000;		//AN8
			SIUL2.MSCR[SIUL_PB9].R = SIUL_ANALOG | 0x10000000;		//AN11	shared between ADC0 and ADC1
			SIUL2.MSCR[SIUL_PB10].R = SIUL_ANALOG | 0x10000000;		//AN12	shared between ADC0 and ADC1
			SIUL2.MSCR[SIUL_PB11].R = SIUL_ANALOG | 0x10000000;		//AN13	shared between ADC0 and ADC1
			SIUL2.MSCR[SIUL_PB12].R = SIUL_ANALOG | 0x10000000;		//AN14	shared between ADC0 and ADC1
			/* main control register */
		 	p_ADC->MCR.R = (vuint32_t)ADC_MCR_CFG;
			//Injected conversion
			p_ADC->JCMR0.R = (vuint32_t)ADC0_JCMR0_CFG;
			//Normal conversion
			p_ADC->NCMR0.R = (vuint32_t)ADC0_NCMR0_CFG;		//sampling enabled for masked channels

		break;
		case 1 :
			p_ADC = &ADC_1;

			SIUL2.MSCR[SIUL_PB13].R = SIUL_ANALOG | 0x10000000;		//AN0
			SIUL2.MSCR[SIUL_PB14].R = SIUL_ANALOG | 0x10000000;		//AN1
			SIUL2.MSCR[SIUL_PB15].R = SIUL_ANALOG | 0x10000000;		//AN2
			SIUL2.MSCR[SIUL_PC0].R = SIUL_ANALOG | 0x10000000;		//AN3
			SIUL2.MSCR[SIUL_PE11].R = SIUL_ANALOG | 0x10000000;		//AN4
			SIUL2.MSCR[SIUL_PE0].R = SIUL_ANALOG | 0x10000000;		//AN5
			SIUL2.MSCR[SIUL_PE12].R = SIUL_ANALOG | 0x10000000;		//AN6
			SIUL2.MSCR[SIUL_PE9].R = SIUL_ANALOG | 0x10000000;		//AN7
			SIUL2.MSCR[SIUL_PE10].R = SIUL_ANALOG | 0x10000000;		//AN8
			SIUL2.MSCR[SIUL_PB9].R = SIUL_ANALOG | 0x10000000;		//AN11	shared between ADC0 and ADC1
			SIUL2.MSCR[SIUL_PB10].R = SIUL_ANALOG | 0x10000000;		//AN12	shared between ADC0 and ADC1
			SIUL2.MSCR[SIUL_PB11].R = SIUL_ANALOG | 0x10000000;		//AN13	shared between ADC0 and ADC1
			SIUL2.MSCR[SIUL_PB12].R = SIUL_ANALOG | 0x10000000;		//AN14	shared between ADC0 and ADC1
			/* main control register */
		 	p_ADC->MCR.R = (vuint32_t)ADC_MCR_CFG;
			//Injected conversion
			p_ADC->JCMR0.R = (vuint32_t)ADC1_JCMR0_CFG;		//sampling enabled for masked channels
			//Normal conversion
			p_ADC->NCMR0.R = (vuint32_t)ADC1_NCMR0_CFG;		//sampling enabled for masked channels

		break;
		default	:
			ret = -1;
		break;
	}

	//Timing register
	p_ADC->CTR0.R = (vuint32_t)ADC_CTR0_CFG;
	p_ADC->CTR1.R = (vuint32_t)ADC_CTR1_CFG;	// setup conversion time, max value
	/*Interrupt configure, ADC_ISR,  ADC_IMR, CIMR0,  */
	p_ADC->ISR.R = ADC_ISR_CLEAR;
	p_ADC->IMR.R = (vuint32_t)ADC_IMR_CFG;
	p_ADC->CIMR0.R = (vuint32_t)ADC_CIMR0_CFG;

	return ret;
}


#if (DAT_ACCESS_MODE == POLLING)

#elif(DAT_ACCESS_MODE == INTERRUPT_MODE)

void adc_clear_isr(uint8_t ad_grp){
	volatile struct ADC_tag *p_ADC;
	switch(ad_grp){
			case 0 : p_ADC = &ADC_0; break;
			case 1 : p_ADC = &ADC_1; break;
			case 2 : p_ADC = &ADC_2; break;
			case 3 : p_ADC = &ADC_3; break;
			default	: 	p_ADC = &ADC_0; break;
		}
	p_ADC->CEOCFR0.R = ADC_ISR_CLEAR;		//clear all end of conversion flags
	p_ADC->ISR.R = ADC_ISR_CLEAR;
}

void adc0_irq_handler(void){
	int i = 0;
	volatile struct ADC_tag *p_ADC = &ADC_0;

	for(i = 0; i < 16; i ++){
		adc_buf[0][i] = p_ADC->CDR[i].B.CDATA;
	}

	adc_clear_isr(0);
	adc0_convert_over_flag = 1;
}


void adc1_irq_handler(void){
	int i = 0;
	volatile struct ADC_tag *p_ADC = &ADC_1;

	for(i = 0; i < 16; i ++){
		adc_buf[1][i] = p_ADC->CDR[i].B.CDATA;
	}

	adc_clear_isr(1);
	adc1_convert_over_flag = 1;
}
#endif

#if (TRIG_MODE == EXT_TRIG_MODE)

#elif(TRIG_MODE == CTU_TRIG_MODE)

#elif(TRIG_MODE == SW_TRIG_MODE)
void adc_start(uint8_t ad_grp, uint32_t ch_mask){
	volatile struct ADC_tag *p_ADC;
	//pointer settings
	switch(ad_grp){
			case 0 :
				p_ADC = &ADC_0;
				p_ADC->NCMR0.R = ch_mask;		//sampling enabled for masked channels
				adc0_convert_over_flag = 0;
				p_ADC->MCR.B.NSTART = 1;
			break;
			case 1 :
				p_ADC = &ADC_1;
				p_ADC->NCMR0.R = ch_mask;		//sampling enabled for masked channels
				adc1_convert_over_flag = 0;
				p_ADC->MCR.B.NSTART = 1;
			break;
			default	:
				p_ADC = &ADC_0;
			break;
		}
}

#endif


#define STSR_ERROR_BIT_MASK		0x0B00B800

#if SELF_TEST_EN
uint32_t adc_self_test(uint8_t ad_grp){
	/* backup register */
	/* 1. abort convertion, save current ADC register status */
	/* 2. //put ADC in power down mode */
	/* 3. modify ADC_CTR0.INPSAMP = 20d */
	/* 4. modify ADC_MCR.MODE = SCAN */
	/* 5. set STCR1.INPSAMP_C = 0x80, STCR1.INPSAMP_S = 0x80 */
	/* 6. set STCR3.ALG = 11b, Algorithm S + Algorithm C (default)*/
	/* 7. enable watchdog */
	/* 8. setup */
	/* 7. enable STCR2.EN = 1, */
	/* 8. start normal convertion */
	/* 9. wait for MSR.SELF_TEST set */
	/* 10. wait for MSR.SELF_TEST reset, or wait ISR.EOC, ISR.ECH, STSR2.ST_EOC set */
	/* 11. clear ISR */
	/* 12. read STDR1.TCDATA , read STDR2.IDATA, read STDR2.FDATA */
	/* 13. */
	/* recover register */

	volatile struct ADC_tag *p_ADC = &ADC_0;
	volatile uint16_t *p_utest_adc = (uint16_t *)0x004000D0;
	uint16_t adc_threshold_value[16] = {0};
	uint32_t  __attribute__((unused))stdr1_dat[3] = {0};
	uint32_t  __attribute__((unused))stdr2_dat[3] = {0};
	uint32_t  __attribute__((unused))stc_dat[12] = {0};
	uint32_t st_error = 0;
	uint16_t  __attribute__((unused))v_bandgap = 0;

	uint32_t adc_mcr_backup = 0;
	uint32_t adc_msr_backup = 0;
	uint32_t adc_imr_backup = 0;
	uint32_t adc_ctr0_backup = 0;
	uint32_t adc_ctr1_backup = 0;
	uint32_t adc_ncmr0_backup = 0;

	switch(ad_grp){
	case 0:
		p_ADC = &ADC_0;
	break;
	case 1:
		p_ADC = &ADC_1;
	break;
	default:
		p_ADC = &ADC_0;
	break;
	}

	int i = 0;
	for(i = 0; i < 15; i ++, p_utest_adc ++){
		adc_threshold_value[i] = *p_utest_adc;
	}

	adc_mcr_backup = p_ADC->MCR.R;
	adc_msr_backup = p_ADC->MSR.R;
	adc_imr_backup = p_ADC->IMR.R;
	adc_ctr0_backup = p_ADC->CTR0.R;
	adc_ctr1_backup = p_ADC->CTR1.R;
	adc_ncmr0_backup = p_ADC->NCMR0.R;

	/* ADC_STAW0R.AWDE  analog watchdog enable for algorithm S step 0*/
	/* ADC_STAW0R.WDTE  watchdog timer enable for algorithm S step 0-2 */
	/* ADC_STAW0R.THRH/THRL High/low threshold value for algorithm S step 0 */
	p_ADC->STAW0R.B.AWDE = 1;
	p_ADC->STAW0R.B.WDTE = 1;
	p_ADC->STAW0R.B.THRH = adc_threshold_value[0];
	p_ADC->STAW0R.B.THRL = adc_threshold_value[1];

	/* ADC_STAW1AR.AWDE alalog watchdog enable for algorithms step 1 */
	/* ADC_STAW1AR.THRH/THRL High/Low threshold vale for algorithm s step 1 , integer part */
	/* ADC_STAW1BR.THRH/THRL High/Low threshold vale for algorithm s step 1 , fractional part */
	p_ADC->STAW1AR.B.AWDE = 1;
	p_ADC->STAW1AR.B.THRH = adc_threshold_value[4];
	p_ADC->STAW1AR.B.THRL = adc_threshold_value[5];
	p_ADC->STAW1BR.B.THRH = adc_threshold_value[6];
	p_ADC->STAW1BR.B.THRL = adc_threshold_value[7];

	/* ADC_STAW2R.AWDE  analog watchdog enable for algorithm S step 2 */
	/* ADC_STAW2R.THRL  Low threshold value for algorithm S step 2*/
	p_ADC->STAW2R.B.AWDE = 1;
	p_ADC->STAW2R.B.THRL = adc_threshold_value[9];

	/* ADC_STAW4R.AWDE	analog watchdog enable for algorithm C  */
	/* ADC_STAW4R.WDTE  watchdog timer enable for algorithm C */
	/* ADC_STAW4R.THRH/THRL High/low threshold value for algorithm C step 0 */
	/* STDR1.TCDATA > STAW4R.THRH then STSR1.ERR_C is set */
	/* STDR1.TCDATA < STAW4R.THRH then STSR1.ERR_C is set */
	p_ADC->STAW4R.B.AWDE = 1;
	p_ADC->STAW4R.B.WDTE = 1;
	p_ADC->STAW4R.B.THRH = adc_threshold_value[10];
	p_ADC->STAW4R.B.THRL = adc_threshold_value[11];

	/* ADC_STAW5R.THRH/THRL High/low threshold value for algorithm C step 1 - 11 */
	/* (STDR1.TCDATA(step n) - STDR1.TCDATA(step 0) > STAW5R.THRH) STSR1.ERR_C is set */
	/* (STDR1.TCDATA(step n) - STDR1.TCDATA(step 0) < STAW5R.THRL) STSR1.ERR_C is set */
	p_ADC->STAW5R.B.THRH = adc_threshold_value[12];
	p_ADC->STAW5R.B.THRL = adc_threshold_value[13];

	/* */
	p_ADC->IMR.R = 0x00000000;			/* disable interrupt */
	p_ADC->CIMR0.R = CH_DIS_ALL;		/* disable channel interrupt */
	p_ADC->NCMR0.R = CH10_EN;			/* enable channel 10 */
	p_ADC->CTR0.R = 0x00000014;			/* convert time 20d */
	p_ADC->MCR.B.MODE = 0;				/* one shot mode */
	p_ADC->STBRR.B.WDT = 2;				/* 010b: 0.1ms, 001b: 0.5ms, 010b: 1ms, 011b: 2ms, 100b: 5ms, 101b: 10ms, 110b: 20ms, 111b: 50ms */
	p_ADC->STCR1.B.INPSAMP_C = 0x18;	/* self test sample C */
	p_ADC->STCR1.B.INPSAMP_S = 0x50;	/* self test sample S */
	p_ADC->STCR3.B.ALG = 2;				/* 0: algorithm S. 2: algorithm C */
	p_ADC->STCR2.B.EN = 1;

	/* Algorithm S test step */
	p_ADC->STCR3.B.ALG = 0;				/* 0: algorithm S. 2: algorithm C */
	for(i = 0; i < 3; i ++){
		p_ADC->STCR3.B.MSTEP = i;
		p_ADC->MCR.B.NSTART = 1;

		/* wait for self test over */
		while(p_ADC->STSR1.B.ST_EOC == 0);

		p_ADC->STSR1.B.ST_EOC = 1;

		stdr1_dat[i] = p_ADC->STDR1.R;
		stdr2_dat[i] = p_ADC->STDR2.R;
		v_bandgap = p_ADC->CDR[10].B.CDATA;
	}
	/* Algorithm C test step */
	p_ADC->STCR3.B.ALG = 2;				/* 0: algorithm S. 2: algorithm C */
	for(i = 0; i < 12; i ++){
		p_ADC->STCR3.B.MSTEP = i;
		p_ADC->MCR.B.NSTART = 1;

		/* wait for self test over */
		while(p_ADC->STSR1.B.ST_EOC != 1);

		p_ADC->STSR1.B.ST_EOC = 1;			/* clear flag */

		stc_dat[i] = p_ADC->STDR1.R;
	}

	st_error = p_ADC->STSR1.R & STSR_ERROR_BIT_MASK;

	p_ADC->MCR.R = adc_mcr_backup;
	p_ADC->MSR.R = adc_msr_backup;
	p_ADC->IMR.R = adc_imr_backup;
	p_ADC->CTR0.R = adc_ctr0_backup;
	p_ADC->CTR1.R = adc_ctr1_backup;
	p_ADC->NCMR0.R = adc_ncmr0_backup;

	return st_error;
}

#endif

#if CALIBRATION_EN
uint32_t adc_calibration(uint8_t ad_grp){
	/* 1. abort convertion */
	/* 2. put ADC in power down mode */
	/* 3. set ADC clock 40MHz */
	/* 4. power up ADC */
	/* 5. start calibration, set TEST_EN */
	/* 6. wait for calibration to complete*/
	/* 7. check calibration result */
	/* 8. recovery ADC */
	volatile struct ADC_tag *p_ADC = 0;
	uint32_t adc_mcr_backup = 0;
	uint32_t adc_msr_backup = 0;
	uint32_t adc_imr_backup = 0;
	uint32_t adc_ctr0_backup = 0;
	uint32_t adc_ctr1_backup = 0;
	uint32_t adc_ncmr0_backup = 0;
	uint32_t cal_result = 0;

	switch(ad_grp){
	case 0:
		p_ADC = &ADC_0;
	break;
	case 1:
		p_ADC = &ADC_1;
	break;
	default:
		p_ADC = &ADC_0;
	break;
	}

	adc_mcr_backup = p_ADC->MCR.R;
	adc_msr_backup = p_ADC->MSR.R;
	adc_imr_backup = p_ADC->IMR.R;
	adc_ctr0_backup = p_ADC->CTR0.R;
	adc_ctr1_backup = p_ADC->CTR1.R;
	adc_ncmr0_backup = p_ADC->NCMR0.R;

	p_ADC->IMR.R = 0;						/* disable interrupt */
	p_ADC->NCMR0.R = 0;						/* disable all channel */


	p_ADC->MCR.B.PWDN = 1;
	//while(p_ADC->MSR.B.ADCSTATUS != 1);		/* wait for powerdown */

	p_ADC->MCR.B.ADCLKSEL = 0;				/* ADC clock frequency is half of bus clock */

	p_ADC->MCR.B.PWDN = 0;					/* power up */
	//while(p_ADC->MSR.B.ADCSTATUS != 0);		/* wait for power up */

	p_ADC->CALBISTREG.R = ADC_CALBISTREG_CFG;

	p_ADC->CALBISTREG.B.TEST_EN = 1; 		/* enable calibration */

	/* wait for calibration complete, MSR.ADCSTATUS or CALBISTREG.C_T_BUSY */
	//while(p_ADC->MSR.B.ADCSTATUS == 3);	/* 000b: idle */
											/* 001b: power down */
											/* 100b: sample */
											/* 110b: conversion */
											/* 010b: wait state (external trigger)*/
											/* 011b: busy in calibrationo */

	volatile int temp = 0;
	while(p_ADC->CALBISTREG.B.C_T_BUSY){
		temp ++;
	}
	/* get calibration result */
	cal_result = p_ADC->CALBISTREG.B.TEST_FAIL;
	/* recovery normal register */
	p_ADC->MCR.R = adc_mcr_backup;
	p_ADC->MSR.R = adc_msr_backup;
	p_ADC->IMR.R = adc_imr_backup;
	p_ADC->CTR0.R = adc_ctr0_backup;
	p_ADC->CTR1.R = adc_ctr1_backup;
	p_ADC->NCMR0.R = adc_ncmr0_backup;

	return cal_result;
}

#endif




