/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê12ÔÂ27ÈÕ
* Compiler   : gcc
* FILE NAME  : LTC6811_interface.h
*
* DESCRIPTION:
*
*/
#ifndef LTC6811_INTERFACE_H_
#define LTC6811_INTERFACE_H_
/********************************************************************
** broadcast CMD:
** ------|-bit7-|-bit6-|-bit5-|-bit4-|-bit3-|-bit2-|-bit1-|-bit0-| **
** CMD0:-|- 0  -|-  0 -|-  0 -|-  0 -|-  0 -|-CC10-|-CC09-|-CC08-| **
** CMD1:-|-CC07-|-CC06-|-CC05-|-CC04-|-CC03-|-CC02-|-CC01-|-CC00-| **
**
** address CMD:
** ------|-bit7-|-bit6-|-bit5-|-bit4-|-bit3-|-bit2-|-bit1-|-bit0-| **
** CMD0:-|- 1  -|- a3 -|- a2 -|- a1 -|- a0 -|-CC10-|-CC09-|-CC08-| **
** CMD1:-|-CC07-|-CC06-|-CC05-|-CC04-|-CC03-|-CC02-|-CC01-|-CC00-| **
**
*********************************************************************
*/

/* COMMAND CODE: CC00 ~ CC10 */
/* |------------------- CMD0 -----------------------|------------------- CMD1 ----------------------| */
/* | bit15|bit14|bit13|bit12|bit11|bit10|bit09|bit08|bit07|bit06|bit05|bit04|bit03|bit02|bit01|bit00| */
/* |  1/0 |   address/boradcast   |  0  |  1  |MD[1]|MD[0]|  1  |  1  | DCP |  0  |CH[2]|CH[1]|CH[0]| */
/* |  1/0 |   address/boradcast   |  0  |  1  |MD[1]|MD[0]| PUP |  1  | DCP |  1  |CH[2]|CH[1]|CH[0]| */
/* |  1/0 |   address/boradcast   |  1  |  0  |MD[1]|MD[0]|  1  |  1  |  0  |  1  |CHST2|CHST1|CHST0| */
/* |  1/0 |   address/boradcast   |  0  |  1  |MD[1]|MD[0]|ST[1]|ST[0]|  0  |  0  |  1  |  1  |  1  | */
/* |  1/0 |   address/boradcast   |  1  |  0  |MD[1]|MD[0]|ST[1]|ST[0]|  0  |  1  |  1  |  1  |  1  | */
/* |  1/0 |   address/boradcast   |  1  |  1  |  1  |  0  |  0  |  0  |  1  |  0  |  0  |  0  |  1  | */
/* |  1/0 |   address/boradcast   |  1  |  1  |  1  |  0  |  0  |  0  |  1  |  0  |  0  |  1  |  1  | */
/* |  1/0 |   address/boradcast   |  1  |  1  |  1  |  0  |  0  |  0  |  1  |  0  |  1  |  0  |  0  | */
/* |  1/0 |   address/boradcast   |  1  |  1  |  1  |  0  |  0  |  0  |  1  |  0  |  1  |  0  |  1  | */

#define CMD_WRADCV	(0x0260)	/* start cell voltage ADC conversion command */
#define CMD_WRADOW	(0x0228)	/* start open wire ADC conversion command */
#define CMD_WRCVST	(0x0207)	/* start self-test cell voltage conversion command */
#define CMD_WRADSTAT	(0x0468)	/* start status group ADC conversion command */
#define CMD_WRSTATST	(0x040F)	/* start self-test status group conversion command */
#define CMD_WRCLRCELL	(0x0711)	/* clear cell voltage register command */
#define CMD_WRCLRSTAT	(0x0713)	/* clear status register command */
#define CMD_WRPLADC	(0x0714)		/*  poll ADC conversion command */
#define CMD_WRDIAGN	(0x0715)	/* diagnose MUX and poll status command */
#define CMD_WRCFG	(0x0001)	/* write configuration register command */
#define CMD_RDCFG	(0x0002)	/* read configuration register command */
#define CMD_RDCVA	(0x0004)	/* read cell voltage register groupA command */
#define CMD_RDCVB	(0x0006)	/* read cell voltage register groupB command */
#define CMD_RDCVC	(0x0008)	/* read cell voltage register groupC command */
#define CMD_RDCVD	(0x000A)	/* read cell voltage register groupD command */
#define CMD_RDAUXA	(0x000C)	/* read auxiliary register groupA */
#define CMD_RDAUXB	(0x000E)	/* read auxiliary register groupB */
#define CMD_RDSTATA	(0x0010)	/* read status register groupA command */
#define CMD_RDSTATB	(0x0012)	/* read status register groupB command */
#define CMD_WRSCTRL	(0x0014)	/* write S control register */
#define CMD_RDSCTRL	(0x0016)	/* read S control register */
#define CMD_WRPWM	(0x0020)	/* write PWM register */
#define CMD_RDPWM	(0x0022)	/* read PWM register */

/*********************************************************************
**	|CFGR0->ADCOPT|MD[1]|MD[0]|: ADC mode 422Hz/1/27/14/7/3/2KHz or 26Hz,
**	|     0       |  0  |  0  |: 422Hz mode
**  |     0       |  0  |  1  |: 27KHz(fast)mode
**  |     0       |  1  |  0  |:  7KHz(normal)mode
**  |     0       |  1  |  1  |:  26Hz(filter)mode
**  |     1       |  0  |  0  |:  1KHz mode
**  |     1       |  0  |  1  |: 14KHz mode
**  |     1       |  1  |  0  |:  3KHz mode
**  |     1       |  1  |  1  |:  2KHz mode
**********************************************************************
*/
#define MD_01		(0x0080)
#define MD_10		(0x0100)
#define MD_11		(0x0180)
#define MD_00		(0x0000)	/*clear bit 00*/

/* discharge permit: '0' is not permitted, '1' is permitted*/
#define DCP_ON		(0x0010)
#define DCP_OFF		(0x0000)

/* pull-up('1')/pull-down('0') current for open-wire conversions*/
#define PUP_UP		(0x0040)
#define PUP_DOWN	(0x0000)

/* self-test mode selection: '01' is test1, '10' is test2 */
#define ST_01		(0x0020)
#define ST_10		(0x0040)
#define ST_CLEAR	(0x0000)

/* status gropu selection: 000:SOC,ITMP,VA,VD. 001: SOC. 010: ITMP. 011:VA. 100:VD*/
#define CH_000	(0x0000)	/* */
#define CH_001	(0x0001)
#define CH_010	(0x0002)
#define CH_011	(0x0003)
#define CH_100	(0x0004)
#define CH_101	(0x0005)
#define CH_110	(0x0006)



/***********************************************************************************
** Configuration Register Group
** -----_-|-bit7-|-bit6-|-bit5-|-bit3-|-bit3-|-bit2-|-bit1-|-bit0-| **
** CFGR0:-|GPIO5-|GPIO4-|GPIO3-|GPIO2-|GPIO1-|REFON |SWTRD |ADCOPT| **
** CFGR1:-|VUV[7]|VUV[6]|VUV[5]|VUV[4]|VUV[3]|VUV[2]|VUV[1]|VUV[0]| **
** CFGR2:-|VOV[3]|VOV[2]|VOV[1]|VOV[0]|VUV[11|VUV[10|VUV[9]|VUV[8]| **
** CFGR3:-|VOV[11|VOV[10|VOV[9]|VOV[8]|VOV[7]|VOV[6]|VOV[3]|VOV[4]| **
** CFGR4:-|-DCC8-|-DCC7-|-DCC6-|-DCC5-|-DCC4-|-DCC3-|-DCC2-|-DCC1-| **
** CFGR5:-|DCTO[4|DCTO[3|DCTO[2|DCTO[1|-DCC12|-DCC11|-DCC10|-DCC9-| **
************************************************************************************
*/
#define REFON_1		(0x04)	/* reference powered up, '1':reference remains power up until watchdog timeout */
#define ADCOPT_1	(0x01)	/* ADC mode option bit: '0'select 27/7KHz or 26Hz. '1'select 14/3/2KHz */
#define DCCL_FF		(0xFF)	/* discharge cell 0~7.*/
#define DCCH_0F		(0x0F)	/* discharge cell 8~11*/
#define DCTO_7		(0x70)	/* discharge time out value. 7: 10min*/

/***************************************************************************************************
** DCTO[1] ~ [4]:
**		|  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |
** Min	| 0.5 |  1  |  2  |  3  |  4  |  5  |  10 |  15 |  20 |  30 |  40 |  60 |  75 |  90 | 120 |
****************************************************************************************************
*/


#define LTC6811_TIME_OUT	50000

#define LTC6811_MAX_CHIP_NUM	16
#define LTC6811_REG_LEN			8				/* data 0 ~ data 5, pec0, pec1 */
#define LTC6811_CMD_LEN			4				/* cmd0, cmd1, pec0, pec1 */
#define LTC6811_BUF_LEN			(LTC6811_CMD_LEN + LTC6811_REG_LEN * LTC6811_MAX_CHIP_NUM)

enum LTC6811_REG_LIST{
	CFGR,
	CVAR,
	CVBR,
	CVCR,
	CVDR,
	AVAR,
	AVBR,
	STAR,
	STBR,
	COMM,
	SCTRL,
	PWM,
	REG_NUM
};

enum LTC6811_CMD_LIST{
	WRCFGA = 0x0001,
	RDCFGA = 0x0002,
	RDCVA = 0x0004,
	RDCVB = 0x0006,
	RDCVC = 0x0008,
	RDCVD = 0x000A,
	RDAUXA = 0x000C,
	RDAUXB = 0x000E,
	RDSTATA = 0x0010,
	RDSTATB = 0x0012,
	WRSCTRL = 0x0014,
	WRPWM = 0x0020,
	RDSCTRL = 0x0016,
	RDPWM = 0x0022,
	STSCTRL = 0x0019,
	CLRSCTRL = 0x0018,
	ADCV = 0x0260,
	ADOW = 0x0228,
	CVST = 0x0207,
	ADOL = 0x0201,
	ADAX = 0x0460,
	ADAXD = 0x0400,
	AXST = 0x0407,
	ADSTAT = 0x0468,
	ADSTATD = 0x0406,
	STATST = 0x040F,
	ADCVAX = 0x046F,
	ADCVSC = 0x0467,
	CLRCELL = 0x0711,
	CLRAUX = 0x0712,
	CLRSTAT = 0x0713,
	PLADC = 0x0714,
	DIAGN = 0x0715,
	WRCOMM ,
	RDCOMM ,
	STCOMM ,
	CMD_END
};



struct ChipREG{
	uint8_t dat[LTC6811_REG_LEN];		/* data 0 ~ data 5, pec0, pec1 */
};

struct ChipCMD{
	uint8_t dat[LTC6811_CMD_LEN];		/* cmd0, cmd1, pec0, pec1 */
};

struct LTC6811DatFrame{
	struct ChipCMD cmd;
	struct ChipREG reg;
};

struct LTC6811Reg{
	struct ChipREG reg[REG_NUM];
};


void LTC6811_init(void);
int32_t LTC6811_RDCV(uint16_t chip_num, struct LTC6811Reg *p_dev);
int32_t LTC6811_RDAUX(uint16_t chip_num, struct LTC6811Reg *p_dev);
int32_t LTC6811_RDSTAT(uint16_t chip_num, struct LTC6811Reg *p_dev);
int32_t LTC6811_RDSCTRL(uint16_t chip_num, struct LTC6811Reg *p_dev);
int32_t LTC6811_RDPWM(uint16_t chip_num, struct LTC6811Reg *p_dev);

int32_t LT6811_START_ADCV(uint16_t parameter);
int32_t LT6811_START_ADOW(uint16_t parameter);
int32_t LT6811_START_ADOL(uint16_t parameter);
int32_t LT6811_START_ADAX(uint16_t parameter);
int32_t LT6811_START_ADAXD(uint16_t parameter);
int32_t LT6811_START_SELFTEST_CV(uint16_t parameter);
int32_t LT6811_START_SELFTEST_GPIO(uint16_t parameter);
int32_t LT6811_START_SELFTEST_STAT(uint16_t parameter);
int32_t LT6811_START_ADSTAT(uint16_t parameter);
int32_t LT6811_START_ADSTATD(uint16_t parameter);
int32_t LT6811_START_ADCVAX(uint16_t parameter);
int32_t LT6811_START_ADCVSC(uint16_t parameter);
int32_t LT6811_CLRSCTRL(uint16_t parameter);
int32_t LT6811_CLRCELL(uint16_t parameter);
int32_t LT6811_CLRAUX(uint16_t parameter);
int32_t LT6811_CLRSTAT(uint16_t parameter);
int32_t LT6811_DIAG_MUX(void);
int32_t LTC6811_SET_GPIO5(uint16_t chip_num, struct LTC6811Reg *p_dev);

int32_t LT6811_WRCFGA(uint16_t chip_num, struct LTC6811Reg *p_dev);
int32_t LT6811_WRSCTRL(uint16_t chip_num, struct LTC6811Reg *p_dev);
int32_t LT6811_WRPWM(uint16_t chip_num, struct LTC6811Reg *p_dev);









#endif /* LTC6811_INTERFACE_H_ */
