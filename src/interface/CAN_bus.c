/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : CAN_bus.c
*
* DESCRIPTION:
*
*/

#include "CAN_bus.h"
#include "CAN_drv.h"


static struct CanBuffer tx_buf[CAN_BUS_NUM][CAN_MSG_BUFF_SIZE];
static struct CanBuffer rx_buf[CAN_BUS_NUM][CAN_MSG_BUFF_SIZE];

struct CanFifo can_tx_fifo[CAN_BUS_NUM];
struct CanFifo can_rx_fifo[CAN_BUS_NUM];

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
static void can_fifo_init(struct CanFifo *p_fifo, struct CanBuffer *buffer){
	int i = 0;

	for(i = 0; i < CAN_MSG_BUFF_SIZE; i ++, buffer ++){
		p_fifo->can_fifo_node[i].data = (void *)buffer;
		buffer->buf_num = i;
	}

	p_fifo->can_fifo_head.next = &p_fifo->can_fifo_tail;
	p_fifo->can_fifo_tail.prev = &p_fifo->can_fifo_head;

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
int32_t can_fifo_in(struct CanMSG *p_msg, struct CanFifo *p_fifo){
	int32_t ret = 0;
	uint32_t x = 0;	/* 0 <= x < CAN_MSG_BUFF_SIZE/8 */
	uint8_t y = 0;	/* 0 < y < 8 */
	int32_t node_cnt = p_fifo->node_cnt;
	int32_t free_node = 0;
	uint8_t node_grp = 0;

	struct CanBuffer *p_buf = 0;

	if(node_cnt >= CAN_MSG_BUFF_SIZE){
		/* buffer full */
		ret= -1;
	}else{

		DI();
		for(x = 0; x < CAN_NODE_FLAG_NUM; x ++){
			node_grp = p_fifo->used_node_flag[x];
			if(node_grp != CAN_NODE_GRP_FULL){
				for(y = 0; y < 8; y ++){
					if((node_grp & 0x01) == 0){
						/* find node which is not used */
						free_node = x * 8 + y;

						p_buf = (struct CanBuffer *)p_fifo->can_fifo_node[free_node].data;
						if(p_buf != 0){
							p_buf->msg = *p_msg;

							dlist_insert_tail(&p_fifo->can_fifo_node[free_node], &p_fifo->can_fifo_tail);

							p_fifo->used_node_flag[x] |= (1 << y);	/* set flag */
							node_cnt += 1;

							p_fifo->node_cnt = node_cnt;

						}else{
							ret = -2;
						}
						/* end find x group */
						x = CAN_NODE_FLAG_NUM;
						break;
					}
					node_grp >>= 1;
				}

			}else{
				/* buffer full */
				ret = -1;
			}
		}

		EI();
	}
	return ret;
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
int32_t can_fifo_out(struct CanMSG *p_msg, struct CanFifo *p_fifo){
	struct CanBuffer *p_buf;
	int32_t ret = 0;
	uint32_t x = 0;	/* 0 <= x < CAN_MSG_BUFF_SIZE/8 */
	uint8_t y = 0;	/* 0 < y < 8 */

	if(p_fifo->can_fifo_head.next != &p_fifo->can_fifo_tail){
		DI();
		p_buf = (struct CanBuffer *)p_fifo->can_fifo_head.next->data;
		if(p_buf != 0){
			*p_msg = p_buf->msg;

			dlist_destroy(p_fifo->can_fifo_head.next);
			x = p_buf->buf_num / 8;
			y = p_buf->buf_num % 8;
			p_fifo->used_node_flag[x] &= ~(1 << y);	/* clear flag */

			p_fifo->node_cnt -= 1;
		}
		EI();
	}else{
		ret = -1;		/* */
		/* buffer is empty */
	}

	return ret;
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
struct CanMSG can_fifo_find(uint32_t id, struct CanFifo *p_fifo){
	struct CanMSG msg = {0,{0,0,0,0,0,0,0,0}};
	struct CanBuffer *p_buf = 0;
	uint32_t x = 0;	/* 0 <= x < CAN_MSG_BUFF_SIZE/8 */
	uint8_t y = 0;	/* 0 < y < 8 */

	DlistNode *node = p_fifo->can_fifo_head.next;

	DI();
	while( node != &p_fifo->can_fifo_tail){
		p_buf = (struct CanBuffer *)node->data;
		if(p_buf->msg.id == id){
			/* find the message id */
			msg = p_buf->msg;
			dlist_destroy(node);

			x = p_buf->buf_num / 8;
			y = p_buf->buf_num % 8;
			p_fifo->used_node_flag[x] &= ~(1 << y);	/* clear flag */

			p_fifo->node_cnt -= 1;
			break;
		}else{
			node = node->next;	/* keep finding */
		}
	}
	EI();

	return msg;
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
static void can_bus_fifo_init(uint8_t bus_fifo_num){
	can_fifo_init(&can_tx_fifo[bus_fifo_num], &tx_buf[bus_fifo_num][0]);
	can_fifo_init(&can_rx_fifo[bus_fifo_num], &rx_buf[bus_fifo_num][0]);
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
int32_t can_bus_read(uint32_t bus_num, struct CanMSG *msg){
	int32_t ret = 0;
	struct CanMSG temp_msg;

	DI();
	temp_msg = can_fifo_find(msg->id, &can_rx_fifo[bus_num]);
	if(temp_msg.id == 0){
		/* no message */
		ret = -1;
	}else{
		*msg = temp_msg;
	}
	EI();

	return ret;
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
int32_t can_bus_write(uint32_t bus_num, struct CanMSG *msg){

	int32_t ret = 0;
	ret = can_fifo_in(msg, &can_tx_fifo[bus_num]);

	mpc5744_can_send_start(bus_num);

	return ret;
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
uint32_t can_bus_status(uint32_t bus_num){

	uint32_t ret = 0;
	ret = mpc5744_can_get_status(bus_num);

	return ret;
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
void can_bus_recover(uint32_t bus_num){
	mpc5744_can_recover(bus_num);
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
void can_bus_init(uint32_t bus_num){

	can_bus_fifo_init(bus_num);

	mpc5744_can_init(bus_num);
}

