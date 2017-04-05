/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê1ÔÂ6ÈÕ
* Compiler   : gcc
* FILE NAME  : error_define.h
*
* DESCRIPTION:
*
*/
#ifndef ERROR_DEFINE_H_
#define ERROR_DEFINE_H_


typedef enum _SYS_STATUS{
	SYS_OK,
	SYS_ARGERROR,
	SYS_TIMEOUT,
	COMM_BUSY,
	COMM_TIMEOUT,
	COMM_NONE_MSG,
	COMM_BUF_OVERFLOW,
	COMM_PARITY_ERR,
	COMM_NACK,
	COMM_TX_COMPLETE,
	COMM_RX_NEW_MSG,
}SYS_STATUS;

/* check error */

#define ARGERROR		/* argument inpout error */
#endif /* ERROR_DEFINE_H_ */
