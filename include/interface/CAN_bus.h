/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê1ÔÂ11ÈÕ
* Compiler   : gcc
* FILE NAME  : CAN_bus_drv.h
*
* DESCRIPTION:
*
*/
#ifndef CAN_BUS_H_
#define CAN_BUS_H_

#include "derivative.h"
#include "dlist.h"

#define CAN_MSG_BUFF_SIZE	64
#define CAN_NODE_FLAG_NUM	(CAN_MSG_BUFF_SIZE/8)
#define CAN_BUS_NUM		3

#define CAN_NODE_GRP_FULL	0xFF

struct CanMSG{
	uint32_t id;			/* Identify */
	uint8_t data[8];		/* data */
	int8_t dat_len;		/* data length */
};

struct CanBuffer{
	struct CanMSG msg;
	uint32_t buf_num;
};

struct CanFifo{
	DlistNode can_fifo_node[CAN_MSG_BUFF_SIZE];
	DlistNode can_fifo_head;
	DlistNode can_fifo_tail;
	int32_t node_cnt;
	uint8_t used_node_flag[CAN_NODE_FLAG_NUM];
};


extern struct CanFifo can_tx_fifo[CAN_BUS_NUM];
extern struct CanFifo can_rx_fifo[CAN_BUS_NUM];

int32_t can_fifo_in(struct CanMSG *p_msg, struct CanFifo *p_fifo);
int32_t can_fifo_out(struct CanMSG *p_msg, struct CanFifo *p_fifo);

void can_bus_init(uint32_t bus_num);
int32_t can_bus_read(uint32_t bus_num, struct CanMSG *msg);
int32_t can_bus_write(uint32_t bus_num, struct CanMSG *msg);
uint32_t can_bus_status(uint32_t bus_num);
void can_bus_recover(uint32_t bus_num);

#endif /* CAN_BUS_H_ */
