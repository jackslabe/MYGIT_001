/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê9ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : ADC_interface.c
*
* DESCRIPTION:
*
*/

#include "ADC_drv.h"
#include "ADC_interface.h"
#include "GPIO_interface.h"

#include "MPC33908_interface.h"
#include "MPC33972_interface.h"

#define MPC33972_MUX_NUMBER	14
#define MPC33908_MUX_NUMBER 8
#define VNQ7050_NUMBRE	3
#define VNQ7050_SEL_NUM	8

struct ad_map{
	uint16_t ad_mux_33972[MPC33972_MUX_NUMBER];
	uint16_t ad_mux_33908[MPC33908_MUX_NUMBER];
	uint16_t ad_mux_7050[VNQ7050_NUMBRE][VNQ7050_SEL_NUM];
	uint16_t ad_cc_value;
	uint16_t ad_cc2_value;
	uint16_t ad_cp_value;
	uint16_t ad_idc1_value;
	uint16_t ad_idc2_value;
	uint16_t ad_iso_n_value;
	uint16_t ad_iso_p_value;
	uint16_t ad_ib1_value;
	uint16_t ad_ib2_value;
	uint16_t ad_sh01_value;
	uint16_t ad_sh02_value;
	uint16_t ad_sh11_value;
	uint16_t ad_sh12_value;
	uint16_t ad_vbus_value;
	uint16_t ad_vbat_value;
	uint16_t ad_dc12_value;
	uint16_t vbg0_code;
	uint16_t TSENS0_code;
	uint16_t vbg1_code;
	uint16_t TSENS1_code;
};

static struct ad_map ad_value;
/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
void ad_sample_ctrl(void){
	static uint32_t mpc33908_mux_num = 0;
	static uint32_t mpc33972_mux_num = 1;
	static uint32_t vqn7050_sel_num = 0;

	/* start ad converter */
	adc_start(ADC_GRP0, CH_EN_ALL);
	adc_start(ADC_GRP1, CH_EN_ALL);
	/* wait convert end */
	while(adc0_convert_over_flag == 0){
		;
	}
	while(adc1_convert_over_flag == 0){
		;
	}
	/* read ad data and filter */
	ad_value.ad_mux_33972[mpc33972_mux_num - 1] = adc_read(ADC_GRP0, MPC33972_MUX_ADC_CH);
	ad_value.ad_mux_33908[mpc33908_mux_num] = adc_read(ADC_GRP0, MPC33908_MUX_ADC_CH);
	ad_value.ad_mux_7050[0][vqn7050_sel_num] = adc_read(ADC_GRP0, VQN7050_1_ADC_CH);
	ad_value.ad_mux_7050[1][vqn7050_sel_num] = adc_read(ADC_GRP0, VQN7050_2_ADC_CH);
	ad_value.ad_mux_7050[2][vqn7050_sel_num] = adc_read(ADC_GRP0, VQN7050_3_ADC_CH);
	ad_value.ad_cc_value = adc_read(ADC_GRP0, CC_ADC_CH);
	ad_value.ad_cc2_value = adc_read(ADC_GRP0, CC2_ADC_CH);
	ad_value.ad_cp_value = adc_read(ADC_GRP0, CP_ADC_CH);
	ad_value.ad_idc1_value = adc_read(ADC_GRP0, IDC1_ADC_CH);
	ad_value.ad_idc2_value = adc_read(ADC_GRP0, IDC2_ADC_CH);
	ad_value.ad_ib1_value = adc_read(ADC_GRP0, IB1_ADC_CH);
	ad_value.ad_ib2_value = adc_read(ADC_GRP0, IB2_ADC_CH);
	ad_value.ad_dc12_value = adc_read(ADC_GRP0, DC12_ADC_CH);

	ad_value.ad_iso_n_value = adc_read(ADC_GRP1, ISO_N_ADC_CH);
	ad_value.ad_iso_p_value = adc_read(ADC_GRP1, ISO_P_ADC_CH);
	ad_value.ad_sh01_value = adc_read(ADC_GRP1, CURRENT0_ADC_CH_SMALL);
	ad_value.ad_sh02_value = adc_read(ADC_GRP1, CURRENT0_ADC_CH_WIDE);
	ad_value.ad_sh11_value = adc_read(ADC_GRP1, CURRENT1_ADC_CH_SMALL);
	ad_value.ad_sh12_value = adc_read(ADC_GRP1, CURRENT1_ADC_CH_WIDE);
	ad_value.ad_vbus_value = adc_read(ADC_GRP1, BUS_VOLTAGE_ADC_CH);
	ad_value.ad_vbat_value = adc_read(ADC_GRP1, BATT_VOLTAGE_ADC_CH);

	ad_value.vbg0_code = adc_read(ADC_GRP0, IC_VBG0_ADC_CH);;
	ad_value.TSENS0_code = adc_read(ADC_GRP0, IC_TSENS0_ADC_CH);
	ad_value.vbg1_code = adc_read(ADC_GRP1, IC_VBG0_ADC_CH);
	ad_value.TSENS1_code = adc_read(ADC_GRP1, IC_TSENS0_ADC_CH);

	mpc33908_mux_num += 1;
	if(mpc33908_mux_num >= MPC33908_MUX_NUMBER){
		mpc33908_mux_num = 0;
	}

	mpc33972_mux_num += 1;
	if(mpc33972_mux_num > MPC33972_MUX_NUMBER){
		mpc33972_mux_num = 1;
	}

	vqn7050_sel_num += 1;
	if(vqn7050_sel_num >= VNQ7050_SEL_NUM){
		vqn7050_sel_num = 0;
	}

	/* control VNQ7050 to select channel current, channel 0~3 */
	vnq7050_sel_ctrl(vqn7050_sel_num);
	/* control mpc33908 to select amux, vsens, io0, io1, 2.5V ref, temperature */
	mpc33908_mux_set(mpc33908_mux_num);

	/* control mpc33972 to select SG channel */
	mpc33972_set_amux(mpc33972_mux_num);

}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is used for get MPC5744 chip temperature
**
**  Inputs:
**		channel: MPC5744 temperature channel
**	Outputs:
**		None
**  Returns:
**		temperature. unit: 0.01 degrees celsius
**	Notes:
**
**-----------------------------------------------------------------------------
*/

int16_t get_chip_temperature(uint8_t channel){
	int16_t K1_0, K2_0, K3_0, K4_0;
	int16_t K1_1, K2_1, K3_1, K4_1;
	uint16_t Vbg0_code, Vbg1_code, TSENS0_code, TSENS1_code;

	uint16_t *testFlashPtr = (uint16_t *)0x04000C0;	/* UTEST flash flash memory map address */
	int64_t  k1_temp = 0;
	int64_t  k2_temp = 0;
	int64_t  k3_temp = 0;
	int64_t  k4_temp = 0;
	int64_t  k_temp1 = 0;
	int64_t  k_temp2 = 0;
	int64_t  t_sensor = 0;
	int16_t temperature = 0;

	K1_0 = *testFlashPtr++;
	K2_0 = *testFlashPtr++;
	K3_0 = *testFlashPtr++;
	K4_0 = *testFlashPtr++;

	K1_1 = *testFlashPtr++;
	K2_1 = *testFlashPtr++;
	K3_1 = *testFlashPtr++;
	K4_1 = *testFlashPtr;

	switch(channel){
	case 0:
		Vbg0_code = ad_value.vbg0_code;
		TSENS0_code = ad_value.TSENS0_code;

		k1_temp = K1_0 * Vbg0_code;
		k2_temp = K2_0 * TSENS0_code * 16;
		k3_temp = K3_0 * Vbg0_code * 4;
		k4_temp = K4_0 * TSENS0_code;
		k_temp1 = (k1_temp + k2_temp)*100*128;
		k_temp2 = k3_temp + k4_temp;
		t_sensor = k_temp1/k_temp2 - 27315;
		temperature = (int16_t)t_sensor;
		break;
	case 1:
		Vbg1_code = ad_value.vbg1_code;
		TSENS1_code = ad_value.TSENS1_code;

		k1_temp = K1_1 * Vbg1_code;
		k2_temp = K2_1 * TSENS1_code * 16;
		k3_temp = K3_1 * Vbg1_code * 4;
		k4_temp = K4_1 * TSENS1_code;
		k_temp1 = (k1_temp + k2_temp)*100*128;
		k_temp2 = k3_temp + k4_temp;
		t_sensor = k_temp1/k_temp2 - 27315;
		temperature = (int16_t)t_sensor;
		break;
	default:
		temperature = 0;
		break;
	}

	return temperature;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
/* Vout = ADV*3.3/4096   */
/* Vin = k*ADV */

int32_t get_b1_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_ib1_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_b2_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_ib2_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_c1_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_idc1_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_c2_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_idc2_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_cc_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_cc_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_cc2_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_cc2_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_cp_voltage(void){
	int32_t adv = 0;
	int32_t voltage = 0;
	adv = ad_value.ad_cp_value;
	voltage = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return voltage;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t get_mpc33908_vsense_value(void){
	uint32_t vsense = 0;
	uint32_t adv = 0;

	adv = ad_value.ad_mux_33908[MUX_VSNS_WIDE_RANG];
	vsense = (adv * ADC_VREF) >> ADC_BIT_LENGTH;
	vsense *= WIDE_RANG_V3_3_RATIO_7;

	return vsense;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t get_mpc33908_io0_voltage(void){
	uint32_t vsense = 0;
	uint32_t adv = 0;

	adv = ad_value.ad_mux_33908[MUX_IO_0_WIDE_RANG];
	vsense = (adv * ADC_VREF) >> ADC_BIT_LENGTH;
	vsense *= WIDE_RANG_V3_3_RATIO_7;

	return vsense;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t get_mpc33908_io1_voltage(void){
	uint32_t vsense = 0;
	uint32_t adv = 0;

	adv = ad_value.ad_mux_33908[MUX_IO_1_WIDE_RANG];
	vsense = (adv * ADC_VREF) >> ADC_BIT_LENGTH;
	vsense *= WIDE_RANG_V3_3_RATIO_7;

	return vsense;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1mV/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t get_mpc33908_ref_voltage(void){
	uint32_t vsense = 0;
	uint32_t adv = 0;

	adv = ad_value.ad_mux_33908[MUX_VREF];
	vsense = (adv * ADC_VREF) >> ADC_BIT_LENGTH;

	return vsense;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001 ¡ãC/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
/* Die temperature sensor T(¡ãC) = (VAMUX - VAMUX_TP) / VAMUX_TP_CO + 165 */

#define MPC33908_MUX_TP			(MUX_TP*1000.0)
#define MPC33908_MUX_TP_CO		(MUX_TP_CO*10.0)
#define MPC33908_MUX_TP_OFFSET	165

int32_t get_mpc33908_temperature(void){
	int32_t mux_tp = MPC33908_MUX_TP;
	int32_t mux_tp_co = MPC33908_MUX_TP_CO;
	int32_t mux_tp_offset = MPC33908_MUX_TP_OFFSET * 1000;
	int32_t adv = 0;
	int32_t vsense = 0;
	int32_t temp = 0;

	adv = ad_value.ad_mux_33908[MUX_TEMPRATURE];
	vsense = (adv * ADC_VREF*100) >> ADC_BIT_LENGTH;
	mux_tp *= 10000;
	vsense *= 100;
	temp = (vsense - mux_tp)/mux_tp_co + mux_tp_offset;

	return temp;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001A/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_dc12V_current(void){
	int32_t adv = 0;
	int32_t current = 0;
	adv = ad_value.ad_dc12_value;

	current = (adv * ADC_VREF*100) >> ADC_BIT_LENGTH;
	current *= 5;

	return current/100;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001A/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/

int32_t get_vnq7050_channel_current(uint8_t chip_num, uint8_t channel){
	int32_t current = 0;
	if((chip_num > VNQ7050_NUMBRE) || channel > VNQ7050_SEL_NUM){
		/* parameter error */
	}else{
		current = (ad_value.ad_mux_7050[chip_num][channel]*470)/1000;
	}

	return current;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001V/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
#define VNQ7050_VSENSEVCC 	4
#define VNQ7050_VSENSETC	2070
#define VNQ7050_DV_DT		-55
#define VNQ7050_Tj			25

uint32_t get_vnq7050_vcc(uint8_t chip_num){
	uint32_t vcc = 0;
	uint32_t vsense = 0;
	if(chip_num > VNQ7050_NUMBRE){
		/* parameter error */
	}else{
		vcc = ad_value.ad_mux_7050[chip_num][5];
		vsense = (vcc * ADC_VREF) >> ADC_BIT_LENGTH;
		vcc = vsense * VNQ7050_VSENSEVCC;
	}
	return vcc;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		1 ¡ãC/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_vnq7050_temperature(uint8_t chip_num){
	int32_t temperature = 0;
	int32_t vsense = 0;
	int32_t vdiff = 0;
	if(chip_num > VNQ7050_NUMBRE){
		/* parameter error */
	}else{
		vsense = (ad_value.ad_mux_7050[chip_num][4] * ADC_VREF) >> ADC_BIT_LENGTH;
		vdiff = vsense - VNQ7050_VSENSETC;
		temperature = (vdiff*1000)/VNQ7050_DV_DT;
		temperature /= 100;
		temperature += VNQ7050_Tj;
	}
	return temperature;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001V/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t get_mpc33972_sg_voltage(uint8_t sg_num){
	uint32_t sg_voltage = 0;

	if(sg_num < MPC33972_MUX_NUMBER){
		sg_voltage = ad_value.ad_mux_33972[sg_num];
		sg_voltage = (sg_voltage*ADC_VREF) >> ADC_BIT_LENGTH;
	}

	return sg_voltage;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is get current channel Analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		value
**	Notes:
**
**-----------------------------------------------------------------------------
*/
#define CURRENT_RANG_NUM		2
#define SHUNT_CH_NUM			2

int32_t get_shunt_value(uint8_t channel, uint8_t rang){

	uint16_t adgrp[SHUNT_CH_NUM][CURRENT_RANG_NUM] = {{ad_value.ad_sh01_value, ad_value.ad_sh02_value},
													  {ad_value.ad_sh11_value, ad_value.ad_sh12_value}};
	if(channel >= SHUNT_CH_NUM || rang >= CURRENT_RANG_NUM){
		channel = 0;
		rang = 0;
	}

	return adgrp[channel][rang];
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/

int32_t get_bus_value(void){
	return ad_value.ad_vbus_value;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is get batter analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_batt_value(void){
	return ad_value.ad_vbat_value;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001V/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_isop_voltage(void){
	uint32_t data = ad_value.ad_iso_p_value;

	data = (data*ADC_VREF) >> ADC_BIT_LENGTH;

	return data;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		0.001V/bit
**	Notes:
**
**-----------------------------------------------------------------------------
*/
int32_t get_ison_voltage(void){
	uint32_t data = ad_value.ad_iso_n_value;

	data = (data*ADC_VREF) >> ADC_BIT_LENGTH;

	return data;
}



