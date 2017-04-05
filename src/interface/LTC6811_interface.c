/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ10ÈÕ
* Compiler   : gcc
* FILE NAME  : LTC6811_drv.c
*
* DESCRIPTION:
*
*/

#include "SPI_bus_drv.h"
#include "LTC6811_interface.h"

extern struct spi_master bus1;

static struct spi_device LTC6811_spi;
static struct spi_transfer LTC6811_transfer;
static uint8_t LTC6811_txbuf[LTC6811_BUF_LEN];
static uint8_t LTC6811_rxbuf[LTC6811_BUF_LEN+1];

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
void LTC6811_init(void ){

	char dev_name[DEV_NAME_LEN] = "LTC6811";
	int i = 0;
	/* configure LTC6811 device */
	LTC6811_spi.max_speed_hz = 1000000;		/* Max SPI operation frequency 1Mhz*/
	LTC6811_spi.cfg_select = 1;				/* Select Clock and Transfer Attributes Register*/
	LTC6811_spi.chip_select = 1;			/* chip select 1 */
	LTC6811_spi.bits_per_word = 8;			/* 8 bit perword */
	LTC6811_spi.dev_CPHA = 1;				/* data captured on the rising edge */
	LTC6811_spi.dev_CPOL = 1;				/* inactive state value of SCK is high */
	LTC6811_spi.dev_single = 1;				/* single device */
	LTC6811_spi.dev_LSB_first = 0;			/* msb first */
	LTC6811_spi.actual_speed_hz = 500000;	/* 500Khz */
	LTC6811_spi.master = &bus1;
	LTC6811_spi.start_delay_nsecs = 5000;		/* delay 5 us */
	LTC6811_spi.end_delay_nsecs = 5000;
	LTC6811_spi.next_frame_delay_nsecs = 5000;
	/* SPI bus setup */
	LTC6811_spi.master->setup(&LTC6811_spi);

	/* transfer option configure */
	LTC6811_transfer.spi_bus_num = 1;
	LTC6811_transfer.bits_per_word = 8;
	LTC6811_transfer.cfg_select = 1;
	LTC6811_transfer.cs_num = 1;
	LTC6811_transfer.rx_buf = (void *)LTC6811_rxbuf;
	LTC6811_transfer.tx_buf = (void *)LTC6811_txbuf;
	LTC6811_transfer.rx_len = 1;
	LTC6811_transfer.tx_len = 1;
	LTC6811_transfer.complete = (void *)0;
	LTC6811_transfer.context = (void *)0;
	LTC6811_transfer.transfer_mode = TRANSFER_WRITE_READ_MODE;	/* TRANSFER_WRITE_READ_MODE, TRANSFER_READ_MODE*/
	LTC6811_transfer.cs_mode = CS_CONT_MODE;


	for(i = 0; i < DEV_NAME_LEN; i ++){
		LTC6811_spi.devname[i] = dev_name[i];
	}
}


static const unsigned short pec15_Table[256] = {0x0, 0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  //precomputed CRC15 Table
                                       0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
                                       0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
                                       0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
                                       0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
                                       0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
                                       0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
                                       0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
                                       0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
                                       0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
                                       0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
                                       0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
                                       0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
                                       0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
                                       0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
                                       0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
                                       0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
                                       0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
                                       0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
                                       0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
                                       0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
                                       0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
                                       0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095};



/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function .
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**
**-----------------------------------------------------------------------------
*/
unsigned short pec15_calc(unsigned char len, unsigned char  *data)
{
    unsigned char i;
	unsigned short remainder, addr;

	remainder = 16;//initialize the PEC
	for(i = 0; i<len; i++) // loops for each byte in data array
	{
		addr = ((remainder>>7)^data[i])&0xff;//calculate PEC table address
		remainder = (remainder<<8)^pec15_Table[addr];
	}

	return(remainder << 1);//The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
#define LTC6811_ACTIVE_LEN	2

static int32_t LTC6811_ACTIVE(void){
	int32_t ret = 0;
	int32_t i = 0;
	LTC6811_transfer.tx_len = LTC6811_ACTIVE_LEN;
	LTC6811_transfer.transfer_mode = TRANSFER_WRITE_MODE;
	LTC6811_transfer.tx_timeout = LTC6811_ACTIVE_LEN * LTC6811_TIME_OUT;

	for(i = 0; i < LTC6811_ACTIVE_LEN; i ++ ){
		LTC6811_txbuf[i] = 0xFF;
	}

	/* send CMD and data */
	ret = spi_bus_write(&LTC6811_spi, &LTC6811_transfer);
	if(ret == 0){
		while((LTC6811_transfer.tx_end_flag == 0) && (LTC6811_transfer.tx_timeout -- > 0)){
			;
		}
		LTC6811_transfer.tx_end_flag = 0;

		if(LTC6811_transfer.tx_timeout <= 0){
			ret = -2;
		}
	}

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
static int32_t LTC6811_tx(uint16_t cmd, uint8_t dat_len, struct spi_transfer *p_transfer){
	uint8_t *p_txbuf = (uint8_t *)p_transfer->tx_buf;
	uint16_t pec = 0;
	int32_t ret = 0;
	/* check parameter */
	if(dat_len > LTC6811_BUF_LEN){
		/* parameter error */
		ret = -1;
	}else{

		LTC6811_ACTIVE();

		p_txbuf[0] = (uint8_t)(cmd >> 8);
		p_txbuf[1] = (uint8_t)cmd;

		pec = pec15_calc(2, p_txbuf);

		p_txbuf[2] = (uint8_t)(pec >> 8);
		p_txbuf[3] = (uint8_t)pec;

		p_transfer->tx_len = dat_len;
		p_transfer->transfer_mode = TRANSFER_WRITE_MODE;
		p_transfer->tx_timeout = dat_len * LTC6811_TIME_OUT;
		/* send CMD and data */
		ret = spi_bus_write(&LTC6811_spi, p_transfer);

		if(ret == 0){
			while((p_transfer->tx_end_flag == 0) && (p_transfer->tx_timeout -- > 0)){
				;
			}
			p_transfer->tx_end_flag = 0;
			if(p_transfer->tx_timeout > 0){
				/* transfer OK */
			}else{
				/* transfer timeout */
				ret = -2;
			}
		}
	}

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
static int32_t LTC6811_rx(uint16_t cmd, uint8_t dat_len, struct spi_transfer *p_transfer){
	uint8_t *p_txbuf = (uint8_t *)p_transfer->tx_buf;
	uint16_t pec = 0;
	int32_t ret = 0;
	/* check parameter */
	if(dat_len > LTC6811_BUF_LEN){
		/* error */
	}else{

		LTC6811_ACTIVE();

		p_txbuf[0] = (uint8_t)(cmd >> 8);
		p_txbuf[1] = (uint8_t)cmd;
		pec = pec15_calc(2, p_txbuf);
		p_txbuf[2] = (uint8_t)(pec >> 8);
		p_txbuf[3] = (uint8_t)pec;

		p_transfer->tx_len = 4;
		p_transfer->rx_len = dat_len + 1;
		p_transfer->transfer_mode = TRANSFER_WRITE_READ_MODE;
		p_transfer->rx_timeout = dat_len * LTC6811_TIME_OUT;
		/* send CMD and data */
		ret = spi_bus_write(&LTC6811_spi, p_transfer);

		if(ret == 0){
			/* wait for read data */
			while((p_transfer->rx_new_dat_flag == 0) && (p_transfer->rx_timeout -- > 0)){
				;
			}
			p_transfer->rx_new_dat_flag = 0;
		}
	}

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
static int32_t LTC6811_read(enum LTC6811_CMD_LIST cmd, uint16_t chip_num, struct LTC6811Reg *p_dev){
	uint16_t pec = 0;
	uint16_t rx_pec = 0;
	int32_t ret = 0;
	int32_t i = 0;
	uint8_t dat_len = chip_num * LTC6811_REG_LEN;
	
	uint8_t *p_rxbuf = &LTC6811_rxbuf[1];

	uint8_t reg_num = 0;

	switch(cmd){
	case RDCFGA:
		reg_num = CFGR;
		break;
	case RDCVA:
		reg_num = CVAR;
			break;
	case RDCVB:
		reg_num = CVBR;
			break;
	case RDCVC:
		reg_num = CVCR;
			break;
	case RDCVD:
		reg_num = CVDR;
			break;
	case RDAUXA:
		reg_num = AVAR;
			break;
	case RDAUXB:
		reg_num = AVBR;
			break;
	case RDSTATA:
		reg_num = STAR;
			break;
	case RDSTATB:
		reg_num = STBR;
			break;
	case RDSCTRL:
		reg_num = SCTRL;
			break;
	case RDPWM:
		reg_num = PWM;
			break;
	default:
		break;
	}

	if((chip_num <= LTC6811_MAX_CHIP_NUM) && (p_dev != 0)){

		ret = LTC6811_rx(cmd, dat_len, &LTC6811_transfer);

		if(ret == 0){
			for(i = 0; i < chip_num; i ++, p_dev ++){
				rx_pec = p_rxbuf[6]*256 + p_rxbuf[7];

				pec = pec15_calc(6, p_rxbuf);

				if(pec == rx_pec){
					p_dev->reg[reg_num].dat[0] = p_rxbuf[0];
					p_dev->reg[reg_num].dat[1] = p_rxbuf[1];
					p_dev->reg[reg_num].dat[2] = p_rxbuf[2];
					p_dev->reg[reg_num].dat[3] = p_rxbuf[3];
					p_dev->reg[reg_num].dat[4] = p_rxbuf[4];
					p_dev->reg[reg_num].dat[5] = p_rxbuf[5];
				}else{
					ret = -3;
				}
				p_rxbuf += LTC6811_REG_LEN;
			}
		}
	}else{
		ret = -1;
	}

return ret;	
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_write(enum LTC6811_CMD_LIST cmd, uint16_t chip_num, struct LTC6811Reg *p_dev){
	uint16_t pec = 0;
	int32_t ret = 0;
	uint8_t dat_len = chip_num * LTC6811_REG_LEN + 4;
	uint8_t reg_num = 0;
	uint8_t *p_txbuf = &LTC6811_txbuf[4];

	switch(cmd){
	case WRCFGA:
		reg_num = CFGR;
		break;
	case WRSCTRL:
		reg_num = SCTRL;
			break;
	case WRPWM:
		reg_num = PWM;
			break;
	default:
		break;
	}

	if((chip_num <= LTC6811_MAX_CHIP_NUM) && (p_dev != 0)){

		while(chip_num -- > 0){
			p_txbuf[0] = p_dev->reg[reg_num].dat[0];
			p_txbuf[1] = p_dev->reg[reg_num].dat[1];
			p_txbuf[2] = p_dev->reg[reg_num].dat[2];
			p_txbuf[3] = p_dev->reg[reg_num].dat[3];
			p_txbuf[4] = p_dev->reg[reg_num].dat[4];
			p_txbuf[5] = p_dev->reg[reg_num].dat[5];

			pec = pec15_calc(6, p_txbuf);

			p_txbuf[6] = (uint8_t)(pec >> 8);
			p_txbuf[7] = (uint8_t)pec;

			p_txbuf += LTC6811_REG_LEN;
			p_dev ++;
		}

		LTC6811_tx(cmd, dat_len, &LTC6811_transfer);
	}else{
		ret = -1;
	}

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_ctrl(enum LTC6811_CMD_LIST cmd, uint16_t parameter){

	uint16_t command = cmd | parameter;

	int32_t ret = 0;

	ret = LTC6811_tx(command, 4, &LTC6811_transfer);

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADCV(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADCV, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADOW(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADOW, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADOL(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADOL, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADAX(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADAX, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADAXD(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADAXD, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_SELFTEST_CV(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(CVST, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_SELFTEST_GPIO(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(AXST, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_SELFTEST_STAT(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(STATST, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADSTAT(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADSTAT, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADSTATD(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADSTAT, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADCVAX(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADCVAX, parameter);
	return ret;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_START_ADCVSC(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(ADCVSC, parameter);
	return ret;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_CLRSCTRL(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(CLRSCTRL, parameter);
	return ret;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_CLRCELL(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(CLRCELL, parameter);
	return ret;
}


/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_CLRAUX(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(CLRAUX, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_CLRSTAT(uint16_t parameter){
	int32_t ret = 0;
	ret = LTC6811_ctrl(CLRSTAT, parameter);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_DIAG_MUX(void){
	int32_t ret = 0;
	ret = LTC6811_ctrl(DIAGN, 0);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_SET_GPIO5(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_write(WRCFGA, chip_num, p_dev);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_RDCV(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_read(RDCVA, chip_num, p_dev);
	ret = LTC6811_read(RDCVB, chip_num, p_dev);
	ret = LTC6811_read(RDCVC, chip_num, p_dev);
	ret = LTC6811_read(RDCVD, chip_num, p_dev);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_RDAUX(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_read(RDAUXA, chip_num, p_dev);
	ret = LTC6811_read(RDAUXB, chip_num, p_dev);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_RDSTAT(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_read(RDSTATA, chip_num, p_dev);
	ret = LTC6811_read(RDSTATB, chip_num, p_dev);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_RDSCTRL(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_read(RDSCTRL, chip_num, p_dev);

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LTC6811_RDPWM(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_read(RDPWM, chip_num, p_dev);

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_WRCFGA(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_write(WRCFGA, chip_num, p_dev);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_WRSCTRL(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_write(WRSCTRL, chip_num, p_dev);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is sampling anolog data
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**		None
**	Notes:
**		calling by cycle,less than 10ms cycles
**-----------------------------------------------------------------------------
*/
int32_t LT6811_WRPWM(uint16_t chip_num, struct LTC6811Reg *p_dev){
	int32_t ret = 0;
	ret = LTC6811_write(WRPWM, chip_num, p_dev);
	return ret;
}




