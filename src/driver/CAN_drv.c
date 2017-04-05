/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ11ÈÕ
* Compiler   : gcc
* FILE NAME  : CAN_drv.c
*
* DESCRIPTION:
*
*/

#include "derivative.h"
#include "SIUL.h"
#include "CAN_bus.h"
#include "CAN_drv.h"

#define MB_RX_ACTIVE	0x04
#define MB_RX_FULL		0x02
#define MB_RX_OVERRUN	0x06

#define CAN_TX_MB_INACTIVE	0x08000000
#define CAN_TX_MB_ACTIVE	0x0C000000

#define CAN_RX_INT_PRIORITY			3
#define CAN_TX_INT_PRIORITY			3

#define CAN_INT_FLAG_MASK00		0x0000000F
#define CAN_INT_FLAG_MASK04		0x000000F0
#define CAN_INT_FLAG_MASK08		0x00000F00
#define CAN_INT_FLAG_MASK12		0x0000F000
#define CAN_INT_FLAG_MASK16		0x000F0000
#define CAN_INT_FLAG_MASK20		0x00F00000
#define CAN_INT_FLAG_MASK24		0x0F000000
#define CAN_INT_FLAG_MASK28		0xF0000000

#define CAN0_MODULE_CLOCK		40000000
#define CAN0_MODULE_PRESCALE	9
#define CAN0_MODULE_BAUDRATE	500000
#define CAN0_MODULE_SAMPLE_RATE	75

#define CAN1_MODULE_CLOCK		40000000
#define CAN1_MODULE_PRESCALE	9
#define CAN1_MODULE_BAUDRATE	500000
#define CAN1_MODULE_SAMPLE_RATE	75

#define CAN2_MODULE_CLOCK		40000000
#define CAN2_MODULE_PRESCALE	9
#define CAN2_MODULE_BAUDRATE	250000
#define CAN2_MODULE_SAMPLE_RATE	75

#define CAN0_ADDR ((volatile struct CAN_tag *) 0xFFEC0000UL)
#define CAN1_ADDR ((volatile struct CAN_tag *) 0xFFEC4000UL)
#define CAN2_ADDR ((volatile struct CAN_tag *) 0xFFEC8000UL)

struct  CanBaudRate {
    uint32_t  baud_rate;                          /* Holds the Baudrate.                                  */
    uint32_t  sample_point;                       /* Holds the Sample point in 0.1%                       */
    uint32_t  resync_jumpwidth;                   /* Holds the Re-Synchronization Jump Width in 0.1%      */
    uint8_t  prescaler_div;                       /* Holds the Prescaler Divide Factor                    */
    uint8_t  SJW;                                 /* Holds the Re-Synch Jump Width         (StdValue = 1) */
    uint8_t  SEG;                                 /* Holds the Propagation Segment Time    (StdValue = 2) */
    uint8_t  PSEG1;                               /* Holds the Phase Buffer Segment 1      (StdValue = 7) */
    uint8_t  PSEG2;                               /* Holds the Phase Buffer Segment 2      (StdValue = 7) */
};


enum  CanMode {
    CAN_MODE_CONFIG = 0u,              	/* CAN Controller Set To: Configure Mode / Reset Mode.  */
    CAN_MODE_RESET,                    	/* CAN Controller Set To: Reset CAN Module.             */
    CAN_MODE_SLEEP,                    	/* CAN Controller Set To: Sleep     Mode.               */
    CAN_MODE_SNOOP,                    	/* CAN Controller Set To: Snoop     Mode.               */
    CAN_MODE_LOOP_BACK,                	/* CAN Controller Set To: Loop Back Mode.               */
    CAN_MODE_NORMAL ,                  	/* CAN Controller Set To: Normal    Mode.               */
	CAN_MODE_FREEZE,							/* CAN Controller Set To: Normal    Mode.               */
	CAN_MODE_HALT,
};


/*
*********************************************************************************************************
*                                          CAN FRAME STRUCT
*
* Description : Structure defines a CAN Frame.
*
* Note(s)     : To Differentiate between Standard and Extended IDs, the following Addition to the
*               ID is implemented: (Based on the Structure found in uC/CAN Frame files).
*                   - Bit #31     : Reserved (Always 0u)
*                   - Bit #30     : Remote Transmission Request Flag (1u = RTR, 0u = Data Frame)
*                   - Bit #29     : Extended ID Flag (1u = Extended, 0u = Standard)
*                   - Bit #28 - 0 : Identifier (Standard, Extended, or Both)
*********************************************************************************************************
*/

struct  CanFrame {
	uint32_t  Identifier;        /* CAN IDENTIFIER: Can Identifier                  */
	uint8_t  Data[8u];           /*  CAN PAYLOAD: Bytes[Max 8] in Single CAN Msg    */
	uint8_t  DLC;                /*  CAN DLC: Num of Valid Data(s) in Payload       */
	uint8_t  Spare[3u];          /*  SPARE: Sets FRM w/ Integral Num of Pointers    */
};



struct MPC5744CanCFG{
	uint32_t clock_freq;
	uint8_t prescale;
	uint32_t bit_rate;
	uint32_t sample_rate;
	uint32_t rx_mask[32];
};


struct CanModuleStatus{
	uint32_t error_status;		/* frame error, crc error, etc. */
	uint32_t busoff_flag;		/* 0: normal, 1: bus off */
	uint32_t module_status;		/* 0: stop, 1:running */
};

/*
*********************************************************************************************************
*                                         DRIVER ERROR CODES
*
* Description : Enumeration defines the possible Driver Error Codes.
*
* Note(s)     : none.
*********************************************************************************************************
*/

enum  CanError {
    CAN_ERR_NONE = 0u,					/*        NO ERR: Everything is OK                      */
    CAN_ERR_BUS,                     	/*       BUS ERR: Wrong Bus Was Chosen                  */
    CAN_ERR_BUSY,                    	/*      BUSY ERR: Msg Can't be Sent, Bus is Busy        */
    CAN_ERR_INIT,                    	/*      INIT ERR: Reset State not Set, Dev Init Fail    */
    CAN_ERR_MODE,                    	/*      MODE ERR: Error Accessing Wanted Mode on Device */
    CAN_ERR_OPEN,                    	/*      OPEN ERR: Device can't be Used, Device un-Opened*/
    CAN_ERR_CLOSE,                   	/*     CLOSE ERR: Device can't be Closed                */
    CAN_ERR_FUNC,                    	/*  FUNCTION ERR: Given Function Code is not Valid      */
    CAN_ERR_ARG,                     	/*  ARGUMENT ERR: Argument Check has Failed             */
    CAN_ERR_NO_DATA,                	/*      DATA ERR: No Data is Available                  */
};

/*
*********************************************************************************************************
*                                     I/O CONTROL FUNCTION CODES
*
* Description : Enumeration defines the available Function Codes for the Driver XXXIoCtl() Function.
*
* Note(s)     : none.
*********************************************************************************************************
*/

enum  CanIOCtrlList {
    IOCTRL_CAN_GET_IDENT = 0u,		/* ---------- GET DRIVER IDENTIFICATION CODE ---------- */
                                    /* arg = Pointer to Local Ident Variable (CPU_INT32U)   */
	IOCTRL_CAN_GET_ERRNO,           /* --------------- GET DRIVER ERROR CODE -------------- */
                                    /* arg = Pointer to Local Error Code Var. (CPU_INT16U)  */
	IOCTRL_CAN_GET_DRVNAME,         /* ------------------ GET DRIVER NAME ----------------- */
                                    /* arg = Pointer to Local String Variable (char)        */
	IOCTRL_CAN_SET_BAUDRATE,        /* ----------------- SET BUS BAUDRATE ----------------- */
                                    /* arg = Pointer to Local Baudrate Var. (CPU_INT32U)    */
	IOCTRL_CAN_START,               /* -------------------- ENABLE BUS -------------------- */
                                    /* No Pointer: Fnct Code sets CAN to Operational Mode.  */
	IOCTRL_CAN_CONFIG,              /* ----------------- SET CAN TO CONFIG ---------------- */
                                    /* No Pointer: Fnct Code sets CAN to 'CONFIG' Mode.     */
	IOCTRL_CAN_SLEEP,               /* ----------------- SET CAN TO SLEEP ----------------- */
                                    /* No Pointer: Fnct Code sets CAN to 'SLEEP' Mode.      */
	IOCTRL_CAN_LBACK,               /* ---------------- SET CAN TO LOOPBACK --------------- */
                                    /* No Pointer: Fnct Code sets CAN to 'LOOPBACK' Mode.   */
	IOCTRL_CAN_SNOOP,               /* ----------------- SET CAN TO SNOOP ----------------- */
                                    /* No Pointer: Fnct Code sets CAN to 'SNOOP' Mode.      */
	IOCTRL_CAN_RX_STANDARD,         /* ------- SET  RECIEVER TO STANDARD IDENTIFIER ------- */
                                    /* No Pointer: CAN Rx recieves only CAN Standard IDs    */
	IOCTRL_CAN_RX_EXTENDED,         /* ------- SET  RECIEVER TO EXTENDED IDENTIFIER ------- */
                                    /* No Pointer: CAN Rx recieves only CAN Extended IDs    */
	IOCTRL_CAN_TX_READY,            /* ---------------- GET TX READY STATUS --------------- */
                                    /* arg = Pointer to TX Rdy Status Variable (CPU_INT08U) */
	IOCTRL_CAN_GET_NODE_STATUS,     /* ------------------ GET NODE STATUS ----------------- */
                                    /* arg = Pointer to Node Status Variable (CPU_INT08U)   */
	IOCTRL_CAN_SET_RX_FILTER,       /* ------------------ SET RX FILTER 1 ----------------- */
                                    /* arg = CPU_INT32U[2]: arg[0] = mask, arg[1] = ID      */
	IOCTRL_CAN_IO_FUNC_N            /* ------------- NUMBER OF FUNCTION CODES ------------- */
};                                  /* No Pointer: Holds number of Function Codes Available */


/*
*********************************************************************************************************
*                                            DEVICE NAMES
*
* Description : Enumeration defines the Available Device Names for the Driver XXXCANInit() Function.
*
* Note(s)     : The ZYNQ ZC7000 Series Driver only Supports 2 Internal CAN Controllers (0->1).
*********************************************************************************************************
*/

enum  CanDev {
    CAN_BUS_0 = 0u,		/* Internal CAN controller #0      */
    CAN_BUS_1 = 1u,		/* Internal CAN controller #1      */
	CAN_BUS_2 = 2u,		/* Internal CAN controller #2      */
    CAN_N_DEV = 3u 		/* Number of CAN controllers       */
};



/* define global variable */

static volatile struct CAN_tag *CAN[CAN_N_DEV] = {CAN0_ADDR, CAN1_ADDR, CAN2_ADDR};

static struct CanModuleStatus module_status[CAN_N_DEV];

static struct MPC5744CanCFG module[CAN_N_DEV] = {
	{
		CAN0_MODULE_CLOCK,
		CAN0_MODULE_PRESCALE,
		CAN0_MODULE_BAUDRATE,
		CAN0_MODULE_SAMPLE_RATE,
		{
			0x100,
			0x200,
			0x300,
			0x400,
			0x500,
			0x600,
			0x700,
			0x100,
			0x200,
			0x300,
			0x400,
			0x500,
			0x600,
			0x700,
			0x100,
			0x200,
			0x300,
			0x400,
			0x500,
			0x600,
			0x700,
			0x100,
			0x200,
			0x300,
			0x400,
			0x500,
			0x600,
			0x700,
			0x100,
			0x200,
			0x300,
			0x400
		}
	},
	{
		CAN1_MODULE_CLOCK,
		CAN1_MODULE_PRESCALE,
		CAN1_MODULE_BAUDRATE,
		CAN1_MODULE_SAMPLE_RATE,
		{
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000
		}
	},
	{
		CAN2_MODULE_CLOCK,
		CAN2_MODULE_PRESCALE,
		CAN2_MODULE_BAUDRATE,
		CAN2_MODULE_SAMPLE_RATE,
		{
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000,
			0x00000000
		}
	}
};

/***************************************************************************//*!
*   @brief The function CAN_ConfigurePads configures pads of the CANx module.
*	@par Include
*					CAN.h
* 	@par Description
*					This function configures pads (Tx/Rx) for CAN module
*					specified by nbModule.
* 	@param[in] nbModule
*					Number of the CAN module (0 or 1).
*	@remarks This function is used by the CAN_Init function.
*	@par Code sample
*			CAN_ConfigurePads(0);
*			- Function configures pads for transmission and reception for the
*				CAN module number 0.
********************************************************************************/
void mpc5744_can_io_cfg(uint8_t bus_num){
	switch(bus_num){
		case 0:
			SIUL_DigitalInput(SIUL_PB1,SIUL_IN_PB1, 0, SIUL_ALT2);
    		SIUL_DigitalOutput(SIUL_PB0,SIUL_ALT1);
    		break;
		case 1:
			SIUL_DigitalInput(SIUL_PA15,SIUL_IN_PA15, 0, SIUL_ALT1);
			SIUL_DigitalOutput(SIUL_PA14,SIUL_ALT1);
			break;
		case 2:
			SIUL_DigitalInput(SIUL_PF15,SIUL_IN_PF15, 0, SIUL_ALT1);
			SIUL_DigitalOutput(SIUL_PF14,SIUL_ALT2);
			break;
		default:
			break;
	}

}

/* interrupt init */


void mpc5744_can_int_init(uint8_t bus_num){

	switch(bus_num){
	case 0:
		/* Vector # 520 FLEXCAN_ESR[ERR_INT] FlexCAN_0 */
		/* Vector # 521 FLEXCAN_ESR_BOFF | FLEXCAN_Transmit_Warning | FLEXCAN_Receive_Warning FlexCAN_0 */

		/* 0 ~ 31 used for rx buffer */
		/* Vector # 522 FLEXCAN_BUF_00_03 FlexCAN_0 */
		/* Vector # 523 FLEXCAN_BUF_04_07 FlexCAN_0 */
		/* Vector # 524 FLEXCAN_BUF_08_11 FlexCAN_0 */
		/* Vector # 525 FLEXCAN_BUF_12_15 FlexCAN_0 */
		/* Vector # 526 FLEXCAN_BUF_16_31 FlexCAN_0 */
		/* 32 ~ 63 used for tx buffer */
		/* Vector # 527 FLEXCAN_BUF_32_39 FlexCAN_0 */
		/* Vector # 528 FLEXCAN_BUF_40_47 FlexCAN_0 */
		/* Vector # 529 FLEXCAN_BUF_48_55 FlexCAN_0 */
		/* Vector # 530 FLEXCAN_BUF_56_63 FlexCAN_0 */
		INTC_0.PSR[521].B.PRIN = 1;
		INTC_0.PSR[522].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[523].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[524].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[525].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[526].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[527].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[528].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[529].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[530].B.PRIN = CAN_TX_INT_PRIORITY;
		break;
	case 1:
		/* Vector # 533 FLEXCAN_ESR[ERR_INT] FlexCAN_1 */
		/* Vector # 534 FLEXCAN_ESR_BOFF | FLEXCAN_Transmit_Warning | FLEXCAN_Receive_Warning FlexCAN_1 */

		/* 0 ~ 31 used for rx buffer */
		/* Vector # 535 FLEXCAN_BUF_00_03 FlexCAN_1 */
		/* Vector # 536 FLEXCAN_BUF_04_07 FlexCAN_1 */
		/* Vector # 537 FLEXCAN_BUF_08_11 FlexCAN_1 */
		/* Vector # 538 FLEXCAN_BUF_12_15 FlexCAN_1 */
		/* Vector # 539 FLEXCAN_BUF_16_31 FlexCAN_1 */
		/* 32 ~ 63 used for tx buffer */
		/* Vector # 540 FLEXCAN_BUF_32_39 FlexCAN_1 */
		/* Vector # 541 FLEXCAN_BUF_40_47 FlexCAN_1 */
		/* Vector # 542 FLEXCAN_BUF_48_55 FlexCAN_1 */
		/* Vector # 543 FLEXCAN_BUF_56_63 FlexCAN_1 */
		INTC_0.PSR[534].B.PRIN = 1;
		INTC_0.PSR[535].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[536].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[537].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[538].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[539].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[540].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[541].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[542].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[543].B.PRIN = CAN_TX_INT_PRIORITY;

		break;
	case 2:
		/* Vector # 546 FLEXCAN_ESR[ERR_INT] FlexCAN_2 */
		/* Vector # 547 FLEXCAN_ESR_BOFF | FLEXCAN_Transmit_Warning | FLEXCAN_Receive_Warning FlexCAN_2 */

		/* 0 ~ 31 used for rx buffer */
		/* Vector # 548 FLEXCAN_BUF_00_03 FlexCAN_2 */
		/* Vector # 549 FLEXCAN_BUF_04_07 FlexCAN_2 */
		/* Vector # 550 FLEXCAN_BUF_08_11 FlexCAN_2 */
		/* Vector # 551 FLEXCAN_BUF_12_15 FlexCAN_2 */
		/* Vector # 552 FLEXCAN_BUF_16_31 FlexCAN_2 */
		/* 32 ~ 63 used for tx buffer */
		/* Vector # 553 FLEXCAN_BUF_32_39 FlexCAN_2 */
		/* Vector # 554 FLEXCAN_BUF_40_47 FlexCAN_2 */
		/* Vector # 555 FLEXCAN_BUF_48_55 FlexCAN_2 */
		/* Vector # 556 FLEXCAN_BUF_56_63 FlexCAN_2 */
		INTC_0.PSR[547].B.PRIN = 1;
		INTC_0.PSR[548].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[549].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[550].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[551].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[552].B.PRIN = CAN_RX_INT_PRIORITY;
		INTC_0.PSR[553].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[554].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[555].B.PRIN = CAN_TX_INT_PRIORITY;
		INTC_0.PSR[556].B.PRIN = CAN_TX_INT_PRIORITY;
		break;
	default:
		break;
	}
}



int32_t mpc5744_can_read(uint8_t bus_num, uint8_t mailbox_num, struct MPC5744CanFrame *frame){
	int32_t ret = 0;
	uint8_t code = 0;
	uint32_t __attribute__((unused)) temp = 0;
	if((bus_num < CAN_N_DEV) && (mailbox_num < 64)){
		/* disable interrupt */
		/* mandatory - read control/status word - lock the MB */
		code = CAN[bus_num]->MB[mailbox_num].CS.B.CODE;
		if((code == MB_RX_FULL) || (code == MB_RX_OVERRUN)){
			frame->DLC = CAN[bus_num]->MB[mailbox_num].CS.B.DLC;
			frame->RTR = CAN[bus_num]->MB[mailbox_num].CS.B.RTR;
			frame->IDE = CAN[bus_num]->MB[mailbox_num].CS.B.IDE;
			if(frame->IDE == 1){
				frame->id = CAN[bus_num]->MB[mailbox_num].ID.R & 0x1FFFFFFF;
			}else{
				frame->id = CAN[bus_num]->MB[mailbox_num].ID.B.ID_STD;
			}
			frame->data1 = CAN[bus_num]->MB[mailbox_num].DATA.W[0];
			frame->data2 = CAN[bus_num]->MB[mailbox_num].DATA.W[1];

			/* Acknowledge the proper flag at IFLAG registers. */
			CAN[bus_num]->IFLAG1.R = 1 << mailbox_num;
			/* release the internal lock for all Rx MBs by reading the TIMER */
			temp = CAN[bus_num]->TIMER.R;

		}else{
			ret = CAN_ERR_NO_DATA;
		}
		/* enable interrupt */
	}else{
		ret = -1;
	}

	return ret;
}



int32_t mpc5744_can_write(uint8_t bus_num, uint8_t mailbox_num, struct MPC5744CanFrame *frame){
	int32_t ret = 0;
	uint8_t code = 0;

	if((bus_num < CAN_N_DEV) && (mailbox_num < 64)){
		code = CAN[bus_num]->MB[mailbox_num].CS.B.CODE;
		/* check mailbox status */
		if((code == 0x8) || (code == 0)){

			CAN[bus_num]->MB[mailbox_num].CS.R = CAN_TX_MB_INACTIVE;
			CAN[bus_num]->MB[mailbox_num].CS.B.SRR = 1;
			CAN[bus_num]->MB[mailbox_num].CS.B.IDE = frame->IDE;
			CAN[bus_num]->MB[mailbox_num].CS.B.RTR = frame->RTR;
			CAN[bus_num]->MB[mailbox_num].CS.B.DLC = frame->DLC;

			if(frame->IDE == 1){
				CAN[bus_num]->MB[mailbox_num].ID.R = frame->id;
			}else{
				CAN[bus_num]->MB[mailbox_num].ID.B.ID_STD = frame->id;
			}

			CAN[bus_num]->MB[mailbox_num].ID.B.PRIO = frame->PRIO;
			CAN[bus_num]->MB[mailbox_num].DATA.W[0] = frame->data1;
			CAN[bus_num]->MB[mailbox_num].DATA.W[1] = frame->data2;

			/* check ESR1.TX */
			CAN[bus_num]->MB[mailbox_num].CS.R |= CAN_TX_MB_ACTIVE;  	/*After transmission, After transmission, the MB automaticallyreturns to the INACTIVE state */

		}else{
			ret = CAN_ERR_BUSY;
		}
	}else{
		ret = -1;
	}
	return ret;
}




void mpc5744_can_rx(uint8_t bus_num){
	struct MPC5744CanFrame frame = {0, 0, 0, 0, 0, 0, 0};
	struct CanMSG msg = {0,{0,0,0,0,0,0,0,0},0};

	int32_t i = 0;
	int32_t ret = 0;
	uint32_t int_flag = CAN[bus_num]->IFLAG1.R;

	uint32_t flag_mask[8];
	int32_t grp = 0;
	uint32_t mailbox_num = 0;

	flag_mask[0] = int_flag & CAN_INT_FLAG_MASK00;
	flag_mask[1] = (int_flag & CAN_INT_FLAG_MASK04) >> 4;
	flag_mask[2] = (int_flag & CAN_INT_FLAG_MASK08) >> 8;
	flag_mask[3] = (int_flag & CAN_INT_FLAG_MASK12) >> 12;
	flag_mask[4] = (int_flag & CAN_INT_FLAG_MASK16) >> 16;
	flag_mask[5] = (int_flag & CAN_INT_FLAG_MASK20) >> 20;
	flag_mask[6] = (int_flag & CAN_INT_FLAG_MASK24) >> 24;
	flag_mask[7] = (int_flag & CAN_INT_FLAG_MASK28) >> 28;

	while(grp < 8){
		if(flag_mask[grp] != 0){
			for(i = 0; i < 4; i ++){
				mailbox_num = grp*4 + i;
				if((flag_mask[grp] & 0x01) == 1){
					/* receive new data */
					DI();
					mpc5744_can_read(bus_num, mailbox_num, &frame);

					msg.id = frame.id;
					msg.dat_len = frame.DLC;
					msg.data[0] = (uint8_t)(frame.data1 >> 24);
					msg.data[1] = (uint8_t)(frame.data1 >> 16);
					msg.data[2] = (uint8_t)(frame.data1 >> 8);
					msg.data[3] = (uint8_t)frame.data1;

					msg.data[4] = (uint8_t)(frame.data2 >> 24);
					msg.data[5] = (uint8_t)(frame.data2 >> 16);
					msg.data[6] = (uint8_t)(frame.data2 >> 8);
					msg.data[7] = (uint8_t)frame.data2;

					/* add in fifo */
					ret = can_fifo_in(&msg, &can_rx_fifo[bus_num]);
					EI();
				}
				flag_mask[grp] = flag_mask[grp] >> 1;
			}
			if(ret < 0){
				break;
			}
		}
		grp ++;
	}
}

void mpc5744_can_tx(uint8_t bus_num){
	struct MPC5744CanFrame frame = {0, 0, 0, 0, 0, 0, 0};
	struct CanMSG msg = {0,{0,0,0,0,0,0,0,0},0};
	uint32_t int_flag = CAN[bus_num]->IFLAG2.R;
	int32_t i = 0;

	uint32_t flag_mask[8];
	int32_t grp = 0;
	uint32_t mailbox_num = 0;
	int32_t ret = 0;

	flag_mask[0] = int_flag & CAN_INT_FLAG_MASK00;
	flag_mask[1] = (int_flag & CAN_INT_FLAG_MASK04) >> 4;
	flag_mask[2] = (int_flag & CAN_INT_FLAG_MASK08) >> 8;
	flag_mask[3] = (int_flag & CAN_INT_FLAG_MASK12) >> 12;
	flag_mask[4] = (int_flag & CAN_INT_FLAG_MASK16) >> 16;
	flag_mask[5] = (int_flag & CAN_INT_FLAG_MASK20) >> 20;
	flag_mask[6] = (int_flag & CAN_INT_FLAG_MASK24) >> 24;
	flag_mask[7] = (int_flag & CAN_INT_FLAG_MASK28) >> 28;

	while(grp < 8){
		if(flag_mask[grp] != 0){
			/* message buffer had been send data over */
			for(i = 0; i < 4; i ++){
				DI();
				if((flag_mask[grp] & 0x01) == 1){
					mailbox_num = grp*4 + i;
					/* clear interrupt flag */
					CAN[bus_num]->IFLAG2.R = 1 << mailbox_num;
					/* send end, add new data to this buffer */
					ret = can_fifo_out(&msg, &can_tx_fifo[bus_num]);
					if(ret >= 0){
						if(msg.id < 0x7FF ){
							frame.IDE = 0;
						}else{
							frame.IDE = 1;
						}

						module_status[bus_num].module_status = 1;

						frame.DLC = msg.dat_len;
						frame.RTR = 0;
						frame.PRIO = 7;
						frame.id = msg.id;
						frame.data1 = msg.data[3] + msg.data[2]*0x100 + msg.data[1]*0x10000 + msg.data[0]*0x1000000;
						frame.data2 = msg.data[7] + msg.data[6]*0x100 + msg.data[5]*0x10000 + msg.data[4]*0x1000000;

						mpc5744_can_write(bus_num, mailbox_num+32, &frame);
					}else{
						/* fifo empty, no data need to send */
						module_status[bus_num].module_status = 0;
						EI();
						break;
					}
				}
				flag_mask[grp] = flag_mask[grp] >> 1;
				EI();
			}
		}
		grp ++;
	}

}

void mpc5744_can_error_process(uint8_t bus_num){
	int32_t i = 2000;

	module_status[bus_num].busoff_flag = CAN[bus_num]->ESR1.B.BOFFINT;
	module_status[bus_num].error_status = CAN[bus_num]->ESR1.R;

	if(module_status[bus_num].busoff_flag == 1){
		/* clear interrupt flag */
		CAN[bus_num]->ESR1.R = module_status[bus_num].error_status;
		/* reset can module */
		CAN[bus_num]->MCR.B.SOFTRST = 1;
		while((CAN[bus_num]->MCR.B.SOFTRST == 1) &&(i -- > 0)){

		}
		/* set can module freeze mode */
		CAN[bus_num]->MCR.B.FRZ = 1;
		CAN[bus_num]->MCR.B.HALT = 1;

		i = 2000;
		while((CAN[bus_num]->MCR.B.FRZACK == 0) &&(i -- > 0)){

		}
	}
}


void can0_rx_isr_handler(void){
	/* Vector # 522 FLEXCAN_BUF_00_03 FlexCAN_0 */
	/* Vector # 523 FLEXCAN_BUF_04_07 FlexCAN_0 */
	/* Vector # 524 FLEXCAN_BUF_08_11 FlexCAN_0 */
	/* Vector # 525 FLEXCAN_BUF_12_15 FlexCAN_0 */
	/* Vector # 526 FLEXCAN_BUF_16_31 FlexCAN_0 */
	mpc5744_can_rx(0);

}

void can0_tx_isr_handler(void){
	/* Vector # 527 FLEXCAN_BUF_32_39 FlexCAN_0 */
	/* Vector # 528 FLEXCAN_BUF_40_47 FlexCAN_0 */
	/* Vector # 529 FLEXCAN_BUF_48_55 FlexCAN_0 */
	/* Vector # 530 FLEXCAN_BUF_56_63 FlexCAN_0 */

	mpc5744_can_tx(0);

}

void can0_busoff_isr(void){
	mpc5744_can_error_process(0);
}


void can1_rx_isr_handler(void){
	/* Vector # 535 FLEXCAN_BUF_00_03 FlexCAN_1 */
	/* Vector # 536 FLEXCAN_BUF_04_07 FlexCAN_1 */
	/* Vector # 537 FLEXCAN_BUF_08_11 FlexCAN_1 */
	/* Vector # 538 FLEXCAN_BUF_12_15 FlexCAN_1 */
	/* Vector # 539 FLEXCAN_BUF_16_31 FlexCAN_1 */
	mpc5744_can_rx(1);
}

void can1_tx_isr_handler(void){
	/* Vector # 540 FLEXCAN_BUF_32_39 FlexCAN_1 */
	/* Vector # 541 FLEXCAN_BUF_40_47 FlexCAN_1 */
	/* Vector # 542 FLEXCAN_BUF_48_55 FlexCAN_1 */
	/* Vector # 543 FLEXCAN_BUF_56_63 FlexCAN_1 */
	mpc5744_can_tx(1);
}

void can1_busoff_isr(void){
	mpc5744_can_error_process(1);
}


void can2_rx_isr_handler(void){
	/* Vector # 548 FLEXCAN_BUF_00_03 FlexCAN_2 */
	/* Vector # 549 FLEXCAN_BUF_04_07 FlexCAN_2 */
	/* Vector # 550 FLEXCAN_BUF_08_11 FlexCAN_2 */
	/* Vector # 551 FLEXCAN_BUF_12_15 FlexCAN_2 */
	/* Vector # 552 FLEXCAN_BUF_16_31 FlexCAN_2 */
	mpc5744_can_rx(2);
}

void can2_tx_isr_handler(void){
	/* Vector # 553 FLEXCAN_BUF_32_39 FlexCAN_2 */
	/* Vector # 554 FLEXCAN_BUF_40_47 FlexCAN_2 */
	/* Vector # 555 FLEXCAN_BUF_48_55 FlexCAN_2 */
	/* Vector # 556 FLEXCAN_BUF_56_63 FlexCAN_2 */
	mpc5744_can_tx(2);
}


void can2_busoff_isr(void){
	mpc5744_can_error_process(2);
}


void mpc5744_can_init(int32_t nbModule){
	   	uint32_t i = 0;
		uint32_t clock_freq = 0;
		uint8_t prescale = 0;
		uint32_t baudrate = 0;
		uint32_t sample_rate = 0;	/* sample_rate = (seg1 + 1)/(seg1 + seg2 + 1)*/
		uint32_t seg1 = 0;			/* seg1 = propseg + pseg1 + 2 */
		uint32_t seg2 = 0;			/* seg2 = pseg2 + 1 */
		uint32_t pseg1 = 0;
		uint32_t pseg2 = 0;
		uint32_t propseg = 0;
		uint32_t can_clock = 0;
		uint32_t tq_num = 0;

		if(nbModule > CAN_BUS_NUM){
			return ;
		}

		clock_freq = module[nbModule].clock_freq;
		prescale = module[nbModule].prescale;
		baudrate = module[nbModule].bit_rate;
		sample_rate = module[nbModule].sample_rate;	/* sample_rate = (seg1 + 1)/(seg1 + seg2 + 1)*/

		can_clock = clock_freq/(prescale + 1);
		tq_num = can_clock/baudrate;
		/* comopute pseg1, pseg2, propseg */
		seg1 = (sample_rate * tq_num)/100 - 1;
		seg2 = tq_num - seg1 - 1;

		pseg2 = seg2 - 1;
		pseg1 = (seg1 - 2)/2;
		propseg = seg1 - 2 - pseg1;

		/* init can gpio */
		mpc5744_can_io_cfg(nbModule);

		/* init module */
		module_status[nbModule].busoff_flag = 0;
		module_status[nbModule].error_status = 0;
		module_status[nbModule].module_status = 0;
	    /* CLKSRC is 1 for PLL, MDIS still set after reset */
		CAN[nbModule]->MCR.B.MDIS = 1;			/* FlexCAN module Module Disable, 0: enable, 1: disable */
	    CAN[nbModule]->CTRL1.B.CLKSRC = 1;		/* 0: oscillator clock, 1: peripheral clock */
	    /* Ensure that FlexCan is in FREEZE */
	    CAN[nbModule]->MCR.B.MDIS = 0;			/* FlexCAN module Module Disable, 0: enable, 1: disable */
	    CAN[nbModule]->MCR.B.FRZ = 1;			/* Freeze Enable, 0: not enter freeze mode, 1: enable enter */

	    CAN[nbModule]->MCR.B.HALT = 1;

	    CAN[nbModule]->MCR.B.SOFTRST = 0;
	    CAN[nbModule]->MCR.B.RFEN = 0;          /* Rx FIFO Enable */
	    CAN[nbModule]->MCR.B.SUPV = 1;
	    CAN[nbModule]->MCR.B.WRNEN = 0;
	    CAN[nbModule]->MCR.B.LPMACK = 0;
	    CAN[nbModule]->MCR.B.SRXDIS = 1;		/* self reception disable */
	    CAN[nbModule]->MCR.B.IRMQ = 0;
	    CAN[nbModule]->MCR.B.LPRIOEN = 1;		/* Local Priority enabled */
	    CAN[nbModule]->MCR.B.AEN = 1;			/* abort enable */
	    CAN[nbModule]->MCR.B.IDAM = 0;
	    CAN[nbModule]->MCR.B.MAXMB = (64 - 1);

	    for(i=0; i<2000; i++)
	    {
	        if (CAN[nbModule]->MCR.B.NOTRDY && CAN[nbModule]->MCR.B.FRZACK && !CAN[nbModule]->MCR.B.LPMACK)
	            break;
	    }

	    /** Configuration CTRL1 and CTRL2 */
	    CAN[nbModule]->CTRL1.B.PRESDIV = prescale;       // 40 MHz / (3+1) = 10 MHz
	    CAN[nbModule]->CTRL1.B.RJW = 1;
	    CAN[nbModule]->CTRL1.B.PSEG1 = pseg1;
	    CAN[nbModule]->CTRL1.B.PSEG2 = pseg2;
	    CAN[nbModule]->CTRL1.B.CLKSRC = 1;      /* clock source, 1: peripheral clock, 0: oscillator clock */
	    CAN[nbModule]->CTRL1.B.SMP = 0;	 		/* CAN Bit Sampling */
	    CAN[nbModule]->CTRL1.B.PROPSEG = propseg;

	    CAN[nbModule]->CTRL1.B.ERRMSK = 0;		/* a mask for the Error Interrupt */
	    CAN[nbModule]->CTRL1.B.BOFFMSK = 1;		/* Bus Off interrupt Mask*/
	    CAN[nbModule]->CTRL1.B.TWRNMSK = 0;		/* Tx Warning Interrupt Mask*/
	    CAN[nbModule]->CTRL1.B.RWRNMSK = 0;		/* Rx Warning Interrupt Mask */

	    CAN[nbModule]->CTRL1.B.LPB = 0;         /* Loopback mode is disable */

	    CAN[nbModule]->CTRL1.B.BOFFREC = 1;		/* Bus Off Recovery, 0: Automatic recovering enable, 1: disable */
	    CAN[nbModule]->CTRL1.B.TSYN = 0;		/* Timer Sync, 0: Timer Sync feature disabled, 1: enable */
	    CAN[nbModule]->CTRL1.B.LBUF = 0;		/* Buffer with highest priority is transmitted first. */
	    CAN[nbModule]->CTRL1.B.LOM = 0;			/* Listen-Only Mode, 0: diable, 1: in Listen-Only mode */

	    CAN[nbModule]->CTRL2.B.ECRWRE = 0;
	    CAN[nbModule]->CTRL2.B.WRMFRZ = 1;        // allow SRAM access in freeze mode
	    CAN[nbModule]->CTRL2.B.RFFN = 0;          // 8 Rx FIFO filters
	    CAN[nbModule]->CTRL2.B.TASD = 16;
	    CAN[nbModule]->CTRL2.B.MRP = 0;
	    CAN[nbModule]->CTRL2.B.RRS = 0;
	    CAN[nbModule]->CTRL2.B.EACEN = 0;

	    /* Enable all IRQs */
	    CAN[nbModule]->IMASK1.R = 0xFFFFFFFF;
	    CAN[nbModule]->IMASK2.R = 0xFFFFFFFF;

	    /* Clear message buffers */
	    for(i = 0; i < 64; i++){
	        CAN[nbModule]->MB[i].CS.R = 0;
	        CAN[nbModule]->MB[i].ID.R = 0;
	        CAN[nbModule]->MB[i].DATA.W[0] = 0;
	        CAN[nbModule]->MB[i].DATA.W[1] = 0;
	    }

	    /* Clear RX filters */
	    for(i= 0; i < 64; i++){
	        CAN[nbModule]->RXIMR[i].R = 0;
	    }

	    /* Clear internal buffers */
	    for(i=0; i<24; i++){
	        CAN[nbModule]->RXIMR[i+128].R = 0;
	    }

	    /* Disable RX FIFO message filter (accept all) */
	    CAN[nbModule]->RXMGMASK.R = 0;
	    /* Enable module */
	    CAN[nbModule]->MCR.B.MDIS = 0;
	    CAN[nbModule]->MCR.B.HALT = 0;
	    /* wait for ready */
		for(i=0; i<1000; i++){
	        if (!CAN[nbModule]->MCR.B.NOTRDY && !CAN[nbModule]->MCR.B.FRZACK && !CAN[nbModule]->MCR.B.LPMACK)
	            break;
	    }
		/* active RX filters */
		for(i = 0; i < 32; i ++){
			CAN[nbModule]->RXIMR[i].R = module[nbModule].rx_mask[i];
		}
		/* active RX message buffer */
		for(i = 0; i < 32; i ++){
			CAN[nbModule]->MB[i].CS.B.CODE = MB_RX_ACTIVE;
		}
		/* inactive TX message buffer */
		for(i = 32; i < 64; i ++){
			CAN[nbModule]->MB[i].CS.R = CAN_TX_MB_INACTIVE;
		}

		mpc5744_can_int_init(nbModule);
}




void mpc5744_can_error_check(void){

}


void mpc5744_can_ioctl(uint8_t bus_num){

	/* mpc5744_can_set_mode */
	/* mpc5744_can_set_mask */
	/* mpc5744_can_set_bandrate */
}


void mpc5744_can_send_start(uint16_t bus_num){
	int32_t ret = 0;
	int32_t i = 0;
	struct MPC5744CanFrame frame = {0, 0, 0, 0, 0, 0, 0};
	struct CanMSG msg = {0,{0,0,0,0,0,0,0,0},0};
	uint32_t mailbox_code = 0;

	if(bus_num > CAN_N_DEV){
		return ;
	}
	uint32_t int_flag = CAN[bus_num]->IFLAG2.R;

	if(module_status[bus_num].module_status == 0){
		/* check message buffer, if empty, fill it */
		DI();
		CAN[bus_num]->IFLAG2.R = int_flag;
		/* inactive TX message buffer */
		for(i = 32; i < 64; i ++){
			mailbox_code = CAN[bus_num]->MB[i].CS.R & CAN_TX_MB_ACTIVE;
			if(mailbox_code == CAN_TX_MB_INACTIVE){
				/* */
				ret = can_fifo_out(&msg, &can_tx_fifo[bus_num]);
				if(ret >= 0){
					/* */
					if(msg.id < 0x7FF ){
						frame.IDE = 0;
					}else{
						frame.IDE = 1;
					}

					frame.DLC = msg.dat_len;
					frame.RTR = 0;
					frame.PRIO = 1;
					frame.id = msg.id;
					frame.data1 = msg.data[3] + msg.data[2]*0x100 + msg.data[1]*0x10000 + msg.data[0]*0x1000000;
					frame.data2 = msg.data[7] + msg.data[6]*0x100 + msg.data[5]*0x10000 + msg.data[4]*0x1000000;

					CAN[bus_num]->MB[i].CS.R = CAN_TX_MB_INACTIVE;
					CAN[bus_num]->MB[i].CS.B.SRR = 1;
					CAN[bus_num]->MB[i].CS.B.IDE = frame.IDE;
					CAN[bus_num]->MB[i].CS.B.RTR = frame.RTR;
					CAN[bus_num]->MB[i].CS.B.DLC = frame.DLC;

					if(frame.IDE == 1){
						CAN[bus_num]->MB[i].ID.R = frame.id;
					}else{
						CAN[bus_num]->MB[i].ID.B.ID_STD = frame.id;
					}

					CAN[bus_num]->MB[i].ID.B.PRIO = frame.PRIO;
					CAN[bus_num]->MB[i].DATA.W[0] = frame.data1;
					CAN[bus_num]->MB[i].DATA.W[1] = frame.data2;

					/* check ESR1.TX */
					CAN[bus_num]->MB[i].CS.R |= CAN_TX_MB_ACTIVE;  	/*After transmission, After transmission, the MB automaticallyreturns to the INACTIVE state */

				}else{
					/* fifo empty */
					break;
				}
			}/* check message buffer end */
		}/* for end */
		EI();
	}

}

/*
SYNCH TWRNINT RWRNINT BIT1ERR BIT0ERR ACKERR CRCERR FRMERR STFERR TXWRN RXWRN IDLE TX FLTCONF RX BOFFINT ERRINT 0
*/

uint32_t mpc5744_can_get_status(uint16_t bus_num){
	return module_status[bus_num].busoff_flag;
}

void mpc5744_can_recover(uint16_t bus_num){
	mpc5744_can_init(bus_num);
}


