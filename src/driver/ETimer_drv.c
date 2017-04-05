/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ10ÈÕ
* Compiler   : gcc
* FILE NAME  : PWM_drv.c
*
* DESCRIPTION:
*
*/

#include "derivative.h"
#include "SIUL.h"
#include "ETimer_drv.h"

#define ETIMER0_ADDR	(volatile struct ETIMER_tag *)0xFBC20000UL
#define ETIMER1_ADDR	(volatile struct ETIMER_tag *)0xFFC24000UL
#define ETIMER2_ADDR	(volatile struct ETIMER_tag *)0xFBC28000UL


struct etimer_set{
	uint32_t count_mode:3;		/* 000: no operation */
								/* 001: count rising edges of primary source */
								/* 010: count rising and falling edges of primary source */
								/* 011: count rising edges of primary source while secondary input high active */
								/* 100: quadrature count mode */
								/* 101: count primary source rising edges, secondary source specifies direction */
								/* 110: edge of secondary source triggers parimary count till compare */
								/* 111: cascaded counter mode */
	uint32_t count1_source:5;
	uint32_t count2_source:5;

	uint32_t reload_mode:2;		/* 00: Do not reload the counter on a capture event */
								/* 01: reload the counter on capture 1 event */
								/* 10: reload the counter on capture 2 event */
								/* 11: reload the counter on both a capture 1 event and 2 envetn */

	uint32_t count_direction:1;	/* 0: count up, 1: count down */

	uint32_t compare_mode:2;	/* 00: COMP1 and COMP2 are counting up */
                            	/* 01: COMP1 counting down, COMP2 counting up */
	                            /* 10: COMP1 counting up, COMP2 counting down */
                                /* 11: COMP1 counting down, COMP2 counting down */

	uint32_t preload1_mode:3;	/* 000: never preload */
								/* 001: reserved */
								/* 010: load COMP1 with CMPLD1, successful compare with COMP1.*/
								/* 011: load COMP1 with CMPLD1, successful compare with COMP2.*/
	                            /* 100: load COMP1 with CMPLD2, successful compare with COMP1.*/
	 	 	 	 	 	 	 	/* 101: load COMP1 with CMPLD2, successful compare with COMP2.*/
	                            /* 110: load CNTR with CMPLD1, successful compare with COMP1.*/
	                            /* 111: load CNTR with CMPLD1, successful compare with COMP2.*/
	uint32_t preload2_mode:3;

	uint32_t capture1_mode:2;	/* 00: Disable, */
								/* 01: capture falling edges, */
								/* 10: capture rising edges, */
								/* 11: capture any edge */
	uint32_t capture2_mode:2;

	uint32_t count_once:1;		/* 0: count repeatedly */
								/* 1: count until compare then stop */
	uint32_t capture_one_shot:1;	/* 0: free running mde */
									/* 1: one shot mode */
	uint32_t count1_polarity:1;	/* 0: true polarity */
								/* 1: inverted polarity */
	uint32_t count2_polarity:1;

	uint16_t compare1;
	uint16_t compare2;
	uint16_t preload_compare1;
	uint16_t preload_compare2;

	uint16_t interrupt_mask;

	uint32_t clock_freq;
	uint32_t prescaler;
};


struct etimer_dat{
	uint32_t freq;
	uint32_t period;
	uint32_t duty;
};


static volatile struct ETIMER_tag *ETIMER[ETIMER_NUM] = {ETIMER0_ADDR, ETIMER1_ADDR, ETIMER2_ADDR};

static struct etimer_dat etimer[ETIMER_NUM][ETIMER_CH_NUM];

static struct etimer_set cfg_dat[ETIMER_NUM][ETIMER_CH_NUM];

static void eTimer_gpio_init(uint8_t timer, uint8_t channel){

	switch(timer){
	case 0:
		switch(channel){
		case 0:
			SIUL_DigitalInput(SIUL_PD10, 59, SIUL_INPUT | SIUL_GPIO, SIUL_ALT1);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			SIUL_DigitalInput(SIUL_PC11, 63, SIUL_INPUT | SIUL_ALT1, SIUL_ALT4);
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	case 1:
		switch(channel){
		case 0:
			break;
		case 1:
			break;
		case 2:
			SIUL_DigitalInput(SIUL_PD1, 67, SIUL_INPUT | SIUL_ALT2, SIUL_ALT3);
			break;
		case 3:
			break;
		case 4:
			SIUL_DigitalInput(SIUL_PD8, 69, SIUL_INPUT | SIUL_ALT2, SIUL_ALT3);
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	case 2:
		switch(channel){
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			SIUL_DigitalInput(SIUL_PJ9, 76, SIUL_INPUT | SIUL_ALT1, SIUL_ALT4);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

static void eTimer_int_init(uint8_t timer){

	switch(timer){
	case 0:
	    INTC_0.PSR[611].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_0 */
	    INTC_0.PSR[612].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_0 */
	    INTC_0.PSR[613].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_0 */
	    INTC_0.PSR[614].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_0 */
	    INTC_0.PSR[615].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_0 */
	    INTC_0.PSR[616].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_0 */
		break;
	case 1:
	    INTC_0.PSR[622].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_1 */
	    INTC_0.PSR[623].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_1 */
	    INTC_0.PSR[624].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_1 */
	    INTC_0.PSR[625].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_1 */
	    INTC_0.PSR[626].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_1 */
	    INTC_0.PSR[627].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_1 */
		break;
	case 2:
	    INTC_0.PSR[633].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_2 */
	    INTC_0.PSR[634].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_2 */
	    INTC_0.PSR[635].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_2 */
	    INTC_0.PSR[636].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_2 */
	    INTC_0.PSR[637].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_2 */
	    INTC_0.PSR[638].B.PRIN = ETIMER_INT_PRIORITY;		/* TC0IR eTimer_2 */
		break;
	default:
		break;
	}

}



static void eTimer_paramter_init(uint8_t timer, uint8_t channel){
		switch(timer){
		case 0:
			switch(channel){
			case 0:
				/* configure timer0 channel 0 */
				cfg_dat[0][0].clock_freq = ETIMER_CLOCK_FREQ;	/* 10Mhz */
				cfg_dat[0][0].prescaler = 128;

				cfg_dat[0][0].count_mode = CNT1_RISING_EDGE;
				cfg_dat[0][0].count1_source = CLK_DIV_128;
				cfg_dat[0][0].count2_source = CNT_0_INPUT;
				cfg_dat[0][0].count_direction = 0;	/* count up */
				cfg_dat[0][0].count_once = 0;

				cfg_dat[0][0].compare_mode = COMP1_COMP2_CNT_UP;
				cfg_dat[0][0].capture1_mode = DISABLE;
				cfg_dat[0][0].capture2_mode = CAPT_RISING_EDGE;
				cfg_dat[0][0].capture_one_shot = 0;

				cfg_dat[0][0].preload1_mode = 0;
				cfg_dat[0][0].preload2_mode = 0;
				cfg_dat[0][0].reload_mode = RELOAD_ON_CAPT2_EVENT;
				cfg_dat[0][0].compare1 = 0xFFFF;
				cfg_dat[0][0].compare2 = 0xFFFF;
				cfg_dat[0][0].preload_compare1 = 0xFFFF;
				cfg_dat[0][0].preload_compare2 = 0xFFFF;

				cfg_dat[0][0].count1_polarity = 0;	/* true polarity */
				cfg_dat[0][0].count2_polarity = 0;	/* true polarity */

				/*ICF2DE ICF1DE CMPLD2DE CMPLD1DE 0     0      WDFIE  RCFIE */
				/*ICF2IE ICF1IE IEHFIE   IELFIE   TOFIE TCF2IE TCF1IE TCFIE */
				cfg_dat[0][0].interrupt_mask = 0x0088;
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				/* configure timer0 channel 4 */
				cfg_dat[0][4].clock_freq = ETIMER_CLOCK_FREQ;	/* 10Mhz */
				cfg_dat[0][4].prescaler = 128;

				cfg_dat[0][4].count_mode = CNT1_RISING_EDGE;
				cfg_dat[0][4].count1_source = CLK_DIV_128;
				cfg_dat[0][4].count2_source = CNT_4_INPUT;
				cfg_dat[0][4].count_direction = 0;	/* count up */
				cfg_dat[0][4].count_once = 0;

				cfg_dat[0][4].compare_mode = COMP1_COMP2_CNT_UP;
				cfg_dat[0][4].capture1_mode = DISABLE;
				cfg_dat[0][4].capture2_mode = CAPT_RISING_EDGE;
				cfg_dat[0][4].capture_one_shot = 0;

				cfg_dat[0][4].preload1_mode = 0;
				cfg_dat[0][4].preload2_mode = 0;
				cfg_dat[0][4].reload_mode = RELOAD_ON_CAPT2_EVENT;
				cfg_dat[0][4].compare1 = 0xFFFF;
				cfg_dat[0][4].compare2 = 0xFFFF;
				cfg_dat[0][4].preload_compare1 = 0xFFFF;
				cfg_dat[0][4].preload_compare2 = 0xFFFF;

				cfg_dat[0][4].count1_polarity = 0;	/* true polarity */
				cfg_dat[0][4].count2_polarity = 1;	/* inverted polarity */

				/*ICF2DE ICF1DE CMPLD2DE CMPLD1DE 0     0      WDFIE  RCFIE */
				/*ICF2IE ICF1IE IEHFIE   IELFIE   TOFIE TCF2IE TCF1IE TCFIE */
				cfg_dat[0][4].interrupt_mask = 0x0088;
				break;
			case 5:
				break;
			default:
				break;
			}
			break;
		case 1:
			switch(channel){
			case 0:
				break;
			case 1:
				break;
			case 2:
				/* configure timer1 channel 2 */
				cfg_dat[1][2].clock_freq = ETIMER_CLOCK_FREQ;	/* 10Mhz */
				cfg_dat[1][2].prescaler = 128;

				cfg_dat[1][2].count_mode = CNT1_RISING_EDGE;
				cfg_dat[1][2].count1_source = CLK_DIV_128;
				cfg_dat[1][2].count2_source = CNT_2_INPUT;
				cfg_dat[1][2].count_direction = 0;	/* count up */
				cfg_dat[1][2].count_once = 0;

				cfg_dat[1][2].compare_mode = COMP1_COMP2_CNT_UP;
				cfg_dat[1][2].capture1_mode = DISABLE;
				cfg_dat[1][2].capture2_mode = CAPT_RISING_EDGE;
				cfg_dat[1][2].capture_one_shot = 0;

				cfg_dat[1][2].preload1_mode = 0;
				cfg_dat[1][2].preload2_mode = 0;
				cfg_dat[1][2].reload_mode = RELOAD_ON_CAPT2_EVENT;
				cfg_dat[1][2].compare1 = 0xFFFF;
				cfg_dat[1][2].compare2 = 0xFFFF;
				cfg_dat[1][2].preload_compare1 = 0xFFFF;
				cfg_dat[1][2].preload_compare2 = 0xFFFF;

				cfg_dat[1][2].count1_polarity = 0;	/* true polarity */
				cfg_dat[1][2].count2_polarity = 0;	/* true polarity */

				/*ICF2DE ICF1DE CMPLD2DE CMPLD1DE 0     0      WDFIE  RCFIE */
				/*ICF2IE ICF1IE IEHFIE   IELFIE   TOFIE TCF2IE TCF1IE TCFIE */
				cfg_dat[1][2].interrupt_mask = 0x0088;
				break;
			case 3:
				break;
			case 4:
				/* configure timer1 channel 4 */
				cfg_dat[1][4].clock_freq = ETIMER_CLOCK_FREQ;	/* 10Mhz */
				cfg_dat[1][4].prescaler = 128;

				cfg_dat[1][4].count_mode = CNT1_RISING_EDGE;
				cfg_dat[1][4].count1_source = CLK_DIV_128;
				cfg_dat[1][4].count2_source = CNT_4_INPUT;
				cfg_dat[1][4].count_direction = 0;	/* count up */
				cfg_dat[1][4].count_once = 0;

				cfg_dat[1][4].compare_mode = COMP1_COMP2_CNT_UP;
				cfg_dat[1][4].capture1_mode = DISABLE;
				cfg_dat[1][4].capture2_mode = CAPT_RISING_EDGE;
				cfg_dat[1][4].capture_one_shot = 0;

				cfg_dat[1][4].preload1_mode = 0;
				cfg_dat[1][4].preload2_mode = 0;
				cfg_dat[1][4].reload_mode = RELOAD_ON_CAPT2_EVENT;
				cfg_dat[1][4].compare1 = 0xFFFF;
				cfg_dat[1][4].compare2 = 0xFFFF;
				cfg_dat[1][4].preload_compare1 = 0xFFFF;
				cfg_dat[1][4].preload_compare2 = 0xFFFF;

				cfg_dat[1][4].count1_polarity = 0;	/* true polarity */
				cfg_dat[1][4].count2_polarity = 0;	/* true polarity */

				/*ICF2DE ICF1DE CMPLD2DE CMPLD1DE 0     0      WDFIE  RCFIE */
				/*ICF2IE ICF1IE IEHFIE   IELFIE   TOFIE TCF2IE TCF1IE TCFIE */
				cfg_dat[1][4].interrupt_mask = 0x0088;
				break;
			case 5:
				break;
			default:
				break;
			}
			break;
		case 2:
			switch(channel){
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				/* configure timer2 channel 5 */
				cfg_dat[2][5].clock_freq = ETIMER_CLOCK_FREQ;	/* 10Mhz */
				cfg_dat[2][5].prescaler = 128;

				cfg_dat[2][5].count_mode = CNT1_RISING_EDGE;
				cfg_dat[2][5].count1_source = CLK_DIV_128;
				cfg_dat[2][5].count2_source = CNT_5_INPUT;
				cfg_dat[2][5].count_direction = 0;	/* count up */
				cfg_dat[2][5].count_once = 0;

				cfg_dat[2][5].compare_mode = COMP1_COMP2_CNT_UP;
				cfg_dat[2][5].capture1_mode = DISABLE;
				cfg_dat[2][5].capture2_mode = CAPT_RISING_EDGE;
				cfg_dat[2][5].capture_one_shot = 0;

				cfg_dat[2][5].preload1_mode = 0;
				cfg_dat[2][5].preload2_mode = 0;
				cfg_dat[2][5].reload_mode = RELOAD_ON_CAPT2_EVENT;
				cfg_dat[2][5].compare1 = 0xFFFF;
				cfg_dat[2][5].compare2 = 0xFFFF;
				cfg_dat[2][5].preload_compare1 = 0xFFFF;
				cfg_dat[2][5].preload_compare2 = 0xFFFF;

				cfg_dat[2][5].count1_polarity = 0;	/* true polarity */
				cfg_dat[2][5].count2_polarity = 0;	/* true polarity */

				/*ICF2DE ICF1DE CMPLD2DE CMPLD1DE 0     0      WDFIE  RCFIE */
				/*ICF2IE ICF1IE IEHFIE   IELFIE   TOFIE TCF2IE TCF1IE TCFIE */
				cfg_dat[2][5].interrupt_mask = 0x0088;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
}

/*******************************************************************************
Function Name : eTimer_Init
Engineer      : PetrS
Date          : Mar-20-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : eTimer0 channel 0, 1 init for capture features
Issues        : NONE
*******************************************************************************/
void eTimer_init(uint8_t timer, uint8_t channel){
	uint16_t channel_enable = 0;

	if((timer < 3) && (channel < 6)){

		eTimer_gpio_init(timer, channel);	/* eTimer GPIO pin init */
		eTimer_int_init(timer);
		eTimer_paramter_init(timer, channel);

		channel_enable = ETIMER[timer]->ENBL.R;
		ETIMER[timer]->ENBL.R = 0;// disable Timer n channels
		ETIMER[timer]->CH[channel].CTRL1.B.CNTMODE = cfg_dat[timer][channel].count_mode;
		ETIMER[timer]->CH[channel].CTRL1.B.DIR = cfg_dat[timer][channel].count_direction;
		ETIMER[timer]->CH[channel].CTRL1.B.ONCE = cfg_dat[timer][channel].count_once;
		ETIMER[timer]->CH[channel].CTRL1.B.PRISRC = cfg_dat[timer][channel].count1_source;
		ETIMER[timer]->CH[channel].CTRL1.B.SECSRC = cfg_dat[timer][channel].count2_source;

		ETIMER[timer]->CH[channel].CTRL2.R = 0;
		ETIMER[timer]->CH[channel].CTRL2.B.PIPS = cfg_dat[timer][channel].count1_polarity;
		ETIMER[timer]->CH[channel].CTRL2.B.SIPS = cfg_dat[timer][channel].count2_polarity;

		ETIMER[timer]->CH[channel].CTRL3.B.ROC = cfg_dat[timer][channel].reload_mode;

		ETIMER[timer]->CH[channel].CCCTRL.B.CLC1 = cfg_dat[timer][channel].preload1_mode;
		ETIMER[timer]->CH[channel].CCCTRL.B.CLC2 = cfg_dat[timer][channel].preload2_mode;
		ETIMER[timer]->CH[channel].CCCTRL.B.CMPMODE = cfg_dat[timer][channel].compare_mode;
		ETIMER[timer]->CH[channel].CCCTRL.B.CPT1MODE = cfg_dat[timer][channel].capture1_mode;
		ETIMER[timer]->CH[channel].CCCTRL.B.CPT2MODE = cfg_dat[timer][channel].capture2_mode;
		ETIMER[timer]->CH[channel].CCCTRL.B.ONESHOT = cfg_dat[timer][channel].capture_one_shot;
		ETIMER[timer]->CH[channel].CCCTRL.B.CFWM = 2;

		ETIMER[timer]->CH[channel].COMP1.R = cfg_dat[timer][channel].compare1;
		ETIMER[timer]->CH[channel].COMP2.R = cfg_dat[timer][channel].compare2;

		ETIMER[timer]->CH[channel].CMPLD1.R = cfg_dat[timer][channel].preload_compare1;
		ETIMER[timer]->CH[channel].CMPLD2.R = cfg_dat[timer][channel].preload_compare2;

		ETIMER[timer]->CH[channel].INTDMA.R = cfg_dat[timer][channel].interrupt_mask;

		channel_enable |= 1 << channel;

		ETIMER[timer]->ENBL.R = channel_enable;			// Enable Timer0 channel 1
	}
}

void eTimer_start(uint8_t timer, uint8_t channel){
	ETIMER[timer]->CH[channel].CCCTRL.B.ARM = 1;		// starts the input capture process
}

void eTimer_stop(uint8_t timer, uint8_t channel){
	ETIMER[timer]->CH[channel].CCCTRL.B.ARM = 0;		// starts the input capture process
}

uint32_t eTimer_get_freq(uint8_t timer, uint8_t channel){

	return etimer[timer][channel].freq;
}

uint32_t eTimer_get_period(uint8_t timer, uint8_t channel){

	return etimer[timer][channel].period;
}

uint32_t eTimer_get_duty(uint8_t timer, uint8_t channel){

	return etimer[timer][channel].duty;
}

#define ICF2_MASK	0x0080

static void eTimer_intterrupt_server(uint8_t timer, uint8_t channel){
	static uint32_t flag[3][6] = {{0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};

	uint32_t capture2[2] = {0,0};
	uint32_t counts = 0;

	uint32_t freq = 0;
	uint32_t period = 0;
	uint32_t duty = 0;
	uint32_t pulseH = 0;
	uint32_t clock = 0;
	uint32_t prescale = 0;

	if(ICF2_MASK & ETIMER[timer]->CH[channel].STS.R){

		capture2[0] = ETIMER[timer]->CH[channel].CAPT2.R;
		capture2[1] = ETIMER[timer]->CH[channel].CAPT2.R;

		clock = cfg_dat[timer][channel].clock_freq;
		prescale = cfg_dat[timer][channel].prescaler;

		if(flag[timer][channel] == 0){
			ETIMER[timer]->CH[channel].CTRL1.B.CNTMODE = CNT1_RISING_EDGE_WITH_CNT2_HING_INPUT;

			flag[timer][channel] = 1;
			counts = capture2[0];
			period = prescale*counts;
			freq = clock/period;

			etimer[timer][channel].period = (period+5)/10;
			etimer[timer][channel].freq = freq;

		}else if(flag[timer][channel] == 1){
			ETIMER[timer]->CH[channel].CTRL1.B.CNTMODE = CNT1_RISING_EDGE ;

			flag[timer][channel] = 0;
			counts = capture2[0];
			period = etimer[timer][channel].period;
			pulseH = prescale*counts*10;
			duty = (pulseH*100)/period;

			if(ETIMER[timer]->CH[channel].CTRL2.B.SIPS == 1){
				etimer[timer][channel].duty = 10000 - duty;
			}else{
				etimer[timer][channel].duty = duty;
			}

		}

	}else{
		/* count timeout */
		etimer[timer][channel].period = 0;
		etimer[timer][channel].freq = 0;
		etimer[timer][channel].duty = 0;

		eTimer_stop(timer, channel);		/* stop the input capture process */

		flag[timer][channel] = 0;
		ETIMER[timer]->CH[channel].CTRL1.B.CNTMODE = CNT1_RISING_EDGE ;
	}

	ETIMER[timer]->CH[channel].STS.R = 0x00FF;		// clear eTimer0 channel 1's capture1/2 flags
}

void eTimer0_ch0_irq(void){
	eTimer_intterrupt_server(0, 0);
}

void eTimer0_ch1_irq(void){
	eTimer_intterrupt_server(0, 1);
}

void eTimer0_ch2_irq(void){
	eTimer_intterrupt_server(0, 2);
}

void eTimer0_ch3_irq(void){
	eTimer_intterrupt_server(0, 3);
}

void eTimer0_ch4_irq(void){
	eTimer_intterrupt_server(0, 4);
}

void eTimer0_ch5_irq(void){
	eTimer_intterrupt_server(0, 5);
}

void eTimer1_ch0_irq(void){
	eTimer_intterrupt_server(1, 0);
}

void eTimer1_ch1_irq(void){
	eTimer_intterrupt_server(1, 1);
}

void eTimer1_ch2_irq(void){
	eTimer_intterrupt_server(1, 2);
}

void eTimer1_ch3_irq(void){
	eTimer_intterrupt_server(1, 3);
}

void eTimer1_ch4_irq(void){
	eTimer_intterrupt_server(1, 4);
}

void eTimer1_ch5_irq(void){
	eTimer_intterrupt_server(1, 5);
}


void eTimer2_ch0_irq(void){
	eTimer_intterrupt_server(2, 0);
}

void eTimer2_ch1_irq(void){
	eTimer_intterrupt_server(2, 1);
}

void eTimer2_ch2_irq(void){
	eTimer_intterrupt_server(2, 2);
}

void eTimer2_ch3_irq(void){
	eTimer_intterrupt_server(2, 3);
}

void eTimer2_ch4_irq(void){
	eTimer_intterrupt_server(2, 4);
}

void eTimer2_ch5_irq(void){
	eTimer_intterrupt_server(2, 5);
}


void eTimer_init_all(void){

	eTimer_init(0, 0);
	eTimer_init(0, 4);

	eTimer_init(1, 2);
	eTimer_init(1, 4);

	eTimer_init(2, 5);
}

void eTimer_start_all(void){

	eTimer_start(0, 0);
	eTimer_start(0, 4);
	eTimer_start(1, 2);
	eTimer_start(1, 4);
	eTimer_start(2, 5);

}





