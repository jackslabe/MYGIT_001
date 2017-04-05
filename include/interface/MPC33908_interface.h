/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2016Äê11ÔÂ3ÈÕ
* Compiler   : gcc
* FILE NAME  : MPC33908_interface.h
*
* DESCRIPTION:
*
*/
#ifndef MPC33908_INTERFACE_H_
#define MPC33908_INTERFACE_H_

/* User configuration */
#define WU1_CFG 	(WU_CFG_IO0_FALLING_EDGE|WU_CFG_IO1_RISING_EDGE|\
					 WU_CFG_IO2_NO_WAKEUP|INT_INHIBIT_IO1|INT_INHIBIT_IO2)

#define WU2_CFG		(WU_CFG_IO3_NO_WAKEUP|WU_CFG_IO4_RISING_EDGE|\
					 WU_CFG_IO5_RISING_EDGE|INT_INHIBIT_IO45)

#define FSSM2_CFG	0x40

#define WD_WINDOW_CFG	WD_WIDOW_8_MS

/* User configuration end */

/*|WU_0|WU_0|WU_1|WU_1|WU_2|WU_2|INT_IO_1 |INT_IO_0 | */
/*|WU_3|WU_3|WU_4|WU_4|WU_5|WU_5|INT_IO_23|INT_IO_45| */

#define WU_CFG_IO0_NO_WAKEUP		0x00
#define WU_CFG_IO0_RISING_EDGE		0x40
#define WU_CFG_IO0_FALLING_EDGE		0x80
#define WU_CFG_IO0_ANY_EDGE			0xC0

#define WU_CFG_IO1_NO_WAKEUP		0x00
#define WU_CFG_IO1_RISING_EDGE		0x10
#define WU_CFG_IO1_FALLING_EDGE		0x20
#define WU_CFG_IO1_ANY_EDGE			0x30

#define WU_CFG_IO2_NO_WAKEUP		0x00
#define WU_CFG_IO2_RISING_EDGE		0x04
#define WU_CFG_IO2_FALLING_EDGE		0x08
#define WU_CFG_IO2_ANY_EDGE			0x0C

#define WU_CFG_IO3_NO_WAKEUP		0x00
#define WU_CFG_IO3_RISING_EDGE		0x40
#define WU_CFG_IO3_FALLING_EDGE		0x80
#define WU_CFG_IO3_ANY_EDGE			0xC0

#define WU_CFG_IO4_NO_WAKEUP		0x00
#define WU_CFG_IO4_RISING_EDGE		0x10
#define WU_CFG_IO4_FALLING_EDGE		0x20
#define WU_CFG_IO4_ANY_EDGE			0x30

#define WU_CFG_IO5_NO_WAKEUP		0x00
#define WU_CFG_IO5_RISING_EDGE		0x04
#define WU_CFG_IO5_FALLING_EDGE		0x08
#define WU_CFG_IO5_ANY_EDGE			0x0C

#define INT_INHIBIT_IO1				0x02
#define INT_INHIBIT_IO2				0x01

#define INT_INHIBIT_IO23			0x02
#define INT_INHIBIT_IO45			0x01

#define WIDE_RANG_V5_0_RATIO_5		5
#define WIDE_RANG_V3_3_RATIO_7		7
#define TIGHT_RANG_V5_0_RATIO_2		2
#define TIGHT_RANG_V3_3_RATIO_3		3
#define MUX_TP_CO					9.9		/* mV/¡ãC */
#define MUX_TP						2.15	/* 2.15 V */

#define MUX_VREF			0
#define MUX_VSNS_WIDE_RANG	1
#define MUX_IO_0_WIDE_RANG	2
#define MUX_IO_1_WIDE_RANG	3
#define MUX_VSNS_TIGHT_RANG	4
#define MUX_IO_0_TIGHT_RANG	5
#define MUX_IO_1_TIGHT_RANG	6
#define MUX_TEMPRATURE		7


#define WD_WIDOW_DISABLE	0x00
#define WD_WIDOW_1_MS		0x10 /* 1 */
#define WD_WIDOW_2_MS		0x20
#define WD_WIDOW_3_MS		0x30
#define WD_WIDOW_4_MS		0x40
#define WD_WIDOW_6_MS		0x50
#define WD_WIDOW_8_MS		0x60
#define WD_WIDOW_12_MS		0x70
#define WD_WIDOW_16_MS		0x80
#define WD_WIDOW_24_MS		0x90
#define WD_WIDOW_32_MS		0xA0
#define WD_WIDOW_64_MS		0xB0
#define WD_WIDOW_128_MS		0xC0
#define WD_WIDOW_256_MS		0xD0
#define WD_WIDOW_512_MS		0xE0
#define WD_WIDOW_1024_MS	0xF0



struct mpc33908_wakeup_source{
	uint8_t phy_wu:1;
	uint8_t io_0_wu:1;
	uint8_t io_1_wu:1;
	uint8_t io_2_wu:1;
	uint8_t io_3_wu:1;
	uint8_t io_4_wu:1;
	uint8_t io_5_wu:1;
};

struct mpc33908_io_status{
	uint8_t io_0:1;
	uint8_t io_1:1;
	uint8_t io_2:1;
	uint8_t io_3:1;
	uint8_t io_4:1;
	uint8_t io_5:1;
};

struct mpc33908_diag_dat{
	uint8_t vreg1_status;
	uint8_t vreg2_status;
	uint8_t vreg1_diag;
	uint8_t vreg2_diag;
	uint8_t vreg3_diag;
	uint8_t can_diag;
	uint8_t can_lin_diag;
	uint8_t spi_diag;
	uint8_t fs1_diag;
	uint8_t fs2_diag;
	uint8_t wd_counter;
};

void mpc33908_init(void );
void mpc33908_mux_set(uint8_t channel);
void mpc33908_release_fs_out(void);
void mpc33908_feed_watchdog(void);
struct mpc33908_io_status mpc33908_get_io_status(void);
void mpc33908_set_lpoff_mode(void);
int32_t mpc33908_diag(struct mpc33908_diag_dat *p_diag);
struct mpc33908_wakeup_source mpc33908_get_wakeup_source(void);

#endif /* MPC33908_INTERFACE_H_ */
