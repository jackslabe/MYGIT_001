/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ3ÈÕ
* Compiler   : gcc
* FILE NAME  : MPC33972_interface.h
*
* DESCRIPTION:
*
*/
#ifndef MPC33972_INTERFACE_H_
#define MPC33972_INTERFACE_H_

struct mpc33972_sp_ch{
	uint8_t sp0:1;
	uint8_t sp1:1;
	uint8_t sp2:1;
	uint8_t sp3:1;
	uint8_t sp4:1;
	uint8_t sp5:1;
	uint8_t sp6:1;
	uint8_t sp7:1;
};

void mpc33972_init(void );
void mpc33972_set_sp(uint8_t sp_num, uint8_t power_gnd);
void mpc33972_set_amux(uint8_t channel);
void mpc33972_reset(void);

struct mpc33972_sp_ch mpc33972_get_sp_input_status(void);
uint32_t  mpc33972_get_IO_status(void);


#endif /* MPC33972_INTERFACE_H_ */
