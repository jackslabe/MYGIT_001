/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ10ÈÕ
* Compiler   : gcc
* FILE NAME  : GPIO_interface.c
*
* DESCRIPTION:
*
*/

#include "derivative.h" /* include peripheral declarations */
#include "SIUL.h"

#define ISO_P_GPIO			107
#define ISO_N_GPIO			1
#define LED1_GPIO			106
#define LED2_GPIO			59
#define S2_GPIO				101
#define VNQ7050_K1_GPIO		46
#define VNQ7050_K2_GPIO		62
#define VNQ7050_K3_GPIO		48
#define VNQ7050_K4_GPIO		80
#define VNQ7050_K5_GPIO		60
#define VNQ7050_K6_GPIO		45
#define VNQ7050_K7_GPIO		47
#define VNQ7050_K8_GPIO		52
#define VNQ7050_K9_GPIO		152
#define VNQ7050_KA_GPIO		102
#define VNQ7050_KB_GPIO		51
#define VNQ7050_KC_GPIO		78

#define VNQ7050_SEL0_GPIO	77
#define VNQ7050_SEL1_GPIO	93
#define VNQ7050_SEL2_GPIO	92

#define SYSTEM_CLOCK_OUT_GPIO	22

#define M25PE40_WP_GPIO		84

#define MPC33908_FS0B_GPIO	104
#define MPC33908_INTB_GPIO	79

#define MPC33664_INTB_GPIO	86
#define MPC33664_EN_GPIO	85

#define FS_GPIO				103


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
void gpio_output_init(void){
	/* led1 */
	/* led2 */
	/* k1 ~ kc */
	/* wp_flash */
	/* 33664_en */
	/* sel10 ~ 12*/
	/* s2_ctrl */
	/* system clock output */
	SIUL_DigitalOutput(LED1_GPIO, 0);
	SIUL_DigitalOutput(LED2_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K1_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K2_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K3_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K4_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K5_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K6_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K7_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K8_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_K9_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_KA_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_KB_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_KC_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_SEL0_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_SEL1_GPIO, 0);
	SIUL_DigitalOutput(VNQ7050_SEL2_GPIO, 0);
	SIUL_DigitalOutput(SYSTEM_CLOCK_OUT_GPIO, SIUL_ALT1);
	SIUL_DigitalOutput(M25PE40_WP_GPIO, 0);
	SIUL_DigitalOutput(MPC33664_EN_GPIO, 0);
	SIUL_DigitalOutput(FS_GPIO, 0);
	SIUL_DigitalOutput(S2_GPIO, 0);
	SIUL_DigitalOutput(ISO_P_GPIO, 0);
	SIUL_DigitalOutput(ISO_N_GPIO, 0);
}

void gpio_input_init(void){

	/* read FS0B */
	/* mpc33908_intb */
	/* mpc33664_intb */
	SIUL_DigitalInputSimple(MPC33908_FS0B_GPIO, 0);
	SIUL_DigitalInputSimple(MPC33908_INTB_GPIO, 0);
	SIUL_DigitalInputSimple(MPC33664_INTB_GPIO, 0);
}

uint32_t mpc33908_fs0b_input(void){
	return SIUL_GetPadState(MPC33908_FS0B_GPIO);
}

uint32_t mpc33908_intb_input(void){
	return SIUL_GetPadState(MPC33908_INTB_GPIO);
}


void iso_p_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(ISO_P_GPIO);
	}else{
		SIUL_ClearPad(ISO_P_GPIO);
	}
}

void iso_n_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(ISO_N_GPIO);
	}else{
		SIUL_ClearPad(ISO_N_GPIO);
	}
}

void led1_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(LED1_GPIO);
	}else{
		SIUL_ClearPad(LED1_GPIO);
	}
}

void led2_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(LED2_GPIO);
	}else{
		SIUL_ClearPad(LED2_GPIO);
	}
}

void fs_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(FS_GPIO);
	}else{
		SIUL_ClearPad(FS_GPIO);
	}
}

void s2_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(S2_GPIO);
	}else{
		SIUL_ClearPad(S2_GPIO);
	}
}

void vnq7050_sel_ctrl(uint8_t sel_num){
	switch(sel_num){
	case 0:
		SIUL_ClearPad(VNQ7050_SEL0_GPIO);
		SIUL_ClearPad(VNQ7050_SEL1_GPIO);
		SIUL_ClearPad(VNQ7050_SEL2_GPIO);
		break;
	case 1:
		SIUL_SetPad(VNQ7050_SEL0_GPIO);
		SIUL_ClearPad(VNQ7050_SEL1_GPIO);
		SIUL_ClearPad(VNQ7050_SEL2_GPIO);
		break;
	case 2:
		SIUL_ClearPad(VNQ7050_SEL0_GPIO);
		SIUL_SetPad(VNQ7050_SEL1_GPIO);
		SIUL_ClearPad(VNQ7050_SEL2_GPIO);
		break;
	case 3:
		SIUL_SetPad(VNQ7050_SEL0_GPIO);
		SIUL_SetPad(VNQ7050_SEL1_GPIO);
		SIUL_ClearPad(VNQ7050_SEL2_GPIO);
		break;
	case 4:
		SIUL_ClearPad(VNQ7050_SEL0_GPIO);
		SIUL_ClearPad(VNQ7050_SEL1_GPIO);
		SIUL_SetPad(VNQ7050_SEL2_GPIO);
		break;
	case 5:
		SIUL_SetPad(VNQ7050_SEL0_GPIO);
		SIUL_ClearPad(VNQ7050_SEL1_GPIO);
		SIUL_SetPad(VNQ7050_SEL2_GPIO);
		break;
	case 6:
		SIUL_ClearPad(VNQ7050_SEL0_GPIO);
		SIUL_SetPad(VNQ7050_SEL1_GPIO);
		SIUL_SetPad(VNQ7050_SEL2_GPIO);
		break;
	case 7:
		SIUL_SetPad(VNQ7050_SEL0_GPIO);
		SIUL_SetPad(VNQ7050_SEL1_GPIO);
		SIUL_SetPad(VNQ7050_SEL2_GPIO);
		break;
	default:
		break;
	}
}

void vnq7050_k1_ctrl(uint8_t ctrl){

	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K1_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K1_GPIO);
	}
}

void vnq7050_k2_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K2_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K2_GPIO);
	}
}

void vnq7050_k3_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K3_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K3_GPIO);
	}
}

void vnq7050_k4_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K4_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K4_GPIO);
	}
}

void vnq7050_k5_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K5_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K5_GPIO);
	}
}

void vnq7050_k6_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K6_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K6_GPIO);
	}
}

void vnq7050_k7_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K7_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K7_GPIO);
	}
}

void vnq7050_k8_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K8_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K8_GPIO);
	}
}

void vnq7050_k9_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_K9_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_K9_GPIO);
	}
}

void vnq7050_ka_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_KA_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_KA_GPIO);
	}
}

void vnq7050_kb_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_KB_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_KB_GPIO);
	}
}

void vnq7050_kc_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(VNQ7050_KC_GPIO);
	}else{
		SIUL_ClearPad(VNQ7050_KC_GPIO);
	}
}

void m25pe40_wp_ctrl(uint8_t ctrl){
	if(ctrl == 1){
		SIUL_SetPad(M25PE40_WP_GPIO);
	}else{
		SIUL_ClearPad(M25PE40_WP_GPIO);
	}
}


