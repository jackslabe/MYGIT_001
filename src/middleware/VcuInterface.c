/*
 * VcuInterface.c
 *
 *  Created on: 2017Äê1ÔÂ2ÈÕ
 *      Author: chj704
 */

#include "VcuInterface.h"


void VcuRead(void)
{
	int i=0;
	for(i=0;i<96;i++)
	{
		BMS_U.VaIF_VCell_V[i] = 3.5;
		BMS_U.VaIF_TCell_C[i] = 20.0;
	}

	BMS_U.TCell = 20.0;
}


