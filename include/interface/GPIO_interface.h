/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ10ÈÕ
* Compiler   : gcc
* FILE NAME  : GPIO_interface.h
*
* DESCRIPTION:
*
*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

void gpio_output_init(void);
void gpio_input_init(void);
uint32_t mpc33908_fs0b_input(void);
uint32_t mpc33908_intb_input(void);
uint32_t mpc33664_intb_input(void);
void led1_ctrl(uint8_t ctrl);
void led2_ctrl(uint8_t ctrl);
void fs_ctrl(uint8_t ctrl);
void s2_ctrl(uint8_t ctrl);
void vnq7050_sel_ctrl(uint8_t sel_num);
void vnq7050_k1_ctrl(uint8_t ctrl);
void vnq7050_k2_ctrl(uint8_t ctrl);
void vnq7050_k3_ctrl(uint8_t ctrl);
void vnq7050_k4_ctrl(uint8_t ctrl);
void vnq7050_k5_ctrl(uint8_t ctrl);
void vnq7050_k6_ctrl(uint8_t ctrl);
void vnq7050_k7_ctrl(uint8_t ctrl);
void vnq7050_k8_ctrl(uint8_t ctrl);
void vnq7050_k9_ctrl(uint8_t ctrl);
void vnq7050_ka_ctrl(uint8_t ctrl);
void vnq7050_kb_ctrl(uint8_t ctrl);
void vnq7050_kc_ctrl(uint8_t ctrl);
void m25pe40_wp_ctrl(uint8_t ctrl);
void mpc33664_en_cntrl(uint8_t ctrl);

void iso_p_ctrl(uint8_t ctrl);
void iso_n_ctrl(uint8_t ctrl);


#endif /* GPIO_INTERFACE_H_ */
