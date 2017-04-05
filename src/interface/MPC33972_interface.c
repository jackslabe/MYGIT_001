/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ17ÈÕ
* Compiler   : gcc
* FILE NAME  : MPC33972_interface.c
*
* DESCRIPTION:
*
*/

#include "SPI_bus_drv.h"
#include "MPC33972_interface.h"

extern struct spi_master bus2;

static struct spi_device mpc33972_spi;
static struct spi_transfer mpc33972_transfer;

static uint8_t mpc33972_txbuf[4];
static uint8_t mpc33972_rxbuf[4];

static uint32_t mpc33972_reg[12];

static void mpc33972_complete(void * context){

}

enum MPC33972_CMD{
	READ_SO_CMD 			= 0,
	SET_BAT_GND_CMD 		= 1,
	SP_WAKEUP_EN_CMD 		= 2,
	SG_WAKEUP_EN_CMD 		= 3,
	SET_SP_CONTACT_CMD 		= 4,
	SET_SG_CONTACT_CMD 		= 5,
	SET_AMUX_CMD			= 6,
	SET_SP_CURRENT_TIME_CMD = 7,
	SET_SG_CURRENT_TIME_CMD = 8,
	SET_SP_TRI_STATE_CMD	= 9,
	SET_SG_TRI_STATE_CMD	= 10,
	CAL_TIMER_CMD			= 11,
	SET_SLEEP_CMD			= 12,
	RESET_CMD				= 0x7F
};

enum MPC33972_REG{
	SO_REG 					= 0,
	SP_BAT_GND_REG 			= 1,
	SP_WAKEUP_EN_REG 		= 2,
	SG_WAKEUP_EN_REG 		= 3,
	SP_CONTACT_REG 			= 4,
	SG_CONTACT_REG 			= 5,
	AMUX_REG				= 6,
	SP_CURRENT_TIME_REG 	= 7,
	SG_CURRENT_TIME_REG 	= 8,
	SP_TRI_STATE_REG		= 9,
	SG_TRI_STATE_REG		= 10,
	CAL_TIMER_REG			= 11,
};

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
static void mpc33972_setup(uint8_t cmd, uint16_t data){
	int32_t ret = 0;
	mpc33972_txbuf[0] = cmd;
	mpc33972_txbuf[1] = (uint8_t)(data >> 8);
	mpc33972_txbuf[2] = (uint8_t)data;

	if(cmd < SET_SLEEP_CMD){
		mpc33972_reg[cmd] = data;
	}
	mpc33972_transfer.transfer_mode = TRANSFER_WRITE_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/

	ret = spi_bus_write(&mpc33972_spi, &mpc33972_transfer);

	if(ret == 0){
		while(mpc33972_transfer.tx_end_flag != 1){
			;/* wait for transmit complete */
		}
		mpc33972_transfer.tx_end_flag = 0;
	}

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
void mpc33972_init(void ){

	char dev_name[DEV_NAME_LEN] = "MPC33972";
	uint16_t data = 0;
	int i = 0;
	/* configure mpc33908 device */
	mpc33972_spi.max_speed_hz = 6000000;		/* Max SPI operation frequency 8Mhz*/
	mpc33972_spi.cfg_select = 2;				/* Select Clock and Transfer Attributes Register*/
	mpc33972_spi.chip_select = 2;				/* chip select 0 */
	mpc33972_spi.bits_per_word = 8;			/* 16 bit perword */
	mpc33972_spi.dev_CPHA = 1;					/* data captured on the following edge */
	mpc33972_spi.dev_CPOL = 0;					/* inactive state value of SCK is low */
	mpc33972_spi.dev_single = 0;				/* multi device on bus2 */
	mpc33972_spi.dev_LSB_first = 0;				/* msb first */
	mpc33972_spi.actual_speed_hz = 2000000;		/* 2Mhz */
	mpc33972_spi.master = &bus2;
	mpc33972_spi.start_delay_nsecs = 500;			/* delay 500 ns */
	mpc33972_spi.end_delay_nsecs = 500;
	mpc33972_spi.next_frame_delay_nsecs = 500;
	/* SPI bus setup */
	mpc33972_spi.master->setup(&mpc33972_spi);

	/* transfer option configure */
	mpc33972_transfer.spi_bus_num = 2;
	mpc33972_transfer.bits_per_word = 8;
	mpc33972_transfer.cfg_select = 2;
	mpc33972_transfer.cs_num = 2;
	mpc33972_transfer.rx_buf = (void *)mpc33972_rxbuf;
	mpc33972_transfer.tx_buf = (void *)mpc33972_txbuf;
	mpc33972_transfer.rx_len = 3;
	mpc33972_transfer.tx_len = 3;
	mpc33972_transfer.complete = mpc33972_complete;
	mpc33972_transfer.context = (void *)&mpc33972_spi;
	mpc33972_transfer.transfer_mode = TRANSFER_WRITE_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/
	mpc33972_transfer.cs_mode = CS_CONT_MODE;	/* CS_SINGLE_MODE CS_CONT_MODE*/

	for(i = 0; i < DEV_NAME_LEN; i ++){
		mpc33972_spi.devname[i] = dev_name[i];
	}

	/* init mpc33972 register */
	mpc33972_setup(RESET_CMD, data);
	/* 1. setup sp switch */
	data = 0x0000;	/* SP4~7 switch to power, SP0~3 switch to gnd */
	mpc33972_setup(SET_BAT_GND_CMD, data);
	/* 2. setup wake-up/interrupt */
	data = 0x0000;	/* non wake up */
	mpc33972_setup(SP_WAKEUP_EN_CMD, data);
	data = 0x0000;	/* non wake up */
	mpc33972_setup(SG_WAKEUP_EN_CMD, data);
	/* 3. setup contact current, 2mA or 16mA */
	data = 0x0000;	/* comtact current is 2mA */
	mpc33972_setup(SP_CONTACT_REG, data);
	data = 0x0000;	/* comtact current is 2mA */
	mpc33972_setup(SG_CONTACT_REG, data);
	/* 4. setup contact current timer */
	data = 0x0000;	/* timer off */
	mpc33972_setup(SET_SP_CURRENT_TIME_CMD, data);
	data = 0x0000;	/* timer off */
	mpc33972_setup(SET_SG_CURRENT_TIME_CMD, data);
	/* 5. setup IO to tri-status */
	data = 0x0000;	/* enable all input */
	mpc33972_setup(SET_SP_TRI_STATE_CMD, data);
	data = 0xFFFF;	/* enable all input */
	mpc33972_setup(SET_SG_TRI_STATE_CMD, data);
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
void mpc33972_set_sp(uint8_t sp_num, uint8_t power_gnd){
	uint16_t data = mpc33972_reg[SP_BAT_GND_REG];
	uint8_t sp_mask = 0;

	power_gnd &= 0x01;				/* clear bit */

	if(sp_num > 8){
		/* parameter error */
	}else{
		/* clear sp bit */
		sp_mask = 1 << sp_num;
		sp_mask = ~sp_mask;
		data &= sp_mask;
		/* set sp channel */
		data |= power_gnd << sp_num;
	}

	mpc33972_setup(SET_BAT_GND_CMD, data);
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
void mpc33972_set_tristate(uint16_t sg_mask, uint16_t sp_mask){
	uint16_t sg_data = mpc33972_reg[SG_TRI_STATE_REG];
	uint16_t sp_data = mpc33972_reg[SP_TRI_STATE_REG];
	int8_t i = 0;
	uint8_t temp = 0;

	for(i = 0; i < 14; i ++){
		temp = (sg_mask >> i) & 0x01;
		if(temp == 1){
			sg_data |= 1 << i;	/* set sp channel power or gnd */
		}
	}

	for(i = 0; i < 8; i ++){
		temp = (sg_mask >> i) & 0x01;
		if(temp == 1){
			sg_data |= 1 << i;	/* set sp channel power or gnd */
		}
	}

	/* 5. setup IO to tri-status */
	mpc33972_setup(SET_SP_TRI_STATE_CMD, sg_data);
	mpc33972_setup(SET_SG_TRI_STATE_CMD, sp_data);
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
struct mpc33972_sp_ch mpc33972_get_sp_input_status(void){
	struct mpc33972_sp_ch sp_status;
	int32_t ret = 0;

	mpc33972_txbuf[0] = READ_SO_CMD;
	mpc33972_txbuf[1] = 0xFF;
	mpc33972_txbuf[2] = 0xFF;

	mpc33972_transfer.transfer_mode = TRANSFER_READ_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/

	ret = spi_bus_read(&mpc33972_spi, &mpc33972_transfer);

	if(ret == 0){
		while(mpc33972_transfer.rx_new_dat_flag == 0){
			/* wait for new data */
			;
		}
		mpc33972_transfer.rx_new_dat_flag = 0;
		sp_status.sp0 = (mpc33972_rxbuf[1]&0x40) >> 6;
		sp_status.sp1 = (mpc33972_rxbuf[1]&0x80) >> 7;
		sp_status.sp2 = (mpc33972_rxbuf[0]&0x01);
		sp_status.sp3 = (mpc33972_rxbuf[0]&0x02) >> 1;
		sp_status.sp4 = (mpc33972_rxbuf[0]&0x04) >> 2;
		sp_status.sp5 = (mpc33972_rxbuf[0]&0x08) >> 3;
		sp_status.sp6 = (mpc33972_rxbuf[0]&0x10) >> 4;
		sp_status.sp7 = (mpc33972_rxbuf[0]&0x20) >> 5;
	}else{
		sp_status.sp0 = 0;
		sp_status.sp1 = 0;
		sp_status.sp2 = 0;
		sp_status.sp3 = 0;
		sp_status.sp4 = 0;
		sp_status.sp5 = 0;
		sp_status.sp6 = 0;
		sp_status.sp7 = 0;
	}

	return sp_status;
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
uint32_t  mpc33972_get_IO_status(void){
	uint32_t ret = 0;

	mpc33972_txbuf[0] = READ_SO_CMD;
	mpc33972_txbuf[1] = 0xFF;
	mpc33972_txbuf[2] = 0xFF;

	mpc33972_transfer.transfer_mode = TRANSFER_READ_MODE;	/* TRANSFER_WRITE_MODE, TRANSFER_READ_MODE*/

	ret = spi_bus_read(&mpc33972_spi, &mpc33972_transfer);

	if(ret == 0){
		while(mpc33972_transfer.rx_new_dat_flag == 0){
			/* wait for new data */
			;
		}
		mpc33972_transfer.rx_new_dat_flag = 0;
		ret = (mpc33972_rxbuf[0] << 16) | (mpc33972_rxbuf[1] << 8) | mpc33972_rxbuf[2];
		mpc33972_reg[SO_REG] = ret;
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
void mpc33972_set_amux(uint8_t channel){
	uint16_t data = 0x001F & channel;
	mpc33972_setup(SET_AMUX_CMD, data);
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
void mpc33972_reset(void){
	uint16_t data = 0xFFFF;
	mpc33972_setup(RESET_CMD, data);
}

#if 0
void mpc33972_timer_calibration(void){
	uint16_t data = 0xFFFF;
	mpc33972_setup(CAL_TIMER_CMD, data);
}
#endif




