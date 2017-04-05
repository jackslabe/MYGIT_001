/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê9ÔÂ8ÈÕ
* Compiler   : gcc
* FILE NAME  : ADC_drv.h
*
* DESCRIPTION:
*
*/
#ifndef ADC_DRV_H_
#define ADC_DRV_H_

#include "derivative.h"

/* Channel enable mask */
#define CH0_EN			0x00000001
#define CH1_EN			0x00000002
#define CH2_EN			0x00000004
#define CH3_EN			0x00000008
#define CH4_EN			0x00000010
#define CH5_EN			0x00000020
#define CH6_EN			0x00000040
#define CH7_EN			0x00000080
#define CH8_EN			0x00000100
#define CH9_EN			0x00000200
#define CH10_EN			0x00000400
#define CH11_EN			0x00000800
#define CH12_EN			0x00001000
#define CH13_EN			0x00002000
#define CH14_EN			0x00004000
#define CH15_EN			0x00008000

#define CH_EN_ALL		0x0000FFFF
#define CH_DIS_ALL		0x00000000


///MCR parameters
#define OWRITE_DIS		0x00000000									//overwrite disabled
#define OWRITE_EN		0x80000000									//overwrite enable
#define	ALIGN_R			0x00000000									//result aligned to the rigth
#define	ALIGN_L			0x40000000									//result aligned to the left
#define ONE_SHOT		0x00000000									//one shot mode
#define SCAN			0x20000000									//scan mode
#define EXT_TRIG_EN		0x08000000									/*external trigger enable */
#define EXT_TRIG_DIS	0x00000000									/*external trigger disable */
#define FALLING_TRIG	0x00000000									/*start trigger edge detection */
#define RISING_TRIG		0x04000000									/*start trigger edge detection  */
#define CTU_EN			0x00020000
#define CTU_DIS			0x00000000
#define HALF_BUS_CLK	0x00000000
#define BUS_CLK			0x00000100
#define PWDN_DIS		0x00000000									//normal mode
#define PWDN_EN			0x00000001									//power down mode
#define ACKO_EN			0x00000020									//auto clock off enabled
#define ACKO_DIS		0x00000000									//auto clock off disabled
#define DEFAULT_ADC		OWRITE_DIS | SCAN | PWDN_DIS | ALIGN_R | ACKO_DIS	//default setting for ADC


///MSR - ADC status parameters
#define ADC_IDLE	0b000
#define ADC_PWDN	0b001
#define ADC_WAIT	0b010
#define ADC_SAMPLE	0b100
#define ADC_CONV	0b110

///IMR interrupt parameters
#define EOCTU_EN 		0x00000010
#define EOCTU_DIS 		0x00000000
#define JEOC_EN			0x00000008
#define JEOC_DIS		0x00000000
#define JECH_EN			0x00000004
#define JECH_DIS		0x00000000
#define EOC_EN			0x00000002
#define EOC_DIS			0x00000000
#define ECH_EN			0x00000001
#define ECH_DIS			0x00000000

///PSCR presampling parameters
#define VSS		0x00	//Prescaler value = Vss
#define VDD		0x01	//Prescaler value = Vdd

/* Calibration BIST control and status register define */
#define CAL_NORMAL_MODE				0x20000000
#define CAL_HIGH_ACCURACY_MODE		0xC0000000

#define CAL_22_CYCLES				0x00000000
#define CAL_8_CYCLES				0x08000000
#define CAL_16_CYCLES				0x10000000
#define CAL_32_CYCLES				0x18000000

#define CAL_16_SAMPLES				0x00000000
#define CAL_32_SAMPLES				0x00000020
#define CAL_128_SAMPLES				0x00000040
#define CAL_512_SAMPLES				0x00000060

#define CAL_AVAG_EN					0x00000010
#define CAL_AVAG_DIS				0x00000000
#define CAL_TEST_FAIL_CLEAR			0x00000080

extern uint32_t adc0_convert_over_flag;
extern uint32_t adc1_convert_over_flag;


uint16_t adc_read(uint8_t ad_grp, uint8_t ad_ch);
int adc_init(uint8_t ad_grp);
void adc_start(uint8_t ad_grp, uint32_t ch_mask);
uint32_t adc_self_test(uint8_t ad_grp);
uint32_t adc_calibration(uint8_t ad_grp);


#endif /* ADC_DRV_H_ */
