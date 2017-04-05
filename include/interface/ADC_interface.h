/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ1ÈÕ
* Compiler   : gcc
* FILE NAME  : ADC_interface.h
*
* DESCRIPTION:
*
*/
#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#define ADC_VREF	3300	/* 3.3 V , 3300mV */
#define ADC_BIT_LENGTH	12	/* 12 bit */
#define ADC_BIT_VALUE	4096	/* 12 bit */


#define ADC_GRP0  	0
#define ADC_GRP1	1

#define ADC0_CH0	0
#define ADC0_CH1	1
#define ADC0_CH2	2
#define ADC0_CH3	3
#define ADC0_CH4	4
#define ADC0_CH5	5
#define ADC0_CH6	6
#define ADC0_CH7	7
#define ADC0_CH8	8
#define ADC0_CH9	9
#define ADC0_CH10	10
#define ADC0_CH11	11
#define ADC0_CH12	12
#define ADC0_CH13	13
#define ADC0_CH14	14
#define ADC0_CH15	15

#define ADC1_CH0	0
#define ADC1_CH1	1
#define ADC1_CH2	2
#define ADC1_CH3	3
#define ADC1_CH4	4
#define ADC1_CH5	5
#define ADC1_CH6	6
#define ADC1_CH7	7
#define ADC1_CH8	8
#define ADC1_CH9	9
#define ADC1_CH10	10
#define ADC1_CH11	11
#define ADC1_CH12	12
#define ADC1_CH13	13
#define ADC1_CH14	14
#define ADC1_CH15	15

#define CURRENT1_ADC_CH_SMALL		ADC1_CH0
#define CURRENT0_ADC_CH_SMALL 		ADC1_CH1
#define BUS_VOLTAGE_ADC_CH			ADC1_CH2
#define ISO_N_ADC_CH				ADC1_CH3
#define CURRENT0_ADC_CH_WIDE		ADC1_CH4
#define ISO_P_ADC_CH				ADC1_CH6
#define CURRENT1_ADC_CH_WIDE		ADC1_CH7
#define BATT_VOLTAGE_ADC_CH			ADC1_CH8


#define IC_VBG0_ADC_CH				ADC0_CH10
#define IC_TSENS0_ADC_CH			ADC0_CH15

#define VQN7050_2_ADC_CH			ADC0_CH0
#define MPC33908_MUX_ADC_CH			ADC0_CH1
#define VQN7050_3_ADC_CH			ADC0_CH2
#define VQN7050_1_ADC_CH			ADC0_CH3
#define CC2_ADC_CH					ADC0_CH4
#define CP_ADC_CH					ADC0_CH5
#define IDC1_ADC_CH					ADC0_CH6
#define MPC33972_MUX_ADC_CH			ADC0_CH7
#define CC_ADC_CH					ADC0_CH8
#define IDC2_ADC_CH					ADC0_CH11
#define DC12_ADC_CH					ADC0_CH12
#define IB1_ADC_CH					ADC0_CH13
#define IB2_ADC_CH					ADC0_CH14




int32_t get_vnq7050_channel_current(uint8_t chip_num, uint8_t channel);
uint32_t get_vnq7050_vcc(uint8_t chip_num);
int32_t get_vnq7050_temperature(uint8_t chip_num);

uint32_t get_mpc33972_sg_voltage(uint8_t sg_num);

int32_t get_dc12V_current(void);
int32_t get_mpc33908_temperature(void);
uint32_t get_mpc33908_ref_voltage(void);
uint32_t get_mpc33908_io1_voltage(void);
uint32_t get_mpc33908_io0_voltage(void);
uint32_t get_mpc33908_vsense_value(void);

void ad_sample_ctrl(void);
int16_t get_chip_temperature(uint8_t channel);

int32_t get_b1_voltage(void);
int32_t get_b2_voltage(void);
int32_t get_c1_voltage(void);
int32_t get_c2_voltage(void);

int32_t get_cc_voltage(void);
int32_t get_cc2_voltage(void);
int32_t get_cp_voltage(void);

int32_t get_shunt_value(uint8_t channel, uint8_t rang);

int32_t get_bus_value(void);
int32_t get_batt_value(void);

int32_t get_isop_voltage(void);
int32_t get_ison_voltage(void);



#endif /* ADC_INTERFACE_H_ */
