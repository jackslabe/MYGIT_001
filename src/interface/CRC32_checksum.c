/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê3ÔÂ22ÈÕ
* Compiler   : gcc
* FILE NAME  : CRC32_checksum.c
*
* DESCRIPTION:
*
*/
#include "derivative.h" /* include peripheral declarations */
#include "CRC32_checksum.h"
static uint32_t   crc32_table[256];


void init_table(void)
{
    int32_t   i = 0,j = 0;
    uint32_t   crc;
    for(i = 0;i < 256;i++)
    {
         crc = i;
        for(j = 0;j < 8;j++)
        {
            if(crc & 1)
            {
                 crc = (crc >> 1) ^ 0xEDB88320;
            }
            else
            {
                 crc = crc >> 1;
            }
        }
        crc32_table[i] = crc;
    }
}


uint32_t compute_crc32(uint8_t *buf, uint32_t len)
{
    uint32_t ret = 0xFFFFFFFF;
    uint32_t i = 0;

    for(i = 0; i < len;i++)
    {
         ret = crc32_table[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
    }
     ret = ~ret;
    return ret;
}


uint32_t compute_crc32_step(uint8_t *buf, uint32_t len, uint8_t step)
{
    static uint32_t ret = 0xFFFFFFFF;
    uint32_t i = 0;

    switch(step){
    case 1:
    	/* continue */
    	/* start crc check sum */
        for(i = 0; i < len;i++)
        {
             ret = crc32_table[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
        }
    	break;
    case 2:
    	/* crc check end */
        for(i = 0; i < len;i++)
         {
              ret = crc32_table[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
         }
     	break;
    	ret = ~ret;
    	ret = 0xFFFFFFFF;
    	break;
    default:
    	break;
    }

    return ret;
}





