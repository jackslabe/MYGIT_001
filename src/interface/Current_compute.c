/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ31ÈÕ
* Compiler   : gcc
* FILE NAME  : current_compute.c
*
* DESCRIPTION:
*
*/
#include "derivative.h"
#include "ADC_interface.h"

#define CURRENT_SMALL_RANG_MAX_VALUE		60000
#define CURRENT_RANG_NUM		2
#define SHUNT_CH_NUM			2
#define SHUNT_CH_0				0
#define SHUNT_CH_1				1
#define SMALL_RANG				0
#define WIDE_RANG				1

/* Current = m*(a*ADV - b) */
/* m = current_rang/voltage_rang */
/* a: adjust parameter */
/* b: adjust parameter */
/* Current = m*a*ADV - mb */
/* y = ax + b */

struct shunt_attribute{
	float shunt_current_rang;		/* unit: A */
	float shunt_voltage_rang;		/* unit: mV */
};

struct shunt_adjust_parameter{
	float input_low;		/* uint: A */
	float input_high;		/* uint: A */
	float adjust_a;		/* m*a*k */
	float adjust_b;		/* m*b*k */
};

struct current_parameter{
	int32_t calculate_a;
	int32_t calculate_b;
	int32_t current;
};

struct shunt_info{
	struct shunt_attribute shunt_dat;
	struct shunt_adjust_parameter shunt_adjust_dat[SHUNT_CH_NUM][CURRENT_RANG_NUM];
};

struct shunt_info shunt_parameter = {{500.0, 75.0},{
									 {{45.0, 120.0, 0.06713867, 125.0},{45.0, 120.0, 0.26855482, 500.0}},
									 {{45.0, 120.0, 0.06713867, 125.0},{45.0, 120.0, 0.26855482, 500.0}}
									}};

struct current_parameter current_info[SHUNT_CH_NUM][CURRENT_RANG_NUM];

#define CURRENT_MULTIPLE		1000000
#define CURRENT_MULTIPLE_DIV	1000

int32_t shunt_init(void){

	/* read calibration data */

	/* copy data to shunt_parameter */

	/**/
	current_info[SHUNT_CH_0][SMALL_RANG].calculate_a = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_0][SMALL_RANG].adjust_a*CURRENT_MULTIPLE);
	current_info[SHUNT_CH_0][SMALL_RANG].calculate_b = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_0][SMALL_RANG].adjust_b*CURRENT_MULTIPLE);

	current_info[SHUNT_CH_0][WIDE_RANG].calculate_a = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_0][WIDE_RANG].adjust_a*CURRENT_MULTIPLE);
	current_info[SHUNT_CH_0][WIDE_RANG].calculate_b = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_0][WIDE_RANG].adjust_b*CURRENT_MULTIPLE);

	current_info[SHUNT_CH_1][SMALL_RANG].calculate_a = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_1][SMALL_RANG].adjust_a*CURRENT_MULTIPLE);
	current_info[SHUNT_CH_1][SMALL_RANG].calculate_b = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_1][SMALL_RANG].adjust_b*CURRENT_MULTIPLE);

	current_info[SHUNT_CH_1][WIDE_RANG].calculate_a = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_1][WIDE_RANG].adjust_a*CURRENT_MULTIPLE);
	current_info[SHUNT_CH_1][WIDE_RANG].calculate_b = (int32_t)(shunt_parameter.shunt_adjust_dat[SHUNT_CH_1][WIDE_RANG].adjust_b*CURRENT_MULTIPLE);

	return 0;
}

/* Current = m*(a*ADV - b) */
/* return value mA */
int32_t get_current(uint8_t channel){

	int32_t ret_current = 0;
	int32_t a_small = 0;
	int32_t b_small = 0;
	int32_t current_small = 0;
	int32_t adv_small = 0;

	int32_t a_wide = 0;
	int32_t b_wide = 0;
	int32_t current_wide = 0;
	int32_t adv_wide = 0;

	if(channel > SHUNT_CH_NUM){
		channel = 0;
	}
	/* get small rang calibration parameter */
	a_small = current_info[channel][SMALL_RANG].calculate_a;
	b_small = current_info[channel][SMALL_RANG].calculate_b;
	/* get wide rang calibration parameter */
	a_wide = current_info[channel][WIDE_RANG].calculate_a;
	b_wide = current_info[channel][WIDE_RANG].calculate_b;
	/* get ADC value */
	adv_small = get_shunt_value(channel, SMALL_RANG);
	adv_wide = get_shunt_value(channel, WIDE_RANG);
	/* current = ax - b */
	/* calculate small rang current */
	current_small = a_small*adv_small - b_small;
	/* calculate wide rang current */
	current_wide = a_wide*adv_wide - b_wide;

	if(current_wide > CURRENT_SMALL_RANG_MAX_VALUE){
		ret_current = current_wide ;
	}else{
		ret_current = current_small;
	}

	return ret_current/CURRENT_MULTIPLE_DIV;
}

int32_t get_shunt(uint8_t shunt_num, uint8_t rang){

	int32_t ret_current = 0;
	int32_t a_small = 0;
	int32_t b_small = 0;
	int32_t current_small = 0;
	int32_t adv_small = 0;

	int32_t a_wide = 0;
	int32_t b_wide = 0;
	int32_t current_wide = 0;
	int32_t adv_wide = 0;

	if(shunt_num > SHUNT_CH_NUM){
		shunt_num = 0;
	}
	/* get small rang calibration parameter */
	a_small = current_info[shunt_num][SMALL_RANG].calculate_a;
	b_small = current_info[shunt_num][SMALL_RANG].calculate_b;
	/* get wide rang calibration parameter */
	a_wide = current_info[shunt_num][WIDE_RANG].calculate_a;
	b_wide = current_info[shunt_num][WIDE_RANG].calculate_b;
	/* get ADC value */
	adv_small = get_shunt_value(shunt_num, SMALL_RANG);
	adv_wide = get_shunt_value(shunt_num, WIDE_RANG);
	/* current = ax - b */
	/* calculate small rang current */
	current_small = a_small*adv_small - b_small;
	/* calculate wide rang current */
	current_wide = a_wide*adv_wide - b_wide;

	if(current_wide > CURRENT_SMALL_RANG_MAX_VALUE){
		ret_current = current_wide ;
	}else{
		ret_current = current_small;
	}

	if(rang == 0){
		ret_current = current_small;
	}else {
		ret_current = current_wide;
	}

	return ret_current/CURRENT_MULTIPLE_DIV;
}

/* current = K(ADV*Vadref/4096 - Voffset) */
/* y = ax + b */
/* y0 = 0, ax0 = -b */
/* y1 = 45A, y1 = ax1 + b */
/* a = y1/(x1 - x0) */
/* b = -y1*x0/(x1 - x0) */


int32_t calibration_current(uint8_t channel){

	int32_t calibration_status = 0;
	int32_t ad_value_low = 0;
	int32_t ad_value_high = 0;
	static int32_t calibration_step = 0;
	float x0 = 0.0;
	float y1 = 0.0;
	float x1 = 0.0;
	float a = 0.0;
	float b = 0.0;
	int32_t filter_times = 50;
	int8_t adgrp[2][2] = {{CURRENT0_ADC_CH_SMALL, CURRENT0_ADC_CH_WIDE},
						  {CURRENT1_ADC_CH_SMALL, CURRENT1_ADC_CH_WIDE}};


	if(channel > CURRENT_RANG_NUM){
		channel = 0;
	}

	switch(calibration_step){
	case 0:
		/* input = 0A, read advalue, x0 */

		break;
	case 1:
		/* calibration little rang */

		break;
	case 2:
		/* read ad data and filter */
		/* compute calibration parameter */
		ad_value_low = adc_read(ADC_GRP1, adgrp[channel][0]);
		ad_value_high = adc_read(ADC_GRP1, adgrp[channel][1]);
		break;
	case 3:
		/* calibration big rang */
		break;
	case 4:
		/* read ad data and filter */
		/* compute calibration parameter */
		ad_value_low = adc_read(ADC_GRP1, adgrp[channel][0]);
		ad_value_high = adc_read(ADC_GRP1, adgrp[channel][1]);
		break;
	default:
		break;
	}

	return calibration_status;
}

