/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê3ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : CRC32_checksum.h
*
* DESCRIPTION:
*
*/
#ifndef INTERFACE_CRC32_CHECKSUM_H_
#define INTERFACE_CRC32_CHECKSUM_H_

#define CRC_CHECK_CONTINUE		1
#define CRC_CHECK_END			2


void init_table(void);
uint32_t compute_crc32( uint8_t *buf, uint32_t len);
uint32_t compute_crc32_step(uint8_t *buf, uint32_t len, uint8_t step);

#endif /* INTERFACE_CRC32_CHECKSUM_H_ */
