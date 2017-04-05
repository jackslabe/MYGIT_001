/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ13ÈÕ
* Compiler   : gcc
* FILE NAME  : SPI_bus_drv.h
*
* DESCRIPTION:
*
*/
#ifndef SPI_BUS_DRV_H_
#define SPI_BUS_DRV_H_

#include "derivative.h"

#define DEV_NAME_LEN	16
struct spi_device {
	struct spi_master	*master;
	char				devname[DEV_NAME_LEN];
	uint32_t			max_speed_hz;
	uint32_t			actual_speed_hz;
	uint8_t				chip_select;
	uint8_t				cfg_select;		/* used for configuration select */
	uint32_t 		dev_CPHA:1;			/* 1: data captured on the rising edge. 0: captured on the following edge.*/
	uint32_t		dev_CPOL:1;			/* 1: inactive state value of SCK is high. 0: inactive state value of SCK is low.*/
	uint32_t		dev_LSB_first:1;	/* 1: lsb first, 0: msb first */
	uint32_t 		dev_single:1;		/* 1: single device, 0: not single */

	uint8_t			bits_per_word;
	int32_t			irq;

	/*    ____                                            __________          */
	/* CS     |__________________________________________|          |________ */
	/*              ____      ____         ____                           ___ */
	/* SCK ________|    |____|    |_-----_|    |_________________________|    */
	/*        |-t1-|                           |<-- t2 ->|<--- t3 ->|         */

	uint32_t 		start_delay_nsecs;	    /* t1: when pull donw CS, delay for data transmit */
	uint32_t		end_delay_nsecs;	    /* t2: at the end of transmit, delay to pull up CS */
	uint32_t		next_frame_delay_nsecs;	/* t3: next frame delay */

	void			*controller_state;
	void			*controller_data;

};

struct spi_master {

	/* other than negative (== assign one dynamically), bus_num is fully
	 * board-specific.  usually that simplifies to being SOC-specific.
	 * example:  one SOC has three SPI controllers, numbered 0..2,
	 * and one board's schematics might show it using SPI-2.  software
	 * would normally use bus_num=2 for that controller.
	 */
	int16_t			bus_num;

	/* chipselects will be integral to many controllers; some others
	 * might use board-specific GPIOs.
	 */
	uint16_t		num_chipselect;

	/* some SPI controllers pose alignment requirements on DMAable
	 * buffers; let protocol drivers know about these requirements.
	 */
	uint16_t		dma_alignment;

	/* spi_device.mode flags understood by this controller driver */
	uint16_t		mode_bits;

	/* other constraints relevant to this driver */
	uint16_t		flags;
#define SPI_MASTER_HALF_DUPLEX	BIT(0)		/* can't do full duplex */
#define SPI_MASTER_NO_RX	BIT(1)		/* can't do buffer read */
#define SPI_MASTER_NO_TX	BIT(2)		/* can't do buffer write */

	/* flag indicating that the SPI bus is locked for exclusive use */
	uint8_t			bus_busy_flag;		/* 0: free, 1: busy, 2: read option, 4: write option */
#define SPI_BUS_FREE	0
#define SPI_BUS_BUSY	1
#define SPI_BUS_READ	2
#define SPI_BUS_WRITE	4

	int32_t			(*setup)(struct spi_device *spi);
};

struct spi_transfer {
	uint8_t 	spi_bus_num;
	const void	*tx_buf;
	void		*rx_buf;
	int16_t		tx_len;
	int16_t		rx_len;
	int16_t 	rx_new_dat_flag;	/* 1: receive new data, 0: no data receive */
	int16_t		tx_end_flag;		/* 1: transimt end, 0: transimt on going */
	int32_t 	tx_timeout;			/* transfer timeout counter */
	int32_t 	rx_timeout;			/* transfer timeout counter */
	int16_t 	transfer_mode;		/* 0: no mode, 1: read mode, 2: write mode, 4: write the read mode */
#define TRANSFER_READ_MODE	 		1
#define TRANSFER_WRITE_MODE	 		2
#define TRANSFER_WRITE_READ_MODE	4
#define TRANSFER_WRITE_CONTINUOUS	8

	uint32_t	tx_dma_addr;
	uint32_t	rx_dma_addr;

	int16_t		cs_change:1;
	int16_t 	cs_mode:1;		/* 0: single mode, 1: Continuous mode, */
#define CS_SINGLE_MODE 	0		/* Return PCSn signals to their inactive state between transfers. */
#define CS_CONT_MODE	1		/* Keep PCSn signals asserted between transfers */
	int16_t		cs_num;			/* same as the device chip_select */
	uint16_t 	cfg_select;		/* Clock and Transfer Attributes Select */
	uint8_t		bits_per_word;	/* */
	uint16_t	delay_usecs;		/* When CS pull down, need some delay time to transfer data */

	/* completion is reported through a callback */
	void		(*complete)(void *context);
	void		*context;
};


/* spi configuration */
#define SPI_RSER_TCF_MASK		0x80000000
#define SPI_RSER_EOQF_MASK		0x10000000
#define SPI_RSER_TFUF_MASK		0x08000000
#define SPI_RSER_TFFF_MASK		0x02000000
#define SPI_RSER_TFFF_DIRS_MASK	0x01000000
#define SPI_RSER_RFOF_MASK		0x00080000
#define SPI_RSER_RFDF_MASK		0x00020000
#define SPI_RSER_RFDF_DIRS_MASK	0x00010000
#define SPI_INT_MASK	(SPI_RSER_TCF_MASK|SPI_RSER_RFDF_MASK)


int32_t spi_bus_read(struct spi_device *dev, struct spi_transfer *dat);
int32_t spi_bus_write(struct spi_device *dev, struct spi_transfer *dat);
void spi_bus_init(void);

#endif /* SPI_BUS_DRV_H_ */


