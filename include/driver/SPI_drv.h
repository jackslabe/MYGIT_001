/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ13ÈÕ
* Compiler   : gcc
* FILE NAME  : SPI_drv.h
*
* DESCRIPTION:
*
*/
#ifndef SPI_DRV_H_
#define SPI_DRV_H_

#include "derivative.h"
#include "SPI_bus_drv.h"

#define MPC5744_SPI_NUM		4

void mpc5744_spi_init(uint8_t DspiNumber,uint8_t mode, uint32_t intMask);
void mpc5744_spi_irq_init(uint8_t spi_num);

int32_t mpc5744_spi_tx(struct spi_device *dev, struct spi_transfer *dat);
int32_t mpc5744_spi_setup(struct spi_device *dev);


#endif /* SPI_DRV_H_ */
