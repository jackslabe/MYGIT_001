/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê3ÔÂ31ÈÕ
* Compiler   : gcc
* FILE NAME  : EXTEND_FLASH_interface.h
*
* DESCRIPTION:
*
*/
#ifndef INTERFACE_EXTEND_FLASH_INTERFACE_H_
#define INTERFACE_EXTEND_FLASH_INTERFACE_H_


uint32_t ext_flash_init(void);
uint32_t ext_flash_write(uint16_t id, uint8_t *p_dat, uint16_t dat_len);
uint32_t ext_flash_read(uint16_t id, uint8_t *p_dat, uint16_t dat_len);


#endif /* INTERFACE_EXTEND_FLASH_INTERFACE_H_ */
