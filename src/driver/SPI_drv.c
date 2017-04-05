/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê9ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : SPI_drv.c
*
* DESCRIPTION:
*
*/

#include "SPI_drv.h"
#include "SIUL.h"

#define MCU_SYS_CLK		200000000		///defines system clock frequency in Hz
#define MCU_PLL0_CLK	160000000		///defines frequency of the PLL0 clock
#define DSPI_CLK		MCU_SYS_CLK/4	///defined by the MC_CGM.SC_DC0.R setting

#define NULL	(void *)0
#define HIGH_WORD_MASK	0xFFFF0000
#define LOW_WORD_MASK	0x0000FFFF

struct spi_message{
	const void	*tx_buf;
	void		*rx_buf;
	int16_t  tx_cnt;
	int16_t  rx_cnt;
	struct spi_transfer *p_dat;
	struct spi_device *p_dev;
};


static struct spi_message bus_msg[MPC5744_SPI_NUM];
static volatile struct SPI_tag *p_spi_base_addr[MPC5744_SPI_NUM] = {
		(volatile struct SPI_tag *) 0xFFE70000UL,
		(volatile struct SPI_tag *) 0xFFE74000UL,
		(volatile struct SPI_tag *) 0xFBE70000UL,
		(volatile struct SPI_tag *) 0xFBE74000UL
};

static struct spi_transfer temp_transfer[MPC5744_SPI_NUM];

static void spi_rx_irq_server(uint8_t bus_num){
	volatile struct SPI_tag *p_DSPI;				//base pointer
	SPI_PUSHR_PUSHR_tag push_reg = {0};

	if(bus_num > MPC5744_SPI_NUM){
		bus_num = 0;
	}
	p_DSPI = p_spi_base_addr[bus_num];

	/* parameter check */
	if((bus_msg[bus_num].rx_buf != NULL) && (bus_msg[bus_num].p_dat != NULL) && (bus_msg[bus_num].p_dev != NULL)){
		bus_msg[bus_num].rx_cnt ++;
		push_reg = p_DSPI->PUSHR.PUSHR;
		/* clear transfer flag */
		p_DSPI->SR.R |= SPI_RSER_TCF_MASK;				/* Clear Transfert Flag */

		if(bus_msg[bus_num].rx_cnt < bus_msg[bus_num].p_dat->rx_len){
			/* receive start */
			/* check bitlengh */
			if(bus_msg[bus_num].p_dat->bits_per_word <= 8){
				*((uint8_t *)bus_msg[bus_num].rx_buf) = (uint8_t)p_DSPI->POPR.R;
				(uint8_t *)bus_msg[bus_num].rx_buf ++;

			}else{
				*((uint16_t *)bus_msg[bus_num].rx_buf) = (uint16_t)p_DSPI->POPR.R;
				(uint16_t *)bus_msg[bus_num].rx_buf ++;
			}

			if((bus_msg[bus_num].rx_cnt + 1) == bus_msg[bus_num].p_dat->rx_len){
				/* last send, clear continuous bit, set CS high at the end of send */
				push_reg.B.CONT = 0;
			}
			/* fill dummy data */
			push_reg.B.TXDATA = 0xFFFF;
			p_DSPI->PUSHR.PUSHR.R = push_reg.R;
		}else{
			/* receive the last data */
			/* check bitlengh */
			if(bus_msg[bus_num].p_dat->bits_per_word <= 8){
				*((uint8_t *)bus_msg[bus_num].rx_buf) = (uint8_t)p_DSPI->POPR.R;
			}else{
				*((uint16_t *)bus_msg[bus_num].rx_buf) = (uint16_t)p_DSPI->POPR.R;
			}
			/* set or clear bus message */
			bus_msg[bus_num].p_dat->rx_new_dat_flag = 1;
			bus_msg[bus_num].rx_cnt = 0;
			bus_msg[bus_num].rx_buf = NULL;
			/* disable receive interrupt */
			p_DSPI->RSER.B.RFDF_RE = 0;
			/* check bus_busy_flag */
			bus_msg[bus_num].p_dev->master->bus_busy_flag &= ~SPI_BUS_READ;
			if((bus_msg[bus_num].p_dev->master->bus_busy_flag & (~SPI_BUS_BUSY)) == 0){
				/* clear bus flag */
				bus_msg[bus_num].p_dev->master->bus_busy_flag = 0;
				/* call complete funciton */
				if(bus_msg[bus_num].p_dat->complete != NULL){
					bus_msg[bus_num].p_dat->complete(bus_msg[bus_num].p_dat->context);
				}else{
					/* complete function is NULL */
				}
				/* clear msg buffer */
				bus_msg[bus_num].p_dat = NULL;
				bus_msg[bus_num].p_dev = NULL;
			}
		}
	}else{
		/* rx buffer pointer is NULL */
		/* clear receive buffer */
		push_reg.R = p_DSPI->POPR.R;
	}
	/* clear interrupt flag */
	p_DSPI->SR.R |= SPI_RSER_RFDF_MASK;						//clear receive flag
}

static void spi_tx_irq_server(uint8_t bus_num){
	volatile struct SPI_tag *p_DSPI;				//base pointer
	SPI_PUSHR_PUSHR_tag push_reg;
	uint16_t temp_dat = 0;

	if(bus_num > MPC5744_SPI_NUM){
		bus_num = 0;
	}
	p_DSPI = p_spi_base_addr[bus_num];

	/* clear transfer flag */
	p_DSPI->SR.R |= SPI_RSER_TCF_MASK;									// Clear Transfert Flag

	/* parameter check */
	if((bus_msg[bus_num].tx_buf != NULL) && (bus_msg[bus_num].p_dat != NULL) && (bus_msg[bus_num].p_dev != NULL)){
		bus_msg[bus_num].tx_cnt ++;
		if(bus_msg[bus_num].tx_cnt < bus_msg[bus_num].p_dat->tx_len){
			/* send next dat */
			/* check bitlength */
			if(bus_msg[bus_num].p_dat->bits_per_word <= 8){
				(uint8_t *)bus_msg[bus_num].tx_buf ++;
				temp_dat = *(uint8_t *)bus_msg[bus_num].tx_buf;
			}else{
				(uint16_t *)bus_msg[bus_num].tx_buf ++;
				temp_dat = *(uint16_t *)bus_msg[bus_num].tx_buf;
			}
			/* set data */
			push_reg = p_DSPI->PUSHR.PUSHR;
			push_reg.R &= 0xFFFF0000;
			push_reg.B.TXDATA = temp_dat;
			if((bus_msg[bus_num].tx_cnt + 1) == bus_msg[bus_num].p_dat->tx_len){
				/* last send, clear continuous bit, set CS high at the end of send */
				if((bus_msg[bus_num].p_dat->transfer_mode != TRANSFER_WRITE_READ_MODE) &&
				   (bus_msg[bus_num].p_dat->transfer_mode != TRANSFER_WRITE_CONTINUOUS)){
					push_reg.B.CONT = 0;
				}else if(bus_msg[bus_num].p_dat->rx_len == 1){
					push_reg.B.CONT = 0;
				}else{
					/* do nothing, keep last status */
				}
			}
			/* clear reveive buffer */
			temp_dat = p_DSPI->POPR.R;
			/* clear interrupt flag */
			p_DSPI->SR.R |= SPI_RSER_RFDF_MASK;		/* clear receive flag */
			/* send data */
			p_DSPI->PUSHR.PUSHR.R = push_reg.R;

		}else{
			/* send end */
			DI();
			bus_msg[bus_num].p_dat->tx_end_flag = 1;
			bus_msg[bus_num].tx_cnt = 0;
			bus_msg[bus_num].tx_buf = NULL;
			/* disable transmit interrupt */
			p_DSPI->RSER.B.TCF_RE = 0;
			EI();
			/* call complete funciton */
			if(bus_msg[bus_num].p_dat->complete != NULL){
				bus_msg[bus_num].p_dat->complete(bus_msg[bus_num].p_dat->context);
			}else{
				/* complete function is empty, do nothing */
			}
			/* clear bus flag */
			DI();
			bus_msg[bus_num].p_dev->master->bus_busy_flag &= ~SPI_BUS_WRITE;
			if((bus_msg[bus_num].p_dev->master->bus_busy_flag & (~SPI_BUS_BUSY)) == 0){
				bus_msg[bus_num].p_dev->master->bus_busy_flag = 0;

				/* clear msg buffer */
				bus_msg[bus_num].p_dat = NULL;
				bus_msg[bus_num].p_dev = NULL;
			}
			EI();
		}/* check tx_cnt end */
	}/* parameter check end */
}

void spi0_rx_irq_handler(void){
	spi_rx_irq_server(0);
}

void spi0_tx_irq_handler(void){
	spi_tx_irq_server(0);
}

void spi1_rx_irq_handler(void){
	spi_rx_irq_server(1);
}

void spi1_tx_irq_handler(void){
	spi_tx_irq_server(1);
}

void spi2_rx_irq_handler(void){
	spi_rx_irq_server(2);
}

void spi2_tx_irq_handler(void){
	spi_tx_irq_server(2);
}


/* interrupt */

/* 259-TFUF: Transmit FIFO underflow flag, 1: TX fifo underflow has occurred */
/* 260-EOQF: End of queue flag, 1: */
/* 261-TFFF: Transmit FIFO fill flag, 1: TX fifo is not full, 0: TX fifo is full */
/* 262-TCF:  Transfer complete flag, 1: Transfer complete */
/* 263-RFDF: Receive FIFO drain flag, 1: RX fifo is not empty */
/**/
void mpc5744_spi_irq_init(uint8_t spi_num){
	volatile struct SPI_tag *p_DSPI;
	switch(spi_num){											//choose base DSPI address
		case 0 :
			p_DSPI = &SPI_0;
		    INTC_0.PSR[262].B.PRIN = 7;
		    INTC_0.PSR[263].B.PRIN = 7;
		    p_DSPI->RSER.B.TCF_RE = 1;
		    p_DSPI->RSER.B.RFDF_RE = 1;
		break;
		case 1 :
			p_DSPI = &SPI_1;
		    INTC_0.PSR[271].B.PRIN = 7;
		    INTC_0.PSR[272].B.PRIN = 7;
		    p_DSPI->RSER.B.TCF_RE = 1;
		    p_DSPI->RSER.B.RFDF_RE = 1;
			break;
		case 2 :
			p_DSPI = &SPI_2;
		    INTC_0.PSR[280].B.PRIN = 7;
		    INTC_0.PSR[281].B.PRIN = 7;
		    p_DSPI->RSER.B.TCF_RE = 1;
		    p_DSPI->RSER.B.RFDF_RE = 1;
			break;
		case 3 :
			p_DSPI = &SPI_3;
		    INTC_0.PSR[289].B.PRIN = 7;
		    INTC_0.PSR[290].B.PRIN = 7;
		    p_DSPI->RSER.B.TCF_RE = 1;
		    p_DSPI->RSER.B.RFDF_RE = 1;
			break;
		default:
			break;
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
 *
 *
 */
/*********************************************************************************/
static void mpc5744_spi_tx_complete(void *context){
	volatile struct SPI_tag *p_DSPI;
	struct spi_transfer *temp = (struct spi_transfer *)context;
	uint8_t bus_num = temp->spi_bus_num;

	if(bus_num > MPC5744_SPI_NUM){
		bus_num = 0;
	}
	p_DSPI = p_spi_base_addr[bus_num];

	bus_msg[bus_num].p_dat = temp;

	/* enable receive interrupt */
	p_DSPI->RSER.R = SPI_RSER_RFDF_MASK;
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
int32_t mpc5744_spi_tx(struct spi_device *dev, struct spi_transfer *dat){
	volatile struct SPI_tag *p_DSPI;				//base pointer
	SPI_PUSHR_PUSHR_tag push_reg = {0};
	uint16_t temp_dat = 0;
	uint8_t bus_num = 0;
	int32_t ret = 0;

	if((dev != NULL) && (dat != NULL)){
		bus_num = dev->master->bus_num;
		if(bus_num < MPC5744_SPI_NUM){
			bus_msg[bus_num].rx_buf = dat->rx_buf;
			bus_msg[bus_num].tx_buf = dat->tx_buf;
			bus_msg[bus_num].p_dat = dat;
			bus_msg[bus_num].p_dev = dev;

			p_DSPI = p_spi_base_addr[bus_num];
			/* send register address */
			/* check bitlength */
			if(dat->bits_per_word <= 8){
				temp_dat = *(uint8_t *)dat->tx_buf;
			}else{
				temp_dat = *(uint16_t *)dat->tx_buf;
			}
			/* clear TX counter, RX counter */
			p_DSPI->MCR.B.CLR_TXF = 1;
			p_DSPI->MCR.B.CLR_RXF = 1;
			/* clear TX interrupt flag, RX interrupt flag */
			p_DSPI->SR.R = 0xFFFFFFFF;
			/* disable interrupt */
			p_DSPI->RSER.R = 0;
			/* Continuous Peripheral Chip Select Enable */
			push_reg.B.CONT = dat->cs_mode;
			/* select CTAR0 ~ 3 */
			push_reg.B.CTAS = dat->cfg_select;
			/* set CS */
			push_reg.B.PCS = 1 << dat->cs_num;
			/* Clear Transfer Counter */
			push_reg.B.CTCNT = 1;
			/* set data */
			push_reg.B.TXDATA = temp_dat;
			/* send first data */
			p_DSPI->PUSHR.PUSHR.R = push_reg.R;
			/* send next data with interrupt */
			/* set interrupt mask */
			if(dat->transfer_mode == TRANSFER_READ_MODE){
				p_DSPI->RSER.R = SPI_RSER_RFDF_MASK;
			}else if((dat->transfer_mode == TRANSFER_WRITE_MODE) ||
					(dat->transfer_mode == TRANSFER_WRITE_CONTINUOUS)){
				p_DSPI->RSER.R = SPI_RSER_TCF_MASK;
			}else if(dat->transfer_mode == TRANSFER_WRITE_READ_MODE){

				temp_transfer[bus_num] = *dat;
				temp_transfer[bus_num].complete = mpc5744_spi_tx_complete;
				temp_transfer[bus_num].context = (void *)dat;
				temp_transfer[bus_num].spi_bus_num = bus_num;

				bus_msg[bus_num].p_dat = &temp_transfer[bus_num];

				p_DSPI->RSER.R = SPI_RSER_TCF_MASK;
			}else{
				p_DSPI->RSER.R = 0;
			}
		}else{
			/* bus_num error */
			ret = -1;
		}
	}else{
		/* parameter error */
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
void mpc5744_spi_init(uint8_t DspiNumber,uint8_t mode, uint32_t intMask){
    volatile struct SPI_tag *p_DSPI;			//base pointer

	if(DspiNumber > MPC5744_SPI_NUM){
		DspiNumber = 0;
	}
	p_DSPI = p_spi_base_addr[DspiNumber];

    p_DSPI->MCR.B.MSTR = mode;		// Set DSPIx in mode Slave-0 or Master-1
    p_DSPI->MCR.B.MDIS=0;    			// Enable clock
    p_DSPI->MCR.B.HALT=0;					// Allow transfer

    p_DSPI->MCR.B.PCSIS=0x3F;		// CS0-5 active Low
    p_DSPI->MCR.B.ROOE = 1;			/* Incoming data is shifted into the shift register. */

    //FIFOs
    p_DSPI->MCR.B.DIS_TXF=1;		//FIFOs
    p_DSPI->MCR.B.DIS_RXF=1;

    p_DSPI->RSER.R = intMask;	//interrupt mask

    p_DSPI->MCR.B.FRZ = 0;

    //Configuration of ports
    switch(DspiNumber)
    {
		case 0 :
			if(mode)
            {   // as a Master
                SIUL_DigitalInput(SIUL_PC7, SIUL_IN_PC7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_0 	->IN
                SIUL_DigitalOutput(SIUL_PC6, SIUL_ALT1);                    // SOUT_0	->OUT
                SIUL_DigitalOutput(SIUL_PC5, SIUL_ALT1);                    // SCK_0	->OUT
                SIUL_DigitalOutput(SIUL_PC4, SIUL_ALT1);                    // CS0_0	->OUT
            }
            else
            {	//as a Slave
                SIUL_DigitalInput(SIUL_PC7, SIUL_IN_PC7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_0	->IN
                SIUL_DigitalOutput(SIUL_PC6, SIUL_ALT1);                    // SOUT_0	->OUT
                SIUL_DigitalInputSimple(SIUL_PC5, SIUL_PULL_UP | SIUL_SLEW_ENB);      // SCK_0	->IN
                SIUL_DigitalInputSimple(SIUL_PC4, SIUL_PULL_UP | SIUL_SLEW_ENB);      // CS0_0	->IN
            }
		break;

        case 1 	:
            if(mode)
            {	//as a Master
                SIUL_DigitalInput(SIUL_PA8, SIUL_IN_PA8, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_1 	->IN
                SIUL_DigitalOutput(SIUL_PA7, SIUL_ALT1);                    // SOUT_1	->OUT
                SIUL_DigitalOutput(SIUL_PA6, SIUL_ALT1);                    // SCK_1	->OUT
                SIUL_DigitalOutput(SIUL_PA5, SIUL_ALT1);                    // CS0_1	->OUT
                SIUL_DigitalOutput(SIUL_PG9, SIUL_ALT2);                    // CS1_1	->OUT
            }
            else
            {	//as a Slave
                SIUL_DigitalInput(SIUL_PA8, SIUL_IN_PA8, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_1 	->IN
                SIUL_DigitalOutput(SIUL_PA7, SIUL_ALT1);                    // SOUT_1	->OUT
                SIUL_DigitalInputSimple(SIUL_PA6, SIUL_PULL_UP | SIUL_SLEW_ENB);      // SCK_1	->IN
                SIUL_DigitalInputSimple(SIUL_PA5, SIUL_PULL_UP | SIUL_SLEW_ENB);      // CS0_1	->IN
            }
            break;
        case 2 	:
            if(mode)
            {		//as a Master
                SIUL_DigitalInput(SIUL_PA13, SIUL_IN_PA2, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_2 	->IN
                SIUL_DigitalOutput(SIUL_PA11, SIUL_ALT1);                    // SOUT_2	->OUT
                SIUL_DigitalOutput(SIUL_PA12, SIUL_ALT1);                    // SCK_2	->OUT
                SIUL_DigitalOutput(SIUL_PA10, SIUL_ALT1);                    // CS0_2	->OUT
                SIUL_DigitalOutput(SIUL_PA9, SIUL_ALT1);                    // CS1_2	->OUT
                SIUL_DigitalOutput(SIUL_PC10, SIUL_ALT1);                    // CS2_2	->OUT
            }
            else
            {				//as a Slave
                SIUL_DigitalInput(SIUL_PA2, SIUL_IN_PA2, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT2);      // SIN_2 	->IN
                SIUL_DigitalOutput(SIUL_PA1, SIUL_ALT2);                    // SOUT_2	->OUT
                SIUL_DigitalInput(SIUL_PA0, SIUL_IN_PA0_DSPI2 , SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SCK_2	->IN
                SIUL_DigitalInput(SIUL_PA3, SIUL_IN_PA3, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // CS0_2	->IN
            }
            break;
		case 3 :
			if(mode)
            {		//as a Master
                SIUL_DigitalInput(SIUL_PD7, SIUL_IN_PD7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT2);      // SIN_2 	->IN
                SIUL_DigitalOutput(SIUL_PD5, SIUL_ALT4);                    // SOUT_2	->OUT
                SIUL_DigitalOutput(SIUL_PD6, SIUL_ALT4);                    // SCK_2	->OUT
                SIUL_DigitalOutput(SIUL_PC11, SIUL_ALT4);                    // CS0_3	->OUT
            }
            else
            {				//as a Slave
                SIUL_DigitalInput(SIUL_PD7, SIUL_IN_PD7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT2);      // SIN_2 	->IN
                SIUL_DigitalOutput(SIUL_PD5, SIUL_ALT4);                    // SOUT_2	->OUT
                SIUL_DigitalInput(SIUL_PD6, SIUL_IN_PD6, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SCK_2	->IN
                SIUL_DigitalInput(SIUL_PC11, SIUL_IN_PC11, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // CS0_3	->IN
            }
            break;

        default :
			if(mode)
			{   // as a Master
				SIUL_DigitalInput(SIUL_PC7, SIUL_IN_PC7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_0 	->IN
				SIUL_DigitalOutput(SIUL_PC6, SIUL_ALT1);                    // SOUT_0	->OUT
				SIUL_DigitalOutput(SIUL_PC5, SIUL_ALT1);                    // SCK_0	->OUT
				SIUL_DigitalOutput(SIUL_PC4, SIUL_ALT1);                    // CS0_0	->OUT
			}
			else
			{	//as a Slave
				SIUL_DigitalInput(SIUL_PC7, SIUL_IN_PC7, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);      // SIN_0	->IN
				SIUL_DigitalOutput(SIUL_PC6, SIUL_ALT1);                    // SOUT_0	->OUT
				SIUL_DigitalInputSimple(SIUL_PC5, SIUL_PULL_UP | SIUL_SLEW_ENB);      // SCK_0	->IN
				SIUL_DigitalInputSimple(SIUL_PC4, SIUL_PULL_UP | SIUL_SLEW_ENB);      // CS0_0	->IN
			}
			break;
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

struct BR_AND_VALUE{
	uint32_t br;
	uint32_t value;
};
static struct BR_AND_VALUE br_calculate(uint32_t scaler_value){

	struct BR_AND_VALUE br_value = {0, 1};

	if(scaler_value <= 2){
		br_value.br = 0;
		br_value.value = 2;
	}else if(scaler_value <= 6){
		br_value.br = 1;
		br_value.value = 4;
	}else if(scaler_value < 8){
		br_value.br = 2;
		br_value.value = 6;
	}else{
		while(scaler_value > 1){
			br_value.br += 1;
			br_value.value *= 2;
			scaler_value /= 2;
		}
	}

	return br_value;
}

/*********************************************************************************/
/* Description:
 *       baud_rate = sys_clk / (PBR *BR)
 *
 * Input:
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
static uint32_t spi_bitrate_calculate(uint32_t sys_clk, uint32_t spi_speed){
	uint32_t spi_prescale = sys_clk/spi_speed;
	uint32_t pbr[4] = {2, 3, 5, 7};
	uint32_t bitrate[4] = {0};
	int32_t bitrate_diff[4] = {0};
	uint32_t br_scale_value[4] = {0};
	uint32_t temp = 0;
	uint32_t br_pbr = 0;
	int32_t min = 0x7FFFFFFF;
	int32_t min_cnt = 0;

	struct BR_AND_VALUE br_value[4] = {{0,0},{0,0},{0,0},{0,0}};

	br_scale_value[0] = spi_prescale/pbr[0];
	br_scale_value[1] = spi_prescale/pbr[1];
	br_scale_value[2] = spi_prescale/pbr[2];
	br_scale_value[3] = spi_prescale/pbr[3];

	if(br_scale_value[0] != 0){

		for(temp = 0; temp < 4; temp ++){
			br_value[temp] = br_calculate(br_scale_value[temp]);
			bitrate[temp] = sys_clk/(pbr[temp]*br_value[temp].value);

			if(bitrate[temp] > spi_speed){
				bitrate_diff[temp] = bitrate[temp] - spi_speed;
			}else{
				bitrate_diff[temp] = spi_speed - bitrate[temp];
			}
		}

		for(temp = 0; temp < 4; temp ++){
			if(bitrate_diff[temp] < min)
			{
				min = bitrate_diff[temp];
				min_cnt = temp;
			}
		}

	}else{
		/* extern max baund rate */
	}

	br_pbr = min_cnt | (br_value[min_cnt].br << 16);

	return br_pbr;
}

/*********************************************************************************/
/* Description:
 *		Tcssck = (1/fp ) * PCSSCK * CSSCK
 *		CSSCK = Tcssck * fp / PCSSCK
 * Input:	delay_time : ns
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
struct CSSCK_VALUE{
	uint32_t cssck;
	uint32_t value;
};

static struct CSSCK_VALUE cssck_calculate(uint32_t cssck_value){
	struct CSSCK_VALUE cssck = {0, 0};

	if(cssck_value > 65535){
		cssck.cssck = 15;
		cssck.value = 65536;
	}else{
		cssck_value /= 2;
		cssck.value = 2;

		while(cssck_value > 1){
			cssck.cssck += 1;
			cssck.value *= 2;
			cssck_value /= 2;
		}
	}

	return cssck;
};
/*********************************************************************************/
/* Description:
 *		Tcssck = (1/fp ) * PCSSCK * CSSCK
 *		CSSCK = Tcssck * fp / PCSSCK
 * Input:	delay_time : ns
 *
 * Output:
 *
 * Code Sample:
 *
 */
/*********************************************************************************/
static uint32_t spi_delay_calculate(uint32_t sys_clk, uint32_t delay_ns){

	uint32_t pcssck[4] = {1, 3, 5, 7};

	uint32_t cssck_scale_value[4] = {0};
	uint32_t sys_time = sys_clk/1000000;	/* Mhz, us */
	uint32_t max_time = 0;
	uint32_t cal_delay_time[4] = {0};
	int32_t cal_diff_time[4] = {0};
	int32_t cal_min_time = 0x7FFFFFFF;

	uint32_t i = 0;
	uint32_t j = 0;

	struct CSSCK_VALUE cssck_dat[4] = {{0,0}, {0,0}, {0,0}, {0,0}};

	sys_time = 1000/sys_time;		/* ns */
	max_time = sys_time * 7 * 65536;

	if(max_time < delay_ns)
	{
		/* time error */
		j = 3;
		cssck_dat[3].cssck = 15;

	}else{

		for(i = 0; i < 4; i ++){
			cssck_scale_value[i] = delay_ns/(sys_time * pcssck[i]);
			cssck_dat[i] = cssck_calculate(cssck_scale_value[i]);
			cal_delay_time[i] = sys_time * pcssck[i] * cssck_dat[i].value;

			if(cal_delay_time[i] > delay_ns){
				cal_diff_time[i] = cal_delay_time[i] - delay_ns;
			}else{
				cal_diff_time[i] = delay_ns - cal_delay_time[i];
			}
		}

		for(i = 0; i < 4; i ++){
			if(cal_diff_time[i] < cal_min_time){
				cal_min_time = cal_diff_time[i];
				j = i;
			}
		}
	}/* check max_time end */

	return cssck_dat[j].cssck | (j << 16);
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

int32_t mpc5744_spi_setup(struct spi_device *dev){
	volatile struct SPI_tag *p_DSPI;				//base pointer
	SPI_MODE_CTAR_tag preset_CTAR = {0};
	SPI_MODE_CTAR_tag current_CTAR = {0};
	uint32_t spi_speed = 0;
	uint32_t spi_cfg_reg = 0;
	uint8_t bus_num = 0;
	uint32_t br_pbr = 0;
	uint32_t tcsc = 0;
	uint32_t tasc = 0;
	uint32_t tdt = 0;
	int32_t ret = 0;

	/* check input parameter */
	if(dev != NULL){
		spi_speed = dev->actual_speed_hz;
		spi_cfg_reg = dev->cfg_select;
		bus_num = dev->master->bus_num;
		if(bus_num < MPC5744_SPI_NUM){
			p_DSPI = p_spi_base_addr[bus_num];
			if(spi_cfg_reg > 4){
				spi_cfg_reg = 0;
			}
			preset_CTAR.R = p_DSPI->MODE.CTAR[spi_cfg_reg].R;

			current_CTAR.B.CPHA = dev->dev_CPHA;
			current_CTAR.B.CPOL = dev->dev_CPOL;
			current_CTAR.B.LSBFE = dev->dev_LSB_first;
			current_CTAR.B.FMSZ = dev->bits_per_word - 1;
			current_CTAR.B.DBR = 0;
			/* calculate tcsc, start transmit delay time  */
			tcsc = spi_delay_calculate(DSPI_CLK, dev->start_delay_nsecs);
			current_CTAR.B.PCSSCK = (tcsc & HIGH_WORD_MASK)>>16;		//tcsc
			current_CTAR.B.CSSCK = tcsc & LOW_WORD_MASK;
			/* calculate tasc, end transmit delay time */
		    tasc = spi_delay_calculate(DSPI_CLK, dev->end_delay_nsecs);
		    current_CTAR.B.PASC = (tasc & HIGH_WORD_MASK)>>16;			//tasc
		    current_CTAR.B.ASC  = tcsc & LOW_WORD_MASK;
		    /* calculate tdt, next frame delay time */
		    tdt = spi_delay_calculate(DSPI_CLK, dev->next_frame_delay_nsecs);
		    current_CTAR.B.PDT = (tdt & HIGH_WORD_MASK)>>16;			//tdt
		    current_CTAR.B.DT = tdt & LOW_WORD_MASK;;
		    /* calculate spi baudrate */
		    br_pbr = spi_bitrate_calculate(DSPI_CLK, spi_speed);
		    current_CTAR.B.PBR = br_pbr & LOW_WORD_MASK;
		    current_CTAR.B.BR = (br_pbr & HIGH_WORD_MASK) >> 16;

		    if(current_CTAR.R != preset_CTAR.R )
		    {
		    	p_DSPI->MODE.CTAR[spi_cfg_reg].R = current_CTAR.R;
		    }
		}else{
			/* bus_num error */
			ret = -1;
		}
	}else{
		/* parameter */
		ret = -1;
	}
    return ret;
}



