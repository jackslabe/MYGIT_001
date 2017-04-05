/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "ADC_drv.h"
#include "ADC_interface.h"

#include "SPI_bus_drv.h"
#include "GPIO_interface.h"
#include "PIT_drv.h"
#include "MPC33972_interface.h"
#include "STM_drv.h"
#include "CAN_drv.h"
#include "CAN_bus.h"
#include "ETimer_drv.h"

#include "MPC33908_interface.h"

#include "LTC6811_interface.h"

#include "EEPROM_interface.h"

#include "M25PE40_interface.h"

#include "EXTEND_FLASH_interface.h"

#include "Test.h"

#include "BMS.h"
#include "BmuInterface.h"
#include "VcuInterface.h"
#include "ScanIOInterface.h"

extern void xcptn_xmpl(void);

void FCCU_SetProtocol(uint32_t protocol){
    // Enter FCCU Configuration Mode
    FCCU.TRANS_LOCK.R = 0xBC;
    FCCU.CTRLK.R = FCCU_CTRLK_OP1;
    FCCU.CTRL.R = FCCU_OPR_SET_CONFIG;
    while(FCCU.CTRL.B.OPS != FCCU_OPS_SUCCESS);

    // Disable all faults
    FCCU.NCF_E[0].R = 0x00000000;
    FCCU.NCF_E[1].R = 0x00000000;
    FCCU.NCF_E[2].R = 0x00000000;
	//SW reaction
    FCCU.NCF_CFG[0].R = 0xFFFFFFFF;
    FCCU.NCF_CFG[1].R = 0xFFFFFFFF;
    FCCU.NCF_CFG[2].R = 0xFFFFFFFF;

    FCCU.CFG.R = 0x01000000;
    FCCU.CFG.B.FOM = protocol;

    // Exit FCCU Configuration Mode
    FCCU.CTRLK.R = FCCU_KEY_NORMAL ;
    FCCU.CTRL.R = FCCU_OPR_SET_NORMAL;
    while(FCCU.CTRL.B.OPS != FCCU_OPS_SUCCESS);

}

void MC_MODE_INIT(void)
{
    /* Enable All Modes */
    MC_ME.ME.R = 0x000005E2;

    /* Peripheral ON in every run mode */
    MC_ME.RUN_PC[0].R = 0x000000FE;

    /******************** Configure XOSC for DRUN **********************/
    /* Enable EXT OSC First */
    XOSC.CTL.B.OSCM = 0x1;                          /* Change OSC mode to LCP (Loop Controlled Pierce Mode) */
    XOSC.CTL.B.EOCV = 0x80;                         /* Set the End of Count Value for when to check stabilization. */

    /* Enable XOSC in DRUN mode and select as SYS_CLK */
    MC_ME.DRUN_MC.R = 0x00130031;

    /* RE enter the DRUM mode, to update the configuration */
    MC_ME.MCTL.R = 0x30005AF0;                      /* Mode & Key */
    MC_ME.MCTL.R = 0x3000A50F;                      /* Mode & Key inverted */
    while(MC_ME.GS.B.S_MTRANS == 1);                /* Wait for mode entry to complete */
    while(MC_ME.GS.B.S_CURRENT_MODE != 0x3);        /* Check DRUN mode has been entered */
    while(!MC_ME.GS.B.S_XOSC);                      /* Wait for clock to stabilise */

    /******************** PLL0, PLL1 **********************/

    /* Route XOSC to PLL1 */
    MC_CGM.AC4_SC.B.SELCTL = 1;

    /* Route XOSC PLL0 */
    MC_CGM.AC3_SC.B.SELCTL = 1;

    /*
        Configure PLL0 Dividers for 160 MHz
        fPLL0_VCO = (fPLL0_ref x PLL0DV[MFD] x 2)/PLL0DV[PREDIV]
                  = 40MHz x 8 x 2 / 1 =  640 MHz

        fPLL0_PHI  = fPLL0_ref x PLL0DV[MFD] / (PLL0DV[PREDIV] x PLL0DV[RFDPHI]
                   = 40MHz x 8 / (1 x 2) = 160 MHz

        fPLL0_PHI1 = fPLL0_ref x PLL0DV[MFD] / (PLL0DV[PREDIV] x PLL0DV[RFDPHI1])
                   = 40MHz x 8 / (1 x 8) = 40 MHz
    */
    PLLDIG.PLL0DV.B.RFDPHI1 = 8;
    PLLDIG.PLL0DV.B.RFDPHI = 2;
    PLLDIG.PLL0DV.B.PREDIV = 1;
    PLLDIG.PLL0DV.B.MFD = 8;

    /*
        Configure PLL1 Dividers for 200 MHz
        fPLL1_VCO = fPLL1_REF x (PLL1DV[MFD] + PLL1FD[FRCDIV]/2^12)
                  = 40MHz x 20 + 0 = 800 MHz

        fPLL1_PHI = fPLL1_REF * ( (PLL1DV[MFD] + PLL1FD[FRCDIV]/2^12) / (2 x PLL1DV[RFDPHI]) )
                  = 40MHz x (20 + 0) / (2 x 2) = 200 MHz


    */
    PLLDIG.PLL1DV.B.RFDPHI = 2;
    PLLDIG.PLL1DV.B.MFD = 20;

    /* Enable PLL0/PLL1 in DRUN mode and set PLL1 as SYS_CLK */
    MC_ME.DRUN_MC.R = 0x001300F4;

    /******************** Configure Clock Dividers **********************/

    SIUL2.MSCR[22].R = 0x22800001;                  /* Configure CLK_OUT (B6) */
    //MC_CGM.AC6_SC.B.SELCTL = 0;                   /* source AC6 is internal RCOSC */
    //MC_CGM.AC6_SC.B.SELCTL = 2;                   /* source AC6 is PLL0 PHI */
    //MC_CGM.AC6_SC.B.SELCTL = 1;                   /* source AC6 is XOSC */
    MC_CGM.AC6_SC.B.SELCTL = 4;                     /* source AC6 is PLL1 PHI */
    MC_CGM.AC6_DC0.R = 0x80090000;                  /* Aux clock select 6 divider 0 --> div by 10 (CLK_OUT) */

    MC_CGM.AC0_SC.B.SELCTL = 2;                     /* source AC0 is PLL0 PHI */
    MC_CGM.AC0_DC0.R = 0x800F0000;                  /* Aux clock select 0 divider 0 --> div by 16 (MOTC_CLK) */
    MC_CGM.AC0_DC1.R = 0x80070000;                  /* Aux clock select 0 divider 1 --> div by 8 (SWG_CLK) */
    MC_CGM.AC0_DC2.R = 0x80010000;                  /* Aux clock select 0 divider 2 --> div by 2 (ADC_CLK) */

    MC_CGM.AC1_DC0.R = 0x80010000;                  /* Aux clock select 1 divider 0 --> div by 2 (FRAY_PLL_CLK) */
    MC_CGM.AC1_DC1.R = 0x80030000;                  /* Aux clock select 1 divider 1 --> div by 4 (SENT_CLK) */

    MC_CGM.AC2_DC0.R = 0x80030000;                  /* Aux clock select 2 divider 0 --> div by 4 (CAN_PLL_CLK) */

    MC_CGM.SC_DC0.R = 0x80030000;                   /* Sys clock select divider 0 --> div by 4 (PBRIDGEx_CLK) -> DSPI clock = 50MHz, PIT clock = 50 MHz */

    /******************** Start the core **********************/
    /* Main and checker cores running in RUN3:0, DRUN, SAFE, TEST modes */
    MC_ME.CCTL0.R = 0x00FE;

     /* Set PRAM controller WS to 1 since running SYS_CLK as PLL1 (200 MHz) */
    PRAMC.PRCR1.B.FT_DIS = 1;

    /******************** Perform mode change **********************/
    /* Mode change re-enter the DRUN mode, to start cores, clock tree & PLL1 */
    MC_ME.MCTL.R = 0x30005AF0;                  /* Mode & Key */
    MC_ME.MCTL.R = 0x3000A50F;                  /* Mode & Key inverted */

    while(MC_ME.GS.B.S_MTRANS == 1);            /* Wait for mode entry complete */
    while(MC_ME.GS.B.S_CURRENT_MODE != 0x3);    /* Check DRUN mode entered */

}



uint32_t timer_flag = 1;
void timer0_irq_handler(void){
	timer_flag = ~timer_flag;
	mpc33908_release_fs_out();
	mpc33908_feed_watchdog();
}


enum CAN_SIGNAL_LIST{
	AD_B1,
	AD_B2,
	AD_C1,
	AD_C2,
	AD_CC,
	AD_CC2,
	AD_CP,
	AD_CURRENT_01,
	AD_CURRENT_02,
	AD_CURRENT_11,
	AD_CURRENT_12,
	AD_DC12,
	AD_ISON,
	AD_ISOP,
	AD_VBATT,
	AD_VBUS,
	CP_DUTY,
	CP_FREQ,
	CP_VOLTAGE,
	ISO_N_CTRL,
	ISO_P_CTRL,
	K10_CTRL,
	K11_CTRL,
	K12_CTRL,
	K1_CTRL,
	K2_CTRL,
	K3_CTRL,
	K4_CTRL,
	K5_CTRL,
	K6_CTRL,
	K7_CTRL,
	K8_CTRL,
	K9_CTRL,
	LED1_CTRL,
	LED2_CTRL,
	MPC33908_CAN_WAKEUP,
	MPC33908_IO0_VOLTAGE,
	MPC33908_IO0_STATUS,
	MPC33908_IO0_WAKEUP,
	MPC33908_IO0_WAKEUP_CFG,
	MPC33908_IO1_VOLTAGE,
	MPC33908_IO1_STATUS,
	MPC33908_IO1_WAKEUP,
	MPC33908_IO1_WAKEUP_CFG,
	MPC33908_IO2_STATUS,
	MPC33908_IO3_STATUS,
	MPC33908_IO4_STATUS,
	MPC33908_IO4_WAKEUP,
	MPC33908_IO4_WAKEUP_CFG,
	MPC33908_IO5_STATUS,
	MPC33908_IO5_WAKEUP,
	MPC33908_IO5_WAKEUP_CFG,
	MPC33908_Temperature,
	MPC33908_VCC,
	MPC33908_Vref,
	MPC33972_SG0,
	MPC33972_SG1,
	MPC33972_SG2,
	MPC33972_SG3,
	MPC33972_SG4,
	MPC33972_SG5,
	MPC33972_SG6,
	MPC33972_SG7,
	MPC33972_SG8,
	MPC33972_SP0_CFG,
	MPC33972_SP1_CFG,
	MPC33972_SP2_CFG,
	MPC33972_SP3_CFG,
	MPC33972_SP4_CFG,
	MPC33972_SP5_CFG,
	MPC33972_SP6_CFG,
	MPC33972_SP7_CFG,
	PWM_DPI1_DUTY,
	PWM_DPI1_FREQ,
	PWM_DPI2_DUTY,
	PWM_DPI2_FREQ,
	PWM_DPI3_DUTY,
	PWM_DPI3_FREQ,
	PWM_DPI4_DUTY,
	PWM_DPI4_FREQ,
	S2_CTRL,
	SP0_STATUS,
	SP1_STATUS,
	SP2_STATUS,
	SP3_STATUS,
	SP4_STATUS,
	SP5_STATUS,
	SP6_STATUS,
	SP7_STATUS,
	SYS_INTERRUPT_TIME,
	SYS_TIME_RUN_TIME,
	VQN0_CH0_CURRENT,
	VQN0_CH1_CURRENT,
	VQN0_CH2_CURRENT,
	VQN0_CH3_CURRENT,
	VQN0_Temperature,
	VQN0_VCC,
	VQN1_CH0_CURRENT,
	VQN1_CH1_CURRENT,
	VQN1_CH2_CURRENT,
	VQN1_CH3_CURRENT,
	VQN1_Temperature,
	VQN1_VCC,
	VQN2_CH0_CURRENT,
	VQN2_CH1_CURRENT,
	VQN2_CH2_CURRENT,
	VQN2_CH3_CURRENT,
	VQN2_Temperature,
	VQN2_VCC,
	SIGNAL_NUM
};

struct SignalCfg{
	uint16_t signal_num;
	uint8_t start_bit;
	uint8_t bit_length;
};

#define NULL ((void *)0)

struct SignalCfg can_signal[SIGNAL_NUM];

void can_signal_init(void){
	can_signal[AD_B1].bit_length = 16;
	can_signal[AD_B1].start_bit = 0;
	can_signal[AD_B1].signal_num = AD_B1;

	can_signal[AD_B2].bit_length = 16;
	can_signal[AD_B2].start_bit = 16;
	can_signal[AD_B2].signal_num = AD_B2;

	can_signal[AD_C1].bit_length = 16;
	can_signal[AD_C1].start_bit = 32;
	can_signal[AD_C1].signal_num = AD_C1;

	can_signal[AD_C2].bit_length = 16;
	can_signal[AD_C2].start_bit = 48;
	can_signal[AD_C2].signal_num = AD_C2;

	can_signal[AD_CC].bit_length = 16;
	can_signal[AD_CC].start_bit = 32;
	can_signal[AD_CC].signal_num = AD_CC;

	can_signal[AD_CC2].bit_length = 16;
	can_signal[AD_CC2].start_bit = 0;
	can_signal[AD_CC2].signal_num = AD_CC2;

	can_signal[AD_CP].bit_length = 16;
	can_signal[AD_CP].start_bit = 16;
	can_signal[AD_CP].signal_num = AD_CP;

	can_signal[AD_CURRENT_01].bit_length = 16;
	can_signal[AD_CURRENT_01].start_bit = 0;
	can_signal[AD_CURRENT_01].signal_num = AD_CURRENT_01;

	can_signal[AD_CURRENT_02].bit_length = 16;
	can_signal[AD_CURRENT_02].start_bit = 16;
	can_signal[AD_CURRENT_02].signal_num = AD_CURRENT_02;

	can_signal[AD_CURRENT_11].bit_length = 16;
	can_signal[AD_CURRENT_11].start_bit = 32;
	can_signal[AD_CURRENT_11].signal_num = AD_CURRENT_11;

	can_signal[AD_CURRENT_12].bit_length = 16;
	can_signal[AD_CURRENT_12].start_bit = 48;
	can_signal[AD_CURRENT_12].signal_num = AD_CURRENT_12;

	can_signal[AD_DC12].bit_length = 16;
	can_signal[AD_DC12].start_bit = 48;
	can_signal[AD_DC12].signal_num = AD_DC12;

	can_signal[AD_ISON].bit_length = 16;
	can_signal[AD_ISON].start_bit = 48;
	can_signal[AD_ISON].signal_num = AD_ISON;

	can_signal[AD_ISOP].bit_length = 16;
	can_signal[AD_ISOP].start_bit = 32;
	can_signal[AD_ISOP].signal_num = AD_ISOP;

	can_signal[AD_VBATT].bit_length = 16;
	can_signal[AD_VBATT].start_bit = 16;
	can_signal[AD_VBATT].signal_num = AD_VBATT;

	can_signal[AD_VBUS].bit_length = 16;
	can_signal[AD_VBUS].start_bit = 0;
	can_signal[AD_VBUS].signal_num = AD_VBUS;

	can_signal[CP_DUTY].bit_length = 16;
	can_signal[CP_DUTY].start_bit = 16;
	can_signal[CP_DUTY].signal_num = CP_DUTY;

	can_signal[CP_FREQ].bit_length = 16;
	can_signal[CP_FREQ].start_bit = 0;
	can_signal[CP_FREQ].signal_num = CP_FREQ;

	can_signal[CP_VOLTAGE].bit_length = 16;
	can_signal[CP_VOLTAGE].start_bit = 32;
	can_signal[CP_VOLTAGE].signal_num = CP_VOLTAGE;

	can_signal[ISO_N_CTRL].bit_length = 1;
	can_signal[ISO_N_CTRL].start_bit = 25;
	can_signal[ISO_N_CTRL].signal_num = ISO_N_CTRL;

	can_signal[ISO_P_CTRL].bit_length = 1;
	can_signal[ISO_P_CTRL].start_bit = 24;
	can_signal[ISO_P_CTRL].signal_num = ISO_P_CTRL;

	can_signal[K10_CTRL].bit_length = 1;
	can_signal[K10_CTRL].start_bit = 9;
	can_signal[K10_CTRL].signal_num = K10_CTRL;


	can_signal[K11_CTRL].bit_length = 1;
	can_signal[K11_CTRL].start_bit = 10;
	can_signal[K11_CTRL].signal_num = K11_CTRL;


	can_signal[K12_CTRL].bit_length = 1;
	can_signal[K12_CTRL].start_bit = 11;
	can_signal[K12_CTRL].signal_num = K12_CTRL;

	can_signal[K1_CTRL].bit_length = 1;
	can_signal[K1_CTRL].start_bit = 0;
	can_signal[K1_CTRL].signal_num = K1_CTRL;

	can_signal[K2_CTRL].bit_length = 1;
	can_signal[K2_CTRL].start_bit = 1;
	can_signal[K2_CTRL].signal_num = K2_CTRL;

	can_signal[K3_CTRL].bit_length = 1;
	can_signal[K3_CTRL].start_bit = 2;
	can_signal[K3_CTRL].signal_num = K3_CTRL;

	can_signal[K4_CTRL].bit_length = 1;
	can_signal[K4_CTRL].start_bit = 3;
	can_signal[K4_CTRL].signal_num = K4_CTRL;

	can_signal[K5_CTRL].bit_length = 1;
	can_signal[K5_CTRL].start_bit = 4;
	can_signal[K5_CTRL].signal_num = K5_CTRL;

	can_signal[K6_CTRL].bit_length = 1;
	can_signal[K6_CTRL].start_bit = 5;
	can_signal[K6_CTRL].signal_num = K6_CTRL;

	can_signal[K7_CTRL].bit_length = 1;
	can_signal[K7_CTRL].start_bit = 6;
	can_signal[K7_CTRL].signal_num = K7_CTRL;

	can_signal[K8_CTRL].bit_length = 1;
	can_signal[K8_CTRL].start_bit = 7;
	can_signal[K8_CTRL].signal_num = K8_CTRL;

	can_signal[K9_CTRL].bit_length = 1;
	can_signal[K9_CTRL].start_bit = 8;
	can_signal[K9_CTRL].signal_num = K9_CTRL;


	can_signal[LED1_CTRL].bit_length = 1;
	can_signal[LED1_CTRL].start_bit = 16;
	can_signal[LED1_CTRL].signal_num = LED1_CTRL;


	can_signal[LED2_CTRL].bit_length = 1;
	can_signal[LED2_CTRL].start_bit = 17;
	can_signal[LED2_CTRL].signal_num = LED2_CTRL;


	can_signal[MPC33908_CAN_WAKEUP].bit_length = 1;
	can_signal[MPC33908_CAN_WAKEUP].start_bit = 20;
	can_signal[MPC33908_CAN_WAKEUP].signal_num = MPC33908_CAN_WAKEUP;

	can_signal[MPC33908_IO0_VOLTAGE].bit_length = 16;
	can_signal[MPC33908_IO0_VOLTAGE].start_bit = 32;
	can_signal[MPC33908_IO0_VOLTAGE].signal_num = MPC33908_IO0_VOLTAGE;

	can_signal[MPC33908_IO0_STATUS].bit_length = 1;
	can_signal[MPC33908_IO0_STATUS].start_bit = 8;
	can_signal[MPC33908_IO0_STATUS].signal_num = MPC33908_IO0_STATUS;

	can_signal[MPC33908_IO0_WAKEUP].bit_length = 1;
	can_signal[MPC33908_IO0_WAKEUP].start_bit = 16;
	can_signal[MPC33908_IO0_WAKEUP].signal_num = MPC33908_IO0_WAKEUP;

	can_signal[MPC33908_IO0_WAKEUP_CFG].bit_length = 2;
	can_signal[MPC33908_IO0_WAKEUP_CFG].start_bit = 0;
	can_signal[MPC33908_IO0_WAKEUP_CFG].signal_num = MPC33908_IO0_WAKEUP_CFG;

	can_signal[MPC33908_IO1_VOLTAGE].bit_length = 16;
	can_signal[MPC33908_IO1_VOLTAGE].start_bit = 48;
	can_signal[MPC33908_IO1_VOLTAGE].signal_num = MPC33908_IO1_VOLTAGE;

	can_signal[MPC33908_IO1_STATUS].bit_length = 1;
	can_signal[MPC33908_IO1_STATUS].start_bit = 9;
	can_signal[MPC33908_IO1_STATUS].signal_num = AD_CURRENT_12;

	can_signal[MPC33908_IO1_WAKEUP].bit_length = 1;
	can_signal[MPC33908_IO1_WAKEUP].start_bit = 17;
	can_signal[MPC33908_IO1_WAKEUP].signal_num = MPC33908_IO1_WAKEUP;

	can_signal[MPC33908_IO1_WAKEUP_CFG].bit_length = 2;
	can_signal[MPC33908_IO1_WAKEUP_CFG].start_bit = 2;
	can_signal[MPC33908_IO1_WAKEUP_CFG].signal_num = MPC33908_IO1_WAKEUP_CFG;

	can_signal[MPC33908_IO2_STATUS].bit_length = 1;
	can_signal[MPC33908_IO2_STATUS].start_bit = 10;
	can_signal[MPC33908_IO2_STATUS].signal_num = MPC33908_IO2_STATUS;

	can_signal[MPC33908_IO3_STATUS].bit_length = 1;
	can_signal[MPC33908_IO3_STATUS].start_bit = 11;
	can_signal[MPC33908_IO3_STATUS].signal_num = MPC33908_IO3_STATUS;

	can_signal[MPC33908_IO4_STATUS].bit_length = 1;
	can_signal[MPC33908_IO4_STATUS].start_bit = 12;
	can_signal[MPC33908_IO4_STATUS].signal_num = MPC33908_IO4_STATUS;

	can_signal[MPC33908_IO4_WAKEUP].bit_length = 1;
	can_signal[MPC33908_IO4_WAKEUP].start_bit = 18;
	can_signal[MPC33908_IO4_WAKEUP].signal_num = MPC33908_IO4_WAKEUP;

	can_signal[MPC33908_IO4_WAKEUP_CFG].bit_length = 2;
	can_signal[MPC33908_IO4_WAKEUP_CFG].start_bit = 4;
	can_signal[MPC33908_IO4_WAKEUP_CFG].signal_num = MPC33908_IO4_WAKEUP_CFG;

	can_signal[MPC33908_IO5_STATUS].bit_length = 1;
	can_signal[MPC33908_IO5_STATUS].start_bit = 13;
	can_signal[MPC33908_IO5_STATUS].signal_num = MPC33908_IO5_STATUS;

	can_signal[MPC33908_IO5_WAKEUP].bit_length = 1;
	can_signal[MPC33908_IO5_WAKEUP].start_bit = 19;
	can_signal[MPC33908_IO5_WAKEUP].signal_num = MPC33908_IO5_WAKEUP;

	can_signal[MPC33908_IO5_WAKEUP_CFG].bit_length = 2;
	can_signal[MPC33908_IO5_WAKEUP_CFG].start_bit = 6;
	can_signal[MPC33908_IO5_WAKEUP_CFG].signal_num = MPC33908_IO5_WAKEUP_CFG;

	can_signal[MPC33908_Temperature].bit_length = 16;
	can_signal[MPC33908_Temperature].start_bit = 0;
	can_signal[MPC33908_Temperature].signal_num = MPC33908_Temperature;

	can_signal[MPC33908_VCC].bit_length = 16;
	can_signal[MPC33908_VCC].start_bit = 16;
	can_signal[MPC33908_VCC].signal_num = MPC33908_VCC;

	can_signal[MPC33908_Vref].bit_length = 16;
	can_signal[MPC33908_Vref].start_bit = 0;
	can_signal[MPC33908_Vref].signal_num = MPC33908_Vref;

	can_signal[MPC33972_SG0].bit_length = 16;
	can_signal[MPC33972_SG0].start_bit = 0;
	can_signal[MPC33972_SG0].signal_num = MPC33972_SG0;

	can_signal[MPC33972_SG1].bit_length = 16;
	can_signal[MPC33972_SG1].start_bit = 16;
	can_signal[MPC33972_SG1].signal_num = MPC33972_SG1;

	can_signal[MPC33972_SG2].bit_length = 16;
	can_signal[MPC33972_SG2].start_bit = 32;
	can_signal[MPC33972_SG2].signal_num = MPC33972_SG2;

	can_signal[MPC33972_SG3].bit_length = 16;
	can_signal[MPC33972_SG3].start_bit = 48;
	can_signal[MPC33972_SG3].signal_num = MPC33972_SG3;

	can_signal[MPC33972_SG4].bit_length = 16;
	can_signal[MPC33972_SG4].start_bit = 0;
	can_signal[MPC33972_SG4].signal_num = MPC33972_SG4;

	can_signal[MPC33972_SG5].bit_length = 16;
	can_signal[MPC33972_SG5].start_bit = 16;
	can_signal[MPC33972_SG5].signal_num = MPC33972_SG5;

	can_signal[MPC33972_SG6].bit_length = 16;
	can_signal[MPC33972_SG6].start_bit = 32;
	can_signal[MPC33972_SG6].signal_num = MPC33972_SG6;

	can_signal[MPC33972_SG7].bit_length = 16;
	can_signal[MPC33972_SG7].start_bit = 48;
	can_signal[MPC33972_SG7].signal_num = MPC33972_SG7;

	can_signal[MPC33972_SG8].bit_length = 16;
	can_signal[MPC33972_SG8].start_bit = 0;
	can_signal[MPC33972_SG8].signal_num = MPC33972_SG8;

	can_signal[MPC33972_SP0_CFG].bit_length = 1;
	can_signal[MPC33972_SP0_CFG].start_bit = 0;
	can_signal[MPC33972_SP0_CFG].signal_num = MPC33972_SP0_CFG;

	can_signal[MPC33972_SP1_CFG].bit_length = 1;
	can_signal[MPC33972_SP1_CFG].start_bit = 1;
	can_signal[MPC33972_SP1_CFG].signal_num = MPC33972_SP1_CFG;

	can_signal[MPC33972_SP2_CFG].bit_length = 1;
	can_signal[MPC33972_SP2_CFG].start_bit = 2;
	can_signal[MPC33972_SP2_CFG].signal_num = MPC33972_SP2_CFG;

	can_signal[MPC33972_SP3_CFG].bit_length = 1;
	can_signal[MPC33972_SP3_CFG].start_bit = 3;
	can_signal[MPC33972_SP3_CFG].signal_num = MPC33972_SP3_CFG;

	can_signal[MPC33972_SP4_CFG].bit_length = 1;
	can_signal[MPC33972_SP4_CFG].start_bit = 4;
	can_signal[MPC33972_SP4_CFG].signal_num = MPC33972_SP4_CFG;

	can_signal[MPC33972_SP5_CFG].bit_length = 1;
	can_signal[MPC33972_SP5_CFG].start_bit = 5;
	can_signal[MPC33972_SP5_CFG].signal_num = MPC33972_SP5_CFG;

	can_signal[MPC33972_SP6_CFG].bit_length = 1;
	can_signal[MPC33972_SP6_CFG].start_bit = 6;
	can_signal[MPC33972_SP6_CFG].signal_num = MPC33972_SP6_CFG;

	can_signal[MPC33972_SP7_CFG].bit_length = 1;
	can_signal[MPC33972_SP7_CFG].start_bit = 7;
	can_signal[MPC33972_SP7_CFG].signal_num = MPC33972_SP7_CFG;

	can_signal[PWM_DPI1_DUTY].bit_length = 16;
	can_signal[PWM_DPI1_DUTY].start_bit = 16;
	can_signal[PWM_DPI1_DUTY].signal_num = PWM_DPI1_DUTY;

	can_signal[PWM_DPI1_FREQ].bit_length = 16;
	can_signal[PWM_DPI1_FREQ].start_bit = 0;
	can_signal[PWM_DPI1_FREQ].signal_num = PWM_DPI1_FREQ;

	can_signal[PWM_DPI2_DUTY].bit_length = 16;
	can_signal[PWM_DPI2_DUTY].start_bit = 48;
	can_signal[PWM_DPI2_DUTY].signal_num = PWM_DPI2_DUTY;

	can_signal[PWM_DPI2_FREQ].bit_length = 16;
	can_signal[PWM_DPI2_FREQ].start_bit = 32;
	can_signal[PWM_DPI2_FREQ].signal_num = PWM_DPI2_FREQ;

	can_signal[PWM_DPI3_DUTY].bit_length = 16;
	can_signal[PWM_DPI3_DUTY].start_bit = 16;
	can_signal[PWM_DPI3_DUTY].signal_num = PWM_DPI3_DUTY;

	can_signal[PWM_DPI3_FREQ].bit_length = 16;
	can_signal[PWM_DPI3_FREQ].start_bit = 0;
	can_signal[PWM_DPI3_FREQ].signal_num = PWM_DPI3_FREQ;

	can_signal[PWM_DPI4_DUTY].bit_length = 16;
	can_signal[PWM_DPI4_DUTY].start_bit = 48;
	can_signal[PWM_DPI4_DUTY].signal_num = PWM_DPI4_DUTY;

	can_signal[PWM_DPI4_FREQ].bit_length = 16;
	can_signal[PWM_DPI4_FREQ].start_bit = 32;
	can_signal[PWM_DPI4_FREQ].signal_num = PWM_DPI4_FREQ;

	can_signal[S2_CTRL].bit_length = 1;
	can_signal[S2_CTRL].start_bit = 12;
	can_signal[S2_CTRL].signal_num = S2_CTRL;

	can_signal[SP0_STATUS].bit_length = 1;
	can_signal[SP0_STATUS].start_bit = 0;
	can_signal[SP0_STATUS].signal_num = SP0_STATUS;

	can_signal[SP1_STATUS].bit_length = 1;
	can_signal[SP1_STATUS].start_bit = 1;
	can_signal[SP1_STATUS].signal_num = SP1_STATUS;

	can_signal[SP2_STATUS].bit_length = 1;
	can_signal[SP2_STATUS].start_bit = 2;
	can_signal[SP2_STATUS].signal_num = SP2_STATUS;

	can_signal[SP3_STATUS].bit_length = 1;
	can_signal[SP3_STATUS].start_bit = 3;
	can_signal[SP3_STATUS].signal_num = SP3_STATUS;

	can_signal[SP4_STATUS].bit_length = 1;
	can_signal[SP4_STATUS].start_bit = 4;
	can_signal[SP4_STATUS].signal_num = SP4_STATUS;

	can_signal[SP5_STATUS].bit_length = 1;
	can_signal[SP5_STATUS].start_bit = 5;
	can_signal[SP5_STATUS].signal_num = SP5_STATUS;

	can_signal[SP6_STATUS].bit_length = 1;
	can_signal[SP6_STATUS].start_bit = 6;
	can_signal[SP6_STATUS].signal_num = SP6_STATUS;

	can_signal[SP7_STATUS].bit_length = 1;
	can_signal[SP7_STATUS].start_bit = 7;
	can_signal[SP7_STATUS].signal_num = SP7_STATUS;

	can_signal[SYS_INTERRUPT_TIME].bit_length = 32;
	can_signal[SYS_INTERRUPT_TIME].start_bit = 32;
	can_signal[SYS_INTERRUPT_TIME].signal_num = SYS_INTERRUPT_TIME;

	can_signal[SYS_TIME_RUN_TIME].bit_length = 32;
	can_signal[SYS_TIME_RUN_TIME].start_bit = 0;
	can_signal[SYS_TIME_RUN_TIME].signal_num = SYS_TIME_RUN_TIME;

	can_signal[VQN0_CH0_CURRENT].bit_length = 16;
	can_signal[VQN0_CH0_CURRENT].start_bit = 0;
	can_signal[VQN0_CH0_CURRENT].signal_num = VQN0_CH0_CURRENT;

	can_signal[VQN0_CH1_CURRENT].bit_length = 16;
	can_signal[VQN0_CH1_CURRENT].start_bit = 16;
	can_signal[VQN0_CH1_CURRENT].signal_num = VQN0_CH1_CURRENT;

	can_signal[VQN0_CH2_CURRENT].bit_length = 16;
	can_signal[VQN0_CH2_CURRENT].start_bit = 32;
	can_signal[VQN0_CH2_CURRENT].signal_num = VQN0_CH2_CURRENT;

	can_signal[VQN0_CH3_CURRENT].bit_length = 16;
	can_signal[VQN0_CH3_CURRENT].start_bit = 48;
	can_signal[VQN0_CH3_CURRENT].signal_num = VQN0_CH3_CURRENT;

	can_signal[VQN0_Temperature].bit_length = 8;
	can_signal[VQN0_Temperature].start_bit = 0;
	can_signal[VQN0_Temperature].signal_num = VQN0_Temperature;

	can_signal[VQN0_VCC].bit_length = 16;
	can_signal[VQN0_VCC].start_bit = 0;
	can_signal[VQN0_VCC].signal_num = VQN0_VCC;

	can_signal[VQN1_CH0_CURRENT].bit_length = 16;
	can_signal[VQN1_CH0_CURRENT].start_bit = 0;
	can_signal[VQN1_CH0_CURRENT].signal_num = VQN1_CH0_CURRENT;

	can_signal[VQN1_CH1_CURRENT].bit_length = 16;
	can_signal[VQN1_CH1_CURRENT].start_bit = 16;
	can_signal[VQN1_CH1_CURRENT].signal_num = VQN1_CH1_CURRENT;

	can_signal[VQN1_CH2_CURRENT].bit_length = 16;
	can_signal[VQN1_CH2_CURRENT].start_bit = 32;
	can_signal[VQN1_CH2_CURRENT].signal_num = VQN1_CH2_CURRENT;

	can_signal[VQN1_CH3_CURRENT].bit_length = 16;
	can_signal[VQN1_CH3_CURRENT].start_bit = 48;
	can_signal[VQN1_CH3_CURRENT].signal_num = VQN1_CH3_CURRENT;

	can_signal[VQN1_Temperature].bit_length = 8;
	can_signal[VQN1_Temperature].start_bit = 8;
	can_signal[VQN1_Temperature].signal_num = VQN1_Temperature;

	can_signal[VQN1_VCC].bit_length = 16;
	can_signal[VQN1_VCC].start_bit = 16;
	can_signal[VQN1_VCC].signal_num = VQN1_VCC;

	can_signal[VQN2_CH0_CURRENT].bit_length = 16;
	can_signal[VQN2_CH0_CURRENT].start_bit = 0;
	can_signal[VQN2_CH0_CURRENT].signal_num = VQN2_CH0_CURRENT;

	can_signal[VQN2_CH1_CURRENT].bit_length = 16;
	can_signal[VQN2_CH1_CURRENT].start_bit = 16;
	can_signal[VQN2_CH1_CURRENT].signal_num = VQN2_CH1_CURRENT;

	can_signal[VQN2_CH2_CURRENT].bit_length = 16;
	can_signal[VQN2_CH2_CURRENT].start_bit = 32;
	can_signal[VQN2_CH2_CURRENT].signal_num = VQN2_CH2_CURRENT;

	can_signal[VQN2_CH3_CURRENT].bit_length = 16;
	can_signal[VQN2_CH3_CURRENT].start_bit = 48;
	can_signal[VQN2_CH3_CURRENT].signal_num = VQN2_CH3_CURRENT;

	can_signal[VQN2_Temperature].bit_length = 8;
	can_signal[VQN2_Temperature].start_bit = 16;
	can_signal[VQN2_Temperature].signal_num = VQN2_Temperature;

	can_signal[VQN2_VCC].bit_length = 16;
	can_signal[VQN2_VCC].start_bit = 32;
	can_signal[VQN2_VCC].signal_num = VQN2_VCC;

}


uint32_t can_protocol_bind(struct CanMSG *msg, const struct SignalCfg *signal, uint32_t data){

	uint32_t ret = 0;
	uint64_t temp_data = 0;
	uint64_t msg_dat = 0;
	uint64_t mask = 0;
	uint64_t mask_l = 0;
	uint64_t mask_h = 0;

	uint32_t i = 0;

	if(msg == NULL || signal == NULL){
		/* parameter error */
		ret = 1;
	}else if((signal->start_bit + signal->bit_length) > 64){
		/* parameter error */
		ret = 2;
	}else{

		msg_dat |= (uint64_t)(msg->data[0]);
		msg_dat |= (uint64_t)(msg->data[1]) << 8;
		msg_dat |= (uint64_t)(msg->data[2]) << 16;
		msg_dat |= (uint64_t)(msg->data[3]) << 24;
		msg_dat |= (uint64_t)(msg->data[4]) << 32;
		msg_dat |= (uint64_t)(msg->data[5]) << 40;
		msg_dat |= (uint64_t)(msg->data[6]) << 48;
		msg_dat |= (uint64_t)(msg->data[7]) << 56;

		for(i = signal->start_bit; i < (signal->start_bit + signal->bit_length); i ++ ){
			if(i < 32){
				mask_l |= 1 << i;
			}else{
				mask_h |= 1 << (i - 32);
			}
		}
		mask = (mask_h << 32) + mask_l;

		temp_data = (uint64_t)data << signal->start_bit;
		temp_data &= mask;

		/* clear msg data signal bit */
		mask = ~mask;
		msg_dat &= mask;
		msg_dat |= temp_data;

		msg->data[0] = (uint8_t)msg_dat;
		msg->data[1] = (uint8_t)(msg_dat >> 8);
		msg->data[2] = (uint8_t)(msg_dat >> 16);
		msg->data[3] = (uint8_t)(msg_dat >> 24);
		msg->data[4] = (uint8_t)(msg_dat >> 32);
		msg->data[5] = (uint8_t)(msg_dat >> 40);
		msg->data[6] = (uint8_t)(msg_dat >> 48);
		msg->data[7] = (uint8_t)(msg_dat >> 56);
	}
	return ret;
}

uint32_t can_protocol_unbind(struct CanMSG *msg, const struct SignalCfg *signal, uint32_t *data){
	uint32_t ret = 0;
	uint64_t temp_data = 0;
	uint64_t msg_dat = 0;
	uint64_t mask = 0;
	int32_t i = 0;
	uint64_t mask_l = 0;
	uint64_t mask_h = 0;

	if((msg == NULL) || (signal == NULL) || (data == NULL)){
		/* parameter error */
		ret = 1;
	}else if((signal->start_bit + signal->bit_length) > 64){
		/* parameter error */
		ret = 2;
	}else{
		for(i = signal->start_bit; i < (signal->start_bit + signal->bit_length); i ++ ){
			if(i < 32){
				mask_l |= 1 << i;
			}else{
				mask_h |= 1 << (i - 32);
			}
		}

		mask = (mask_h << 32) + mask_l;

		msg_dat |= (uint64_t)(msg->data[0]);
		msg_dat |= (uint64_t)(msg->data[1]) << 8;
		msg_dat |= (uint64_t)(msg->data[2]) << 16;
		msg_dat |= (uint64_t)(msg->data[3]) << 24;
		msg_dat |= (uint64_t)(msg->data[4]) << 32;
		msg_dat |= (uint64_t)(msg->data[5]) << 40;
		msg_dat |= (uint64_t)(msg->data[6]) << 48;
		msg_dat |= (uint64_t)(msg->data[7]) << 56;

		msg_dat &= mask;
		temp_data = msg_dat >> signal->start_bit;

		*data = (uint32_t)temp_data;
	}
	return ret;
}

extern void voltage_compute_init(void);
extern int32_t get_bus_voltage(void);
extern int32_t get_battery_voltage(void);
extern void m25pe40_init(void);
extern int32_t shunt_init(void);
extern int32_t get_shunt(uint8_t shunt_num, uint8_t rang);

int32_t current01 = 0;
int32_t current02 = 0;
int32_t current11 = 0;
int32_t current12 = 0;
int32_t bus_voltage = 0;
int32_t batt_voltage = 0;

int32_t mpc33908_temperature = 0;
uint32_t mpc33908_vref = 0;
uint32_t mpc33908_io1 = 0;
uint32_t mpc33908_io0 = 0;
uint32_t mpc33908_vsense = 0;
int16_t mpc5744_chip_t0 = 0;
int16_t mpc5744_chip_t1 = 0;
int32_t b1 = 0;
int32_t b2 = 0;
int32_t c1 =0;
int32_t c2 = 0;
int32_t cc = 0;
int32_t cc2 = 0;
int32_t cp = 0;
int32_t dc12 = 0;

int32_t ison_voltage = 0;
int32_t isop_voltage = 0;

int32_t DPI1_freq = 0;
int32_t DPI1_duty = 0;

int32_t DPI2_freq = 0;
int32_t DPI2_duty = 0;

int32_t DPI3_freq = 0;
int32_t DPI3_duty = 0;

int32_t DPI4_freq = 0;
int32_t DPI4_duty = 0;

int32_t CP_freq = 0;
int32_t CP_duty = 0;


uint32_t timer_start = 0;
uint32_t timer_end = 0;
uint32_t timer_diff = 0;

uint16_t discharge_ctrl = 0;

uint32_t mpc33972_sg[14];

volatile int counter = 0;

struct vnq7050_dev{
	int32_t ch1_current;
	int32_t ch2_current;
	int32_t ch3_current;
	int32_t ch4_current;
	int32_t vcc;
	int32_t temperature;
};

struct vnq7050_dev vnq7050_dat[3];
struct mpc33972_sp_ch mpc33972_sp_input;

void sys_test(void){
	int32_t i = 0;
	int32_t ret = 0;
	struct CanMSG msg;

	struct mpc33908_io_status mpc33908_io;
	struct mpc33908_wakeup_source mpc33908_wakeup;

	uint32_t kn_ctrl[12] = {0};
	uint32_t s2_status = 0;
	uint32_t led2_status = 0;
	uint32_t led1_status = 0;
	uint32_t iso_p_status = 0;
	uint32_t iso_n_status = 0;

	timer_start = system_timer_counter();
	counter ++;

	ad_sample_ctrl();

	current01 = get_shunt(0, 0);
	current02 = get_shunt(0, 1);
	current11 = get_shunt(1, 0);
	current12 = get_shunt(1, 1);

	bus_voltage = get_bus_voltage();
	batt_voltage = get_battery_voltage();

	mpc33908_temperature = get_mpc33908_temperature();
	mpc33908_vref = get_mpc33908_ref_voltage();
	mpc33908_io1 = get_mpc33908_io1_voltage();
	mpc33908_io0 = get_mpc33908_io0_voltage();
	mpc33908_vsense = get_mpc33908_vsense_value();
	mpc33908_wakeup = mpc33908_get_wakeup_source();

	mpc5744_chip_t0 = get_chip_temperature(0);
	mpc5744_chip_t1 = get_chip_temperature(1);

	i = counter%14;
	mpc33972_sg[i] =  get_mpc33972_sg_voltage(i);

	b1 = get_b1_voltage();
	b2 = get_b2_voltage();
	c1 = get_c1_voltage();
	c2 = get_c2_voltage();

	cc = get_cc_voltage();
	cc2 = get_cc2_voltage();
	cp = get_cp_voltage();

	dc12 = get_dc12V_current();

	isop_voltage = get_isop_voltage();
	ison_voltage = get_ison_voltage();

	if(counter == 10){
		mpc33972_init();
	}



	vnq7050_dat[0].ch1_current = get_vnq7050_channel_current(0, 0);
	vnq7050_dat[0].ch2_current = get_vnq7050_channel_current(0, 1);
	vnq7050_dat[0].ch3_current = get_vnq7050_channel_current(0, 2);
	vnq7050_dat[0].ch4_current = get_vnq7050_channel_current(0, 3);
	vnq7050_dat[0].temperature = get_vnq7050_temperature(0);
	vnq7050_dat[0].vcc = get_vnq7050_vcc(0);

	vnq7050_dat[1].ch1_current = get_vnq7050_channel_current(1, 0);
	vnq7050_dat[1].ch2_current = get_vnq7050_channel_current(1, 1);
	vnq7050_dat[1].ch3_current = get_vnq7050_channel_current(1, 2);
	vnq7050_dat[1].ch4_current = get_vnq7050_channel_current(1, 3);
	vnq7050_dat[1].temperature = get_vnq7050_temperature(1);
	vnq7050_dat[1].vcc = get_vnq7050_vcc(1);

	vnq7050_dat[2].ch1_current = get_vnq7050_channel_current(2, 0);
	vnq7050_dat[2].ch2_current = get_vnq7050_channel_current(2, 1);
	vnq7050_dat[2].ch3_current = get_vnq7050_channel_current(2, 2);
	vnq7050_dat[2].ch4_current = get_vnq7050_channel_current(2, 3);
	vnq7050_dat[2].temperature = get_vnq7050_temperature(2);
	vnq7050_dat[2].vcc = get_vnq7050_vcc(2);


	timer_end = system_timer_counter();
	timer_diff = timer_end - timer_start;

	msg.id = 0x2F0;
	msg.dat_len = 8;
	can_protocol_bind(&msg, &can_signal[SYS_TIME_RUN_TIME], timer_diff);

	timer_start = system_timer_counter();
	while(timer_flag == 0){
		;
	}
	timer_end = system_timer_counter();
	timer_diff = timer_end - timer_start;
	timer_flag = 0;

	can_protocol_bind(&msg, &can_signal[SYS_INTERRUPT_TIME], timer_diff);

	can_bus_write(0, &msg);

	mpc33972_sp_input = mpc33972_get_sp_input_status();


	mpc33908_io = mpc33908_get_io_status();
	if(mpc33908_io.io_4 | mpc33908_io.io_5){

	}else{
		if(counter % 5 == 0){
			mpc33908_set_lpoff_mode();
		}
	}

	DPI1_freq = eTimer_get_freq(1, 4);
	DPI1_duty = eTimer_get_duty(1, 4);

	DPI2_freq = eTimer_get_freq(2, 5);
	DPI2_duty = eTimer_get_duty(2, 5);

	DPI3_freq = eTimer_get_freq(1, 2);
	DPI3_duty = eTimer_get_duty(1, 2);

	DPI4_freq = eTimer_get_freq(0, 0);
	DPI4_duty = eTimer_get_duty(0, 0);

	CP_freq = eTimer_get_freq(0, 4);
	CP_duty = eTimer_get_duty(0, 4);

	eTimer_start_all();

	{

		msg.id = 0x20D;
		msg.dat_len = 8;
		can_protocol_bind(&msg, &can_signal[AD_CC2], cc2);
		can_protocol_bind(&msg, &can_signal[AD_CP], cp);
		can_protocol_bind(&msg, &can_signal[AD_CC], cc);
		can_protocol_bind(&msg, &can_signal[AD_DC12], dc12);
		can_bus_write(0, &msg);

		msg.id = 0x20E;
		can_protocol_bind(&msg, &can_signal[AD_B1], b1);
		can_protocol_bind(&msg, &can_signal[AD_B2], b2);
		can_protocol_bind(&msg, &can_signal[AD_C1], c1);
		can_protocol_bind(&msg, &can_signal[AD_C2], c2);
		can_bus_write(0, &msg);

		msg.id = 0x20F;
		can_protocol_bind(&msg, &can_signal[AD_CURRENT_01], current01);
		can_protocol_bind(&msg, &can_signal[AD_CURRENT_02], current02);
		can_protocol_bind(&msg, &can_signal[AD_CURRENT_11], current11);
		can_protocol_bind(&msg, &can_signal[AD_CURRENT_12], current12);
		can_bus_write(0, &msg);

		msg.id = 0x210;
		can_protocol_bind(&msg, &can_signal[AD_VBUS], bus_voltage/100);
		can_protocol_bind(&msg, &can_signal[AD_VBATT], batt_voltage/100);
		can_protocol_bind(&msg, &can_signal[AD_ISOP], isop_voltage);
		can_protocol_bind(&msg, &can_signal[AD_ISON], ison_voltage);
		can_bus_write(0, &msg);

		msg.id = 0x203;
		can_protocol_bind(&msg, &can_signal[CP_FREQ], CP_freq);
		can_protocol_bind(&msg, &can_signal[CP_DUTY], CP_duty);
		can_protocol_bind(&msg, &can_signal[CP_VOLTAGE], cp);
		can_bus_write(0, &msg);

		msg.id = 0x201;
		can_protocol_bind(&msg, &can_signal[SP0_STATUS], mpc33972_sp_input.sp0);
		can_protocol_bind(&msg, &can_signal[SP1_STATUS], mpc33972_sp_input.sp1);
		can_protocol_bind(&msg, &can_signal[SP2_STATUS], mpc33972_sp_input.sp2);
		can_protocol_bind(&msg, &can_signal[SP3_STATUS], mpc33972_sp_input.sp3);
		can_protocol_bind(&msg, &can_signal[SP4_STATUS], mpc33972_sp_input.sp4);
		can_protocol_bind(&msg, &can_signal[SP5_STATUS], mpc33972_sp_input.sp5);
		can_protocol_bind(&msg, &can_signal[SP6_STATUS], mpc33972_sp_input.sp6);
		can_protocol_bind(&msg, &can_signal[SP7_STATUS], mpc33972_sp_input.sp7);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO0_STATUS], mpc33908_io.io_0);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO1_STATUS], mpc33908_io.io_1);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO2_STATUS], mpc33908_io.io_2);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO3_STATUS], mpc33908_io.io_3);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO4_STATUS], mpc33908_io.io_4);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO5_STATUS], mpc33908_io.io_5);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO0_WAKEUP], mpc33908_wakeup.io_0_wu);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO1_WAKEUP], mpc33908_wakeup.io_1_wu);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO4_WAKEUP], mpc33908_wakeup.io_4_wu);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO5_WAKEUP], mpc33908_wakeup.io_5_wu);
		can_protocol_bind(&msg, &can_signal[MPC33908_CAN_WAKEUP], mpc33908_wakeup.phy_wu);
		can_bus_write(0, &msg);

		msg.id = 0x20B;
		can_protocol_bind(&msg, &can_signal[MPC33908_Vref], mpc33908_vref);
		can_protocol_bind(&msg, &can_signal[MPC33908_VCC], mpc33908_vsense);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO0_VOLTAGE], mpc33908_io0);
		can_protocol_bind(&msg, &can_signal[MPC33908_IO1_VOLTAGE], mpc33908_io1);
		can_bus_write(0, &msg);

		msg.id = 0x20C;
		can_protocol_bind(&msg, &can_signal[MPC33908_Temperature], mpc33908_temperature);
		can_bus_write(0, &msg);

		msg.id = 0x208;
		can_protocol_bind(&msg, &can_signal[MPC33972_SG0], mpc33972_sg[0]);
		can_protocol_bind(&msg, &can_signal[MPC33972_SG1], mpc33972_sg[1]);
		can_protocol_bind(&msg, &can_signal[MPC33972_SG2], mpc33972_sg[2]);
		can_protocol_bind(&msg, &can_signal[MPC33972_SG3], mpc33972_sg[3]);
		can_bus_write(0, &msg);

		msg.id = 0x209;
		can_protocol_bind(&msg, &can_signal[MPC33972_SG4], mpc33972_sg[4]);
		can_protocol_bind(&msg, &can_signal[MPC33972_SG5], mpc33972_sg[5]);
		can_protocol_bind(&msg, &can_signal[MPC33972_SG6], mpc33972_sg[6]);
		can_protocol_bind(&msg, &can_signal[MPC33972_SG7], mpc33972_sg[7]);
		can_bus_write(0, &msg);

		msg.id = 0x20A;
		can_protocol_bind(&msg, &can_signal[MPC33972_SG8], mpc33972_sg[8]);
		can_bus_write(0, &msg);

		msg.id = 0x202;
		can_protocol_bind(&msg, &can_signal[PWM_DPI1_DUTY], DPI1_duty);
		can_protocol_bind(&msg, &can_signal[PWM_DPI1_FREQ], DPI1_freq);
		can_protocol_bind(&msg, &can_signal[PWM_DPI2_DUTY], DPI2_duty);
		can_protocol_bind(&msg, &can_signal[PWM_DPI2_FREQ], DPI2_freq);
		can_bus_write(0, &msg);

		msg.id = 0x211;
		can_protocol_bind(&msg, &can_signal[PWM_DPI3_DUTY], DPI3_duty);
		can_protocol_bind(&msg, &can_signal[PWM_DPI3_FREQ], DPI3_freq);
		can_protocol_bind(&msg, &can_signal[PWM_DPI4_DUTY], DPI4_duty);
		can_protocol_bind(&msg, &can_signal[PWM_DPI4_FREQ], DPI4_freq);
		can_bus_write(0, &msg);

		msg.id = 0x212;
		can_protocol_bind(&msg, &can_signal[VQN0_Temperature], vnq7050_dat[0].temperature);
		can_protocol_bind(&msg, &can_signal[VQN1_Temperature], vnq7050_dat[1].temperature);
		can_protocol_bind(&msg, &can_signal[VQN2_Temperature], vnq7050_dat[2].temperature);
		can_bus_write(0, &msg);

		msg.id = 0x207;
		can_protocol_bind(&msg, &can_signal[VQN0_VCC], vnq7050_dat[0].vcc);
		can_protocol_bind(&msg, &can_signal[VQN1_VCC], vnq7050_dat[1].vcc);
		can_protocol_bind(&msg, &can_signal[VQN2_VCC], vnq7050_dat[2].vcc);
		can_bus_write(0, &msg);

		msg.id = 0x204;
		can_protocol_bind(&msg, &can_signal[VQN0_CH0_CURRENT], vnq7050_dat[0].ch1_current);
		can_protocol_bind(&msg, &can_signal[VQN0_CH1_CURRENT], vnq7050_dat[0].ch2_current);
		can_protocol_bind(&msg, &can_signal[VQN0_CH2_CURRENT], vnq7050_dat[0].ch3_current);
		can_protocol_bind(&msg, &can_signal[VQN0_CH3_CURRENT], vnq7050_dat[0].ch4_current);
		can_bus_write(0, &msg);

		msg.id = 0x205;
		can_protocol_bind(&msg, &can_signal[VQN1_CH0_CURRENT], vnq7050_dat[1].ch1_current);
		can_protocol_bind(&msg, &can_signal[VQN1_CH1_CURRENT], vnq7050_dat[1].ch2_current);
		can_protocol_bind(&msg, &can_signal[VQN1_CH2_CURRENT], vnq7050_dat[1].ch3_current);
		can_protocol_bind(&msg, &can_signal[VQN1_CH3_CURRENT], vnq7050_dat[1].ch4_current);
		can_bus_write(0, &msg);

		msg.id = 0x206;
		can_protocol_bind(&msg, &can_signal[VQN2_CH0_CURRENT], vnq7050_dat[2].ch1_current);
		can_protocol_bind(&msg, &can_signal[VQN2_CH1_CURRENT], vnq7050_dat[2].ch2_current);
		can_protocol_bind(&msg, &can_signal[VQN2_CH2_CURRENT], vnq7050_dat[2].ch3_current);
		can_protocol_bind(&msg, &can_signal[VQN2_CH3_CURRENT], vnq7050_dat[2].ch4_current);
		can_bus_write(0, &msg);

		msg.id = 0x300;
		ret = can_bus_read(0, &msg);
		if(ret == 0){
			/* setup MPC33972 sp switch */
			mpc33972_set_sp(0, msg.data[0]&0x01);
			mpc33972_set_sp(1, (msg.data[0] >> 1)&0x01);
			mpc33972_set_sp(2, (msg.data[0] >> 2)&0x01);
			mpc33972_set_sp(3, (msg.data[0] >> 3)&0x01);
			mpc33972_set_sp(4, (msg.data[0] >> 4)&0x01);
			mpc33972_set_sp(5, (msg.data[0] >> 5)&0x01);
			mpc33972_set_sp(6, (msg.data[0] >> 6)&0x01);
			mpc33972_set_sp(7, (msg.data[0] >> 7)&0x01);
		}

		msg.id = 0x301;
		ret = can_bus_read(0, &msg);
		if(ret == 0){

		}

		msg.id = 0x200;
		ret = can_bus_read(0, &msg);
		if(ret == 0){
			/*unbind message */
			can_protocol_unbind(&msg, &can_signal[K1_CTRL], &kn_ctrl[0]);
			can_protocol_unbind(&msg, &can_signal[K2_CTRL], &kn_ctrl[1]);
			can_protocol_unbind(&msg, &can_signal[K3_CTRL], &kn_ctrl[2]);
			can_protocol_unbind(&msg, &can_signal[K4_CTRL], &kn_ctrl[3]);
			can_protocol_unbind(&msg, &can_signal[K5_CTRL], &kn_ctrl[4]);
			can_protocol_unbind(&msg, &can_signal[K6_CTRL], &kn_ctrl[5]);
			can_protocol_unbind(&msg, &can_signal[K7_CTRL], &kn_ctrl[6]);
			can_protocol_unbind(&msg, &can_signal[K8_CTRL], &kn_ctrl[7]);
			can_protocol_unbind(&msg, &can_signal[K9_CTRL], &kn_ctrl[8]);
			can_protocol_unbind(&msg, &can_signal[K10_CTRL], &kn_ctrl[9]);
			can_protocol_unbind(&msg, &can_signal[K11_CTRL], &kn_ctrl[10]);
			can_protocol_unbind(&msg, &can_signal[K12_CTRL], &kn_ctrl[11]);
			can_protocol_unbind(&msg, &can_signal[S2_CTRL], &s2_status);
			can_protocol_unbind(&msg, &can_signal[LED1_CTRL], &led1_status);
			can_protocol_unbind(&msg, &can_signal[LED2_CTRL], &led2_status);
			can_protocol_unbind(&msg, &can_signal[ISO_N_CTRL], &iso_n_status);
			can_protocol_unbind(&msg, &can_signal[ISO_P_CTRL], &iso_p_status);
			/* */
			vnq7050_k1_ctrl(kn_ctrl[0]);
			vnq7050_k2_ctrl(kn_ctrl[1]);
			vnq7050_k3_ctrl(kn_ctrl[2]);
			vnq7050_k4_ctrl(kn_ctrl[3]);
			vnq7050_k5_ctrl(kn_ctrl[4]);
			vnq7050_k6_ctrl(kn_ctrl[5]);
			vnq7050_k7_ctrl(kn_ctrl[6]);
			vnq7050_k8_ctrl(kn_ctrl[7]);
			vnq7050_k9_ctrl(kn_ctrl[8]);
			vnq7050_ka_ctrl(kn_ctrl[9]);
			vnq7050_kb_ctrl(kn_ctrl[10]);
			vnq7050_kc_ctrl(kn_ctrl[11]);
			s2_ctrl(s2_status);
			led1_ctrl(led1_status);
			led2_ctrl(led2_status);
			iso_p_ctrl(iso_p_status);
			iso_n_ctrl(iso_n_status);

		}

		can_bus_write(1, &msg);
		can_bus_write(2, &msg);
		ret = can_bus_status(0);
		if(ret == 1){
			can_bus_recover(0);
		}

		ret = can_bus_status(1);
		if(ret == 1){
			can_bus_recover(1);
		}

		ret = can_bus_status(2);
		if(ret == 1){
			can_bus_recover(2);
		}

	}
}

#define FLASH_TEST_BUFFER_LEN	253

uint8_t flash_test_buf[FLASH_TEST_BUFFER_LEN];
uint8_t flash_read_buf[FLASH_TEST_BUFFER_LEN];

int main(void)
{

	int32_t ret = 0;

	/* Configure FCCU */
	FCCU_SetProtocol(FCCU_BI_STABLE);		//set Bi-Stable protocol if not in the SAFE mode

	/* Init MODE ENTRY module */
	MC_MODE_INIT();                             /* (pll_init) Setup the MCU clocks and modes */

	xcptn_xmpl();              /* Configure and Enable Interrupts */

	gpio_output_init();
	gpio_input_init();


	fs_ctrl(1);

	spi_bus_init();
	mpc33908_init();
	mpc33972_init();

	m25pe40_init();
	LTC6811_init();

	adc_init(0);
	adc_init(1);

	adc_start(0, 0x0000FFFF);
	adc_start(1, 0x0000FFFF);

	voltage_compute_init();
	shunt_init();

	system_timer_init(50);
	system_timer_start();

	eTimer_init_all();

	ret = eeprom_erase();
	ret = eeprom_init();

	m25pe40_init();

	can_bus_init(0);
	can_bus_init(1);
	can_bus_init(2);

	/* Init PIT module */
	PIT_Init();
	PIT_Setup(0, 50000000, 0.01);	/* 0.6 is 0.6s */
	PIT_EnableInt(0, timer0_irq_handler);
	PIT_EnableChannel(0);

	can_signal_init();

	ext_flash_init();


	/* Loop forever */
	for(;;) {

		flash_test_buf[0] += 1;
		flash_test_buf[252] += 1;
		//sys_test();
		ext_flash_write(0x100, flash_test_buf, sizeof(flash_test_buf));
		ext_flash_read(0x100, flash_read_buf, sizeof(flash_read_buf));

		flash_test_buf[12] += 1;
		ext_flash_write(0x200, flash_test_buf, 13);
		ext_flash_read(0x200, flash_read_buf, 13);

		flash_test_buf[78] += 1;
		ext_flash_write(0x300, flash_test_buf, 79);
		ext_flash_read(0x300, flash_read_buf, 79);

	}


	return ret;
}


