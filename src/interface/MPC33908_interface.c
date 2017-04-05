/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê10ÔÂ13ÈÕ
* Compiler   : gcc
* FILE NAME  : MPC33908_interface.c
*
* DESCRIPTION:
*
*/


#include "SPI_bus_drv.h"
#include "MPC33908_interface.h"

extern struct spi_master bus0;

static struct spi_device mpc33908_spi;
static struct spi_transfer mpc33908_txopt;

enum MPC33908_REG_ADDR{
	INIT_Vreg1 		= 1,
	INIT_Vreg2 		= 2,
	INIT_CAN_LIN 	= 3,
	INIT_IO_WU1 	= 4,
	INIT_IO_WU2 	= 5,
	INIT_INT 		= 6,
	/* RESERVE      = 7, */
	HW_CONFIG		= 8,
	WU_SOURCE		= 9,
	/* RESERVE      = 10, */
	IO_INPUT		= 11,
	STATUS_Vreg1	= 12,
	STATUS_Vreg2	= 13,
	DIAG_Vreg1		= 14,
	DIAG_Vreg2		= 15,
	DIAG_Vreg3		= 16,
	DIAG_CAN1		= 17,
	DIAG_CAN_LIN	= 18,
	DIAG_SPI		= 19,
	/* RESERVE		= 20, */
	MODE			= 21,
	Vreg_MODE		= 22,
	IO_OUT_AMUX		= 23,
	CAN_LIN_MODE	= 24,
	CAN_MODE2		= 25,
	/* RESERVE 		= 26, */
	INIT_SUPER1		= 33,
	INIT_SUPER2		= 34,
	INIT_SUPER3		= 35,
	INIT_FSSM1		= 36,
	INIT_FSSM2		= 37,
	WD_WINDOW		= 38,
	WD_LFSR			= 39,
	WD_ANSWER		= 40,
	FS_OUT			= 41,
	RSTV_REQ		= 42,
	INIT_WD			= 43,
	DIAG_FS1		= 44,
	WD_COUNTER		= 45,
	DIAG_FS2		= 46
};

static uint16_t mpc33908_reg[47];

static uint16_t mpc33908_txbuf[4];
static uint16_t mpc33908_rxbuf[4];

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
static void mpc33908_complete(void * context){
	/* do nothing */
}

/***************************************************************************//*!
*   @brief The function PwSBC_ComputeParity computes a parity for a specified
*			command and returns the whole command.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function computes odd parity for the given command and
*					returns modified command.
* 	@param[in] cmd - 16-bit command.
* 	@return 	Modified 16-bit command including odd parity.
*	@remarks 	If a secured command is being used, then the
*				PwSBC_ComputeSecurityBits function must be applied before
*				computing parity.
*	@par Code sample
*																		</para>
*			uint32_t cmd_with_parity;									</para>
*			cmd_with_parity = PwSBC_ComputeParity(0b1010111000000001);	</para>
*			- This function computes parity for the command (IO_out - AMUX) and
*			returns its modified version in the variable cmd_with_ parity.
********************************************************************************/
static uint16_t mpc33908_compute_p(uint16_t cmd){
	uint16_t sum = 0;
	uint16_t i = 0;
	cmd &= 0xFEFF;			/* clear bit8 */

	for(i = 0;i < 16;i++){
		sum = sum + ((cmd >> i) & 0x0001);
	}
	/* check even or odd */
	if((sum & 0x0001) == 0){			//even number -> P = 1
		cmd = cmd | 0x0100;
	}

	return cmd;
}


/* Secured_3 = NOT(bit5) */
/* Secured_2 = NOT(bit4) */
/* Secured_1 = bit7		 */
/* Secured_0 = bit6		 */
static uint8_t mpc33908_compute_secure(uint8_t data){
	uint8_t temp = data;
	temp |= (data & 0x40) >> 6;					/* secured_0 = bit6 */
	temp |= (data & 0x80) >> 6;					/* secured_1 = bit7 */
	temp |= (~((data & 0x10) >> 2)) & 0x04;		/* secured_2 = ~bit4 */
	temp |= (~((data & 0x20) >> 2)) & 0x08;		/* secured_3 = ~bit5 */

	return temp;
}


/***************************************************************************//*!
*   @brief The function PwSBC_ComputeLFSR computes, stores and returns test based from actual LFSR.
*	@par Include
*
* 	@par Description
*					This function makes control computations with the given LFSR and
*					returns result of the computation on the least 8 bits.
* 	@param[in] actualLFSR - 8-bit LFSR value on which will be applied control
*						computations.
* 	@return 	Result of the control computations. Computations are made as
*				follows:
*				~(((lfsr*4 + 6 - 4)/4
*	@remarks 	Control computations are made in assembler using instructions
*				to prove ALU using basic operations (*, /, +, -).
*	@par Code sample
*												</para>
*			uint32_t result;					</para>
*			result = PwSBC_ComputeLFSR(50);		</para>
*			- Command makes control computations with the given LFSRvalue (50)
*			and returns result.
********************************************************************************/

static uint16_t mpc33908_compute_answer(uint16_t lfsr){
/* Answer = (NOT(((LFSR x 4)+6)-4))/4 */
		register uint16_t answer = 0;
		answer = lfsr*4 + 6;
		answer -= 4;
		answer = ~answer;
		answer /= 4;
		return answer&0xFF;
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
static uint16_t mpc33908_read(uint16_t reg_addr){
	int32_t ret = 0;
	uint16_t reg_num = reg_addr & 0x3F;
	mpc33908_txbuf[0] = (reg_addr << 9) & 0x7FFF;
	mpc33908_txopt.transfer_mode = TRANSFER_READ_MODE;	/* TRANSFER_WRITE_READ_MODE, TRANSFER_READ_MODE*/

	ret = spi_bus_read(&mpc33908_spi, &mpc33908_txopt);

	if(ret == 0){
		while(mpc33908_txopt.rx_new_dat_flag == 0){
			/* wait for new data */
		}
		mpc33908_txopt.rx_new_dat_flag = 0;
		mpc33908_reg[reg_num] = mpc33908_rxbuf[0];
	}else{
		mpc33908_rxbuf[0] = 0xFFFF;
	}

	return mpc33908_rxbuf[0];
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
static uint16_t mpc33908_write(uint16_t reg_addr, uint8_t tx_data){
	uint16_t reg_num = reg_addr & 0x3F;
	int32_t ret  = 0;

	mpc33908_txbuf[0] = (reg_addr << 9) | 0x8000;	/* set bit 15 and bit 9~14 */
	mpc33908_txbuf[0] &= 0xFF00;	/* clear low byte */
	mpc33908_txbuf[0] |= tx_data;
	mpc33908_txbuf[0] = mpc33908_compute_p(mpc33908_txbuf[0]);
	mpc33908_txopt.transfer_mode = TRANSFER_WRITE_READ_MODE;	/* TRANSFER_WRITE_READ_MODE, TRANSFER_READ_MODE*/

	ret = spi_bus_write(&mpc33908_spi, &mpc33908_txopt);

	if(ret == 0){
		while(mpc33908_txopt.rx_new_dat_flag == 0){
			/* wait for new data */
		}
		mpc33908_txopt.rx_new_dat_flag = 0;
		mpc33908_reg[reg_num] = mpc33908_txbuf[0] & 0x00FF;
	}else{
		mpc33908_rxbuf[0] = 0xFFFF;
	}

	return mpc33908_rxbuf[0];
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
void mpc33908_feed_watchdog(void){
	uint16_t newLFSR = 0;
	uint16_t firstAnswer = 0;
	newLFSR = mpc33908_read(WD_LFSR);					/* read lfsr */
	firstAnswer = mpc33908_compute_answer(newLFSR);		/* computer answer */
	mpc33908_write(WD_ANSWER, (uint8_t)firstAnswer);	/* feed answer */
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

void mpc33908_release_fs_out(void){
	/* READ lfsr */
	/* fs_out reg = lfsr*/
	/* WD_LFSR_7:0 = b7 b6 b5 b4 b3 b2 b1 b0 */
	/* FS_OUT_ 7:0 = ~(b0 b1 b2 b3 b4 b5 b6 b7) */
	/* When fault is removed and reset error counter changes back to level ¡°0¡±, */
	/* a right word must be filled in the FS_OUT register. */
	uint16_t newLFSR = 0;
	uint16_t diag_fs2_dat = 0;
	uint8_t fs_out_dat = 0;
	static  uint8_t rewrite_flag = 0;

	diag_fs2_dat = mpc33908_read(DIAG_FS2);
	if((diag_fs2_dat & 0xE0) == 0)
	{	/* error counter is 0 */
		if(rewrite_flag == 0){
			rewrite_flag = 1;
			newLFSR = mpc33908_read(WD_LFSR);					/* read lfsr */
			newLFSR &= 0x00FF;
			fs_out_dat  = (newLFSR & 0x80) >> 7;				/* fs_bit0 = lfsr_bit7 */
			fs_out_dat |= (newLFSR & 0x40) >> 5;				/* fs_bit1 = lfsr_bit6 */
			fs_out_dat |= (newLFSR & 0x20) >> 3;				/* fs_bit2 = lfsr_bit5 */
			fs_out_dat |= (newLFSR & 0x10) >> 1;				/* fs_bit3 = lfsr_bit4 */
			fs_out_dat |= (newLFSR & 0x08) << 1;				/* fs_bit4 = lfsr_bit3 */
			fs_out_dat |= (newLFSR & 0x04) << 3;				/* fs_bit5 = lfsr_bit2 */
			fs_out_dat |= (newLFSR & 0x02) << 5;				/* fs_bit6 = lfsr_bit1 */
			fs_out_dat |= (newLFSR & 0x01) << 7;				/* fs_bit7 = lfsr_bit0 */
			fs_out_dat = ~fs_out_dat;							/* fs_out = ~ fs_out */

			mpc33908_write(FS_OUT, fs_out_dat);		/* write fs_out register */
		}
	}else{
		/* wait RSTB error counter = 0 */
		rewrite_flag = 0;
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


struct mpc33908_wakeup_source mpc33908_get_wakeup_source(void){
	uint16_t data = 0;
	struct mpc33908_wakeup_source wu_src;
	data = mpc33908_reg[WU_SOURCE];

	wu_src.phy_wu  = data & 0x0001;
	wu_src.io_0_wu = (data & 0x0004) >> 2;
	wu_src.io_1_wu = (data & 0x0008) >> 3;
	wu_src.io_2_wu = (data & 0x0010) >> 4;
	wu_src.io_3_wu = (data & 0x0020) >> 5;
	wu_src.io_4_wu = (data & 0x0040) >> 6;
	wu_src.io_5_wu = (data & 0x0080) >> 7;

	return wu_src;
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

struct mpc33908_io_status mpc33908_get_io_status(void){
	uint16_t data = 0;
	struct mpc33908_io_status io_dat;
	data = mpc33908_read(IO_INPUT);

	io_dat.io_0 = data & 0x0001;
	io_dat.io_1 = (data & 0x0002) >> 1;
	io_dat.io_2 = (data & 0x0008) >> 3;
	io_dat.io_3 = (data & 0x0010) >> 4;
	io_dat.io_4 = (data & 0x0040) >> 6;
	io_dat.io_5 = (data & 0x0080) >> 7;

	return io_dat;
}

/*********************************************************************************/
/* Description:
 *			set low power mode.
 *
 * Input:
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
#define MPC33908_GOTO_LPOFF		0x20

void mpc33908_set_lpoff_mode(void){
	uint16_t data = MPC33908_GOTO_LPOFF;
	mpc33908_write(MODE, mpc33908_compute_secure(data));
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

int32_t mpc33908_diag(struct mpc33908_diag_dat *p_diag){
	struct mpc33908_diag_dat diag_dat;
	int32_t diag_result = 0;
	/* read Vreg1 status */
	/* read Vreg2 status */
	/* read diag Vreg1 */
	/* read diag Vreg2 */
	/* read diag Vreg3 */
	/* read diag CAN1 */
	/* read diag CAN_LIN */
	/* read diag SPI */
	/* read diag FS1 */
	/* read diag FS2 */
	/* read WD_Counter */
	diag_dat.vreg1_status = mpc33908_read(STATUS_Vreg1)&0x00FF;
	diag_dat.vreg2_status = mpc33908_read(STATUS_Vreg2)&0x00FF;
	diag_dat.vreg1_diag = mpc33908_read(DIAG_Vreg1)&0x00FF;
	diag_dat.vreg2_diag = mpc33908_read(DIAG_Vreg2)&0x00FF;
	diag_dat.vreg3_diag = mpc33908_read(DIAG_Vreg3)&0x00FF;
	diag_dat.can_diag = mpc33908_read(DIAG_CAN1)&0x00FF;
	diag_dat.can_lin_diag = mpc33908_read(DIAG_CAN_LIN)&0x00FF;
	diag_dat.spi_diag = mpc33908_read(DIAG_SPI)&0x00FF;
	diag_dat.fs1_diag = mpc33908_read(DIAG_FS1)&0x00FF;
	diag_dat.fs2_diag = mpc33908_read(DIAG_FS2)&0x00FF;
	diag_dat.wd_counter = mpc33908_read(WD_COUNTER)&0x00FF;

	if(p_diag != (void *)0){
		*p_diag = diag_dat;
	}
	/* check diag_dat */

	return diag_result;
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



void mpc33908_mux_set(uint8_t channel){
	uint16_t data = 0;
	data = mpc33908_read(IO_OUT_AMUX);
	data &= 0x00F0;
	data |= channel&0x07;
	mpc33908_write(IO_OUT_AMUX, data);
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
void mpc33908_init(void ){

	char dev_name[DEV_NAME_LEN] = "MPC33908";
	int i = 0;
	uint16_t data = 0;
	/* configure mpc33908 device */
	mpc33908_spi.max_speed_hz = 8000000;		/* Max SPI operation frequency 8Mhz*/
	mpc33908_spi.cfg_select = 0;				/* Select Clock and Transfer Attributes Register*/
	mpc33908_spi.chip_select = 0;				/* chip select 0 */
	mpc33908_spi.bits_per_word = 16;			/* 16 bit perword */
	mpc33908_spi.dev_CPHA = 1;					/* data captured on the following edge */
	mpc33908_spi.dev_CPOL = 0;					/* inactive state value of SCK is low */
	mpc33908_spi.dev_single = 1;				/* single device */
	mpc33908_spi.dev_LSB_first = 0;				/* msb first */
	mpc33908_spi.actual_speed_hz = 2000000;		/* 2Mhz */
	mpc33908_spi.master = &bus0;
	mpc33908_spi.start_delay_nsecs = 500;			/* delay 500 ns */
	mpc33908_spi.end_delay_nsecs = 500;
	mpc33908_spi.next_frame_delay_nsecs = 500;
	/* SPI bus setup */
	mpc33908_spi.master->setup(&mpc33908_spi);

	/* transfer option configure */
	mpc33908_txopt.spi_bus_num = 0;
	mpc33908_txopt.bits_per_word = 16;
	mpc33908_txopt.cfg_select = 0;
	mpc33908_txopt.cs_num = 0;
	mpc33908_txopt.rx_buf = (void *)mpc33908_rxbuf;
	mpc33908_txopt.tx_buf = (void *)mpc33908_txbuf;
	mpc33908_txopt.rx_len = 1;
	mpc33908_txopt.tx_len = 1;
	mpc33908_txopt.complete = mpc33908_complete;
	mpc33908_txopt.context = (void *)&mpc33908_spi;
	mpc33908_txopt.transfer_mode = TRANSFER_WRITE_READ_MODE;	/* TRANSFER_WRITE_READ_MODE, TRANSFER_READ_MODE*/
	mpc33908_txopt.cs_mode = CS_SINGLE_MODE;


	for(i = 0; i < DEV_NAME_LEN; i ++){
		mpc33908_spi.devname[i] = dev_name[i];
	}

	/* read wake up source */
	data = mpc33908_read(WU_SOURCE);

	if((data & 0x00FF) == 0){
		/* init mpc33908 register */
		/* VREG1, VREG2 use default configuration */
		data = mpc33908_read(INIT_Vreg1);
		mpc33908_write(INIT_Vreg1, data & 0x00FF);

		data = mpc33908_read(INIT_Vreg2);
		mpc33908_write(INIT_Vreg2, data & 0x00FF);
		/* CAN_LIN REG use default configuration */
		data = mpc33908_read(INIT_CAN_LIN);
		mpc33908_write(INIT_CAN_LIN, data & 0x00FF);
		/* init IO_WU REG, IO0, IO1, IO4, IO5 used as wake up IO, configure as rising edge */
		/* init IO_WU REG, inhibit IO1, IO0, IO2, IO3 INT pulse */
		mpc33908_write(INIT_IO_WU1, WU1_CFG);	/*|WU_0|WU_0|WU_1|WU_1|WU_2|WU_2|INT_IO_1 |INT_IO_0 | */
		mpc33908_write(INIT_IO_WU2, WU2_CFG);	/*|WU_3|WU_3|WU_4|WU_4|WU_5|WU_5|INT_IO_23|INT_IO_45| */

		/* init SUPER1 REG, use default configuration */
		data = mpc33908_read(INIT_SUPER1);
		mpc33908_write(INIT_SUPER1, mpc33908_compute_secure(data & 0x00FF));
		/* init SUPER2 REG, use default ocnfiguration */
		data = mpc33908_read(INIT_SUPER2);
		mpc33908_write(INIT_SUPER2, mpc33908_compute_secure(data & 0x00FF));
		/* init SUPER3 REG, use defalut configuration */
		data = mpc33908_read(INIT_SUPER3);
		mpc33908_write(INIT_SUPER3, mpc33908_compute_secure(data & 0x00FF));
		/* init FSSM1 REG, use defalut cfg IO01, IO1, IO45 not safety, RSTB_low 10ms */
		data = mpc33908_read(INIT_FSSM1);
		mpc33908_write(INIT_FSSM1, mpc33908_compute_secure(data & 0x00FF));
		/* init FSSM2 REG, cfg IO32 as SAFETY CRITICAL, cfg RSTB error counter 3, 6 */
		/* init FSSM2 REG, cfg FCCU polarity HIGH */
		mpc33908_write(INIT_FSSM2, mpc33908_compute_secure(FSSM2_CFG));
		/* init WD REG, in normal mode, cfg as 8ms */
		mpc33908_write(WD_WINDOW, mpc33908_compute_secure(WD_WINDOW_CFG));

	}else{
		/* read can_wu to clear can_wu flag */
		if((data & 0x0001) == 1){
			/* can wake up */
			data = mpc33908_read(CAN_LIN_MODE);
		}
	}

	/* do first answer */
	mpc33908_feed_watchdog();
	/* set normal mode */
	mpc33908_write(INIT_INT, 0x7F);

}












