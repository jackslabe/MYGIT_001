/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ13ÈÕ
* Compiler   : gcc
* FILE NAME  : SPI_bus_drv.c
*
* DESCRIPTION:
*
*/

#include "SPI_bus_drv.h"
#include "SPI_drv.h"

/*struct spi_master bus[MPC5744_SPI_NUM];*/
struct spi_master bus0;
struct spi_master bus1;
struct spi_master bus2;
struct spi_master bus3;

/*********************************************************************************/
/* Description:
 *
 *
 * Input:
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
int32_t spi_bus_read(struct spi_device *dev, struct spi_transfer *dat){
	int32_t ret = 0;
	/* set bus busy */
	if(dev->master->bus_busy_flag == SPI_BUS_FREE){
		DI();
		dev->master->bus_busy_flag = SPI_BUS_BUSY;
		dev->master->bus_busy_flag |= SPI_BUS_READ;
		EI();

		ret = mpc5744_spi_tx(dev, dat);
	}else{
		/* bus busy */
		ret = -1;
	}
	return ret;
}

/*********************************************************************************/
/* Description:
 *
 *
 * Input:
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
int32_t spi_bus_write(struct spi_device *dev, struct spi_transfer *dat){

	int32_t ret = 0;
	if(dev->master->bus_busy_flag == SPI_BUS_FREE){
		dev->master->bus_busy_flag = SPI_BUS_BUSY;
		if(dat->transfer_mode == TRANSFER_WRITE_READ_MODE){
			dev->master->bus_busy_flag |= (SPI_BUS_WRITE | SPI_BUS_READ);
		}else
		{
			dev->master->bus_busy_flag |= SPI_BUS_WRITE;
		}
		ret = mpc5744_spi_tx(dev, dat);
	}else{
		/* bus busy */
		ret = -1;
	}
	return ret;
}

/*********************************************************************************/
/* Description:
 *
 *
 * Input:
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
void spi_bus_init(void){

	mpc5744_spi_init(0, 1, SPI_INT_MASK);
	mpc5744_spi_init(1, 1, SPI_INT_MASK);
	mpc5744_spi_init(2, 1, SPI_INT_MASK);

	mpc5744_spi_irq_init(0);
	mpc5744_spi_irq_init(1);
	mpc5744_spi_irq_init(2);

	bus0.bus_num = 0;
	bus0.num_chipselect = 1;
	bus0.setup = mpc5744_spi_setup;

	bus1.bus_num = 1;
	bus1.num_chipselect = 1;
	bus1.setup = mpc5744_spi_setup;

	bus2.bus_num = 2;
	bus2.num_chipselect = 2;
	bus2.setup = mpc5744_spi_setup;

}

/*********************************************************************************/
/* Description:
 *
 *
 * Input:
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
void spi_bus_abort(void){

}


