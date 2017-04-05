/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê3ÔÂ16ÈÕ
* Compiler   : gcc
* FILE NAME  : M25PE40_interface.h
*
* DESCRIPTION:
*
*/
#ifndef INTERFACE_M25PE40_INTERFACE_H_
#define INTERFACE_M25PE40_INTERFACE_H_

#include "derivative.h" /* include peripheral declarations */

#define M25PE40_PAGE_ERASE_CMD			1
#define M25PE40_SUBSECTOR_ERASE_CMD		2
#define M25PE40_SECTOR_ERASE_CMD		3

#define M25PE40_PAGE_NUM			16
#define M25PE40_SUBSECTOR_NUM		16
#define M25PE40_SECTOR_NUM			8

#define M25PE40_PAGE_SIZE		256
#define M25PE40_SUBSECTOR_SIZE	(M25PE40_PAGE_NUM*M25PE40_PAGE_SIZE)
#define M25PE40_SECTOR_SIZE		(M25PE40_SUBSECTOR_NUM*M25PE40_SUBSECTOR_SIZE)

void m25pe40_init(void);
int32_t m25pe40_read(uint32_t addr, uint8_t *dat_buff, uint32_t rx_len);
int32_t m25pe40_write(uint32_t addr, uint8_t * dat_buff, uint32_t tx_len);
int32_t m25pe40_erase(uint8_t erase_cmd, uint16_t num);

#endif /* INTERFACE_M25PE40_INTERFACE_H_ */
