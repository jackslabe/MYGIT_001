/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê2ÔÂ13ÈÕ
* Compiler   : gcc
* FILE NAME  : EEPROM_interface.h
*
* DESCRIPTION:
*
*/
#ifndef EEPROM_INTERFACE_H_
#define EEPROM_INTERFACE_H_

#define EEPROM_OK					0
#define EEPROM_ERR_INIT				1	/* call eeprom_init function to to synchronize EEPROM system */
#define EEPROM_ERR_BUSY				2	/* Waiting for the completion of the operation */
#define EEPROM_ERR_CONFIG			3	/* Check the EPROM/BLOCK configurations and hardware status */
#define EEPROM_ERR_NO_RECORD		4	/* No id record in EEPROM */
#define EEPROM_ERR_NO_ENOUGH_SPACE	5	/* No enough memory for init swap or write */
#define EEPROM_ERR_MISMATCH			6	/* It indicates that there is at least one double word is not same with source data.*/
#define EEPROM_ERR_BLOCK			7	/* Check the EPROM/BLOCK configurations and hardware status */
										/* Check block address spaces in the configuration structures*/
uint32_t eeprom_init(void);
uint32_t eeprom_read(uint16_t data_id, uint16_t data_size, uint32_t *p_buff);
uint32_t eeprom_write(uint16_t data_id, uint16_t data_size, uint32_t *p_buff);
uint32_t eeprom_delete_record(uint16_t data_id);
uint32_t eeprom_erase(void);


#endif /* EEPROM_INTERFACE_H_ */
