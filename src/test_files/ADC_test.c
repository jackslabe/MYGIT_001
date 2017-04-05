/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê9ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : ADC_test.c
*
* DESCRIPTION:
*
*/

#include "ADC_drv.h"

extern int16_t get_chip_temperature(uint8_t channel);

void adc_test(void){
	volatile int counter = 0;
	uint32_t ret = 0;
	int32_t current1 = 0;
	int32_t current2 = 0;
	int16_t temperature0 = 0;
	int16_t temperature1 = 0;
	ret = adc_calibration(0);
	ret = adc_calibration(1);

	adc_init(0);
	adc_init(1);

	ret = adc_calibration(0);
	ret = adc_calibration(1);

	/* Loop forever */
	//for(;;)
	{
		ret = adc_self_test(0);

		adc_start(0, CH_EN_ALL);
		adc_start(1, CH_EN_ALL);
		while(counter++ < 500000);
		counter = 0;

		temperature0 = get_chip_temperature(0);
		temperature1 = get_chip_temperature(1);

		current1 = get_current(0);

	}
}
