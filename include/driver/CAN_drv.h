/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ25ÈÕ
* Compiler   : gcc
* FILE NAME  : CAN_drv.h
*
* DESCRIPTION:
*
*/
#ifndef CAN_DRV_H_
#define CAN_DRV_H_

struct MPC5744CanFrame{
	uint8_t IDE:1;		/* 0: standard frame, 1: extended frame */
	uint8_t RTR:1;		/* remote transmission request */
	uint8_t DLC:4;		/* data length */
	uint8_t PRIO:3;		/* priority */
	uint32_t id;			/* Identify */
	uint32_t data1;		/* data */
	uint32_t data2;

};


void mpc5744_can_init(int32_t nbModule);
int32_t mpc5744_can_write(uint8_t bus_num, uint8_t mail_box_num, struct MPC5744CanFrame *frame);
void mpc5744_can_send_start(uint16_t bus_num);
uint32_t mpc5744_can_get_status(uint16_t bus_num);
void mpc5744_can_recover(uint16_t bus_num);

#endif /* CAN_DRV_H_ */
