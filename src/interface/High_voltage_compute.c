/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ31ÈÕ
* Compiler   : gcc
* FILE NAME  : total_voltage_compute.c
*
* DESCRIPTION:
*
*/

#include "ADC_drv.h"
#include "ADC_interface.h"


/* V = K*ADV */
struct voltage_adjust_parameter{
	float voltage_adjust_dat1;
	float voltage_adjust_dat2;
	float voltage_standard_input1;
	float voltage_standard_input2;
};

float voltage_adjust_dat1 = 0.1334;
float voltage_adjust_dat2 = 0.1334;
float voltage_standard_input1 = 100.0;
float voltage_standard_input2 = 100.0;

int32_t voltage_compute_k1 = 0;
int32_t voltage_compute_k2 = 0;

void voltage_compute_init(void){

	/* read storage data */
	voltage_compute_k1 = (int32_t)(voltage_adjust_dat1 * 100000.0);
	voltage_compute_k2 = (int32_t)(voltage_adjust_dat1 * 100000.0);
}


int32_t get_bus_voltage(void){

	int32_t voltage = 0;
	int32_t adv = 0;

	adv = get_bus_value();

	voltage = voltage_compute_k1*adv;
	voltage /= 100;

	return voltage;
}

int32_t get_battery_voltage(void){
	int32_t voltage = 0;
	int32_t adv = 0;

	adv = get_batt_value();

	voltage = voltage_compute_k2 * adv;
	voltage /= 100;

	return voltage;
}

int32_t bus_voltage_adjust(void){
	static int32_t adjust_step = 0;
	int32_t filter_times = 50;

	switch(adjust_step){
	case 1:
		/* wait for input */

		/* check ad value */

		break;
	case 2:
		/* read ad value and filter */
		/* compute adjust k */

		break;
	default:
		/* adjust complete */
		break;
	}

	return adjust_step;
}

int32_t battery_voltage_adjust(void){
	static int32_t adjust_step = 0;

	switch(adjust_step){
	case 1:
		/* wait for input */

		break;
	case 2:
		/* read ad value and filter */
		/* compute adjust k */

		break;
	default:
		/* adjust complete */
		break;
	}

	return adjust_step;
}
