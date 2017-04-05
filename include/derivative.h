/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <MPC5744P.h>


#ifndef _FCCU_H_
#define _FCCU_H_


#define EI() asm("wrteei      1")		/* enable interrupt */
#define DI() asm("wrteei      0")		/* disable interrupt */

#define	FCCU_CFK           	0x618B7A50
#define	FCCU_NCF_KEY         	0xAB3498FE

#define FCCU_OP1  0x01
#define FCCU_OP3  0x03
#define FCCU_OP10 0x0A
#define FCCU_OP15 0x0F


#define FCCU_NCF_KEY  	0xAB3498FE
#define FCCU_CTRLK_OP1	0x913756AF
#define FCCU_CTRLK_OP2	0x825A132B

//different FCCU protocols
#define	FCCU_DUAL_RAIL          0
#define	FCCU_TIME_SWITCHING     1
#define	FCCU_BI_STABLE          2
#define	FCCU_TEST0              5
#define	FCCU_TEST1              6
#define	FCCU_TEST2              7

//keys to enable secured operations in the FCCU_CTRL register
#define	FCCU_KEY_CONFIG         0x913756AF
#define	FCCU_KEY_NORMAL         0x825A132B
#define	FCCU_KEY_LOCK           0x7ACB32F0
#define	FCCU_KEY_NVM_LOAD       0x29AF8752

//different FCCU_CTRL operations
#define	FCCU_OPR_NO                 0
#define	FCCU_OPR_SET_CONFIG         1
#define	FCCU_OPR_SET_NORMAL         2
#define	FCCU_OPR_READ_STAT          3
#define	FCCU_OPR4                   4
#define	FCCU_OPR5                   5
#define	FCCU_OPR6                   6
#define	FCCU_OPR7                   7
#define	FCCU_OPR8                   8
#define	FCCU_OPR_READ_CF	        9
#define	FCCU_OPR_READ_NCF	        10
#define	FCCU_OPR_CLEAR_CF           11
#define	FCCU_OPR_CLEAR_NCF          12
#define	FCCU_OPR_CLEAR_FRZ_STAT     13
#define	FCCU_OPR_CONF_TIMEOUT	    14
#define	FCCU_OPR_CLEAR_OP_STAT	    15
#define	FCCU_OPR_LOCK_FCCU	        16
#define	FCCU_OPR_READ_ALARM	        17
#define	FCCU_OPR_READ_SMRT	        18
#define	FCCU_OPR_READ_CFG	        19
#define	FCCU_OPR_NVM_LOAD	        31

//different FCCU_CTRL status
#define	FCCU_OPS_IDLE           0
#define	FCCU_OPS_IN_PROGR       1
#define	FCCU_OPS_ABORTED        2
#define	FCCU_OPS_SUCCESS        3

//Pad Pull
#define 	SIUL_PULL_UP	0x00030000
#define 	SIUL_PULL_DOWN	0x00010000
#define 	SIUL_PULL_DIS	0x00000000

//Pad dir
#define  	SIUL_INPUT	0x00080000
#define 	SIUL_OUTPUT	0x02000000
#define     SIUL_BIDIR  0x02080000

//Pad function
#define 	SIUL_GPIO	0x0
#define 	SIUL_ALT1	0x1
#define 	SIUL_ALT2	0x2
#define 	SIUL_ALT3	0x3
#define 	SIUL_ALT4	0x4

//Safe mode control
#define 	SIUL_SMC	0x00800000

//Pad type
#define 	SIUL_DIGITAL	0x00000000
#define 	SIUL_ANALOG 	0x00400000

//Pad slew rate
#define 	SIUL_SLEW_ENB 	0x00000000
#define 	SIUL_SLEW_DIS	0x20000000
#define 	SIUL_DRIVE_HALF 0x00000000
#define 	SIUL_DRIVE_FULL 0x1000000

//Output type
#define 	SIUL_NORMAL	0x00000000
#define 	SIUL_OPEND	0x01000000

// external interruption masks EIRQx

#define	SIUL_EIRQ0	0x1
#define	SIUL_EIRQ1	0x2
#define	SIUL_EIRQ2	0x4
#define	SIUL_EIRQ3	0x8
#define	SIUL_EIRQ4	0x10
#define	SIUL_EIRQ5	0x20
#define	SIUL_EIRQ6	0x40
#define	SIUL_EIRQ7	0x80
#define	SIUL_EIRQ8	0x100
#define	SIUL_EIRQ9	0x200
#define	SIUL_EIRQ10	0x400
#define	SIUL_EIRQ11	0x800
#define	SIUL_EIRQ12	0x1000
#define	SIUL_EIRQ13	0x2000
#define	SIUL_EIRQ14	0x4000
#define	SIUL_EIRQ15	0x8000
#define	SIUL_EIRQ16	0x10000
#define	SIUL_EIRQ17	0x20000
#define	SIUL_EIRQ18	0x40000
#define	SIUL_EIRQ19	0x80000
#define	SIUL_EIRQ20	0x100000
#define	SIUL_EIRQ21	0x200000
#define	SIUL_EIRQ22	0x400000
#define	SIUL_EIRQ23	0x800000
#define	SIUL_EIRQ24	0x1000000
#define	SIUL_EIRQ25	0x2000000
#define	SIUL_EIRQ26	0x4000000
#define	SIUL_EIRQ27	0x8000000
#define	SIUL_EIRQ28	0x10000000
#define	SIUL_EIRQ29	0x20000000
#define	SIUL_EIRQ30	0x40000000
#define	SIUL_EIRQ31	0x80000000

// pad list Name versus MSCR[SSS]

#define	SIUL_PA0	0
#define	SIUL_PA1	1
#define	SIUL_PA2	2
#define	SIUL_PA3	3
#define	SIUL_PA4	4
#define	SIUL_PA5	5
#define	SIUL_PA6	6
#define	SIUL_PA7	7
#define	SIUL_PA8	8
#define	SIUL_PA9	9
#define	SIUL_PA10	10
#define	SIUL_PA11	11
#define	SIUL_PA12	12
#define	SIUL_PA13	13
#define	SIUL_PA14	14
#define	SIUL_PA15	15
#define	SIUL_PB0	16
#define	SIUL_PB1	17
#define	SIUL_PB2	18
#define	SIUL_PB3	19
#define	SIUL_PB4	20
#define	SIUL_PB5	21
#define	SIUL_PB6	22
#define	SIUL_PB7	23
#define	SIUL_PB8	24
#define	SIUL_PB9	25
#define	SIUL_PB10	26
#define	SIUL_PB11	27
#define	SIUL_PB12	28
#define	SIUL_PB13	29
#define	SIUL_PB14	30
#define	SIUL_PB15	31
#define	SIUL_PC0	32
#define	SIUL_PC1	33
#define	SIUL_PC2	34
#define	SIUL_PC3	35
#define	SIUL_PC4	36
#define	SIUL_PC5	37
#define	SIUL_PC6	38
#define	SIUL_PC7	39
#define	SIUL_PC8	40
#define	SIUL_PC9	41
#define	SIUL_PC10	42
#define	SIUL_PC11	43
#define	SIUL_PC12	44
#define	SIUL_PC13	45
#define	SIUL_PC14	46
#define	SIUL_PC15	47
#define	SIUL_PD0	48
#define	SIUL_PD1	49
#define	SIUL_PD2	50
#define	SIUL_PD3	51
#define	SIUL_PD4	52
#define	SIUL_PD5	53
#define	SIUL_PD6	54
#define	SIUL_PD7	55
#define	SIUL_PD8	56
#define	SIUL_PD9	57
#define	SIUL_PD10	58
#define	SIUL_PD11	59
#define	SIUL_PD12	60
#define	SIUL_PD13	61
#define	SIUL_PD14	62
#define	SIUL_PD15	63
#define	SIUL_PE0	64
#define	SIUL_PE1	65
#define	SIUL_PE2	66
#define	SIUL_PE3	67
#define	SIUL_PE4	68
#define	SIUL_PE5	69
#define	SIUL_PE6	70
#define	SIUL_PE7	71
#define	SIUL_PE8	72
#define	SIUL_PE9	73
#define	SIUL_PE10	74
#define	SIUL_PE11	75
#define	SIUL_PE12	76
#define	SIUL_PE13	77
#define	SIUL_PE14	78
#define	SIUL_PE15	79
#define	SIUL_PF0	80
#define	SIUL_PF1	81
#define	SIUL_PF2	82
#define	SIUL_PF3	83
#define	SIUL_PF4	84
#define	SIUL_PF5	85
#define	SIUL_PF6	86
#define	SIUL_PF7	87
#define	SIUL_PF8	88
#define	SIUL_PF9	89
#define	SIUL_PF10	90
#define	SIUL_PF11	91
#define	SIUL_PF12	92
#define	SIUL_PF13	93
#define	SIUL_PF14	94
#define	SIUL_PF15	95
#define	SIUL_PG0	96
#define	SIUL_PG1	97
#define	SIUL_PG2	98
#define	SIUL_PG3	99
#define	SIUL_PG4	100
#define	SIUL_PG5	101
#define	SIUL_PG6	102
#define	SIUL_PG7	103
#define	SIUL_PG8	104
#define	SIUL_PG9	105
#define	SIUL_PG10	106
#define	SIUL_PG11	107
#define	SIUL_PG12	108
#define	SIUL_PG13	109
#define	SIUL_PG14	110
#define	SIUL_PG15	111
#define	SIUL_PH0	112
#define	SIUL_PH1	113
#define	SIUL_PH2	114
#define	SIUL_PH3	115
#define	SIUL_PH4	116
#define	SIUL_PH5	117
#define	SIUL_PH6	118
#define	SIUL_PH7	119
#define	SIUL_PH8	120
#define	SIUL_PH9	121
#define	SIUL_PH10	122
#define	SIUL_PH11	123
#define	SIUL_PH12	124
#define	SIUL_PH13	125
#define	SIUL_PH14	126
#define	SIUL_PH15	127
#define	SIUL_PI0	128
#define	SIUL_PI1	129
#define	SIUL_PI2	130
#define	SIUL_PI3	131
#define	SIUL_PI4	132
#define	SIUL_PI5	133
#define	SIUL_PI6	134
#define	SIUL_PI7	135
#define	SIUL_PI8	136
#define	SIUL_PI9	137
#define	SIUL_PI10	138
#define	SIUL_PI11	139
#define	SIUL_PI12	140
#define	SIUL_PI13	141
#define	SIUL_PI14	142
#define	SIUL_PI15	143
#define	SIUL_PJ0	144
#define	SIUL_PJ1	145
#define	SIUL_PJ2	146
#define	SIUL_PJ3	147
#define	SIUL_PJ4	148
#define	SIUL_PJ5	149
#define	SIUL_PJ6	150
#define	SIUL_PJ7	151
#define	SIUL_PJ8	152
#define	SIUL_PJ9	153

// pad list Name versus IMCR[SSS]
#define SIUL_IN_PA0     	173
#define SIUL_IN_PA0_DSPI2	48
#define SIUL_IN_PA3		49
#define SIUL_IN_PB3     165
#define SIUL_IN_PB7     165
#define SIUL_IN_PC7     41
#define SIUL_IN_PD12    166
#define SIUL_IN_PB1		32
#define SIUL_IN_PA15	33
#define SIUL_IN_PF15	34
#define SIUL_IN_PA8		44
#define SIUL_IN_PA2		47
#define SIUL_IN_PD7		50
#define SIUL_IN_PD6	51
#define	SIUL_IN_PC11	52

#endif

