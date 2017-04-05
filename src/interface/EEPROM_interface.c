/*
* LICENSE:
* Copyright (c) 2016 CheHeJia technology
*
* Composed By: chj1541
* Dated      : 2017Äê2ÔÂ9ÈÕ
* Compiler   : gcc
* FILE NAME  : EEPROM_interface.c
*
* DESCRIPTION:
*
*/

#include "derivative.h" /* include peripheral declarations */
#include "user_cfg.h"
#include "EEPROM_interface.h"
/*****************************************************************
Global variables
******************************************************************/
/* cache configuration */
UINT32 table[EE_CACHE_SIZE/4];
CACHE_TABLE cTable =
{
    (UINT32)table,      /* cache start address */
    sizeof(table)       /* cache size */
};

/* block 0 configuration */
BLOCK_CONFIG blockConf0 =
{
    BLOCK0_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK0_OFFSET,       /* the block start address */
    BLOCK0_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK0_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK0_PARTITION                       /* Partition number of selelcted blocks */
};

/* block 1 configuration */
BLOCK_CONFIG blockConf1 =
{
    BLOCK1_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK1_OFFSET,       /* the block start address */
    BLOCK1_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK1_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK1_PARTITION                       /* Partition number of selelcted blocks */
};

/* block 2 configuration */
BLOCK_CONFIG blockConf2 =
{
    BLOCK2_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK2_OFFSET,       /* the block start address */
    BLOCK2_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK2_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK2_PARTITION                       /* Partition number of selelcted blocks */
};

/* block 3 configuration */
BLOCK_CONFIG blockConf3 =
{
    BLOCK3_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK3_OFFSET,       /* the block start address */
    BLOCK3_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK3_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK3_PARTITION                       /* Partition number of selelcted blocks */
};
/* block configuration array */
BLOCK_CONFIG* blockConfig[4] =
{
    &blockConf0,        /* block 0 structure address */
    &blockConf1,        /* block 1 structure address */
    &blockConf2,        /* block 2 structure address */
    &blockConf3         /* block 3 structure address */
};
/* EEPROM configuration */
EEPROM_CONFIG eepromConf =
{
    NUMBER_OF_BLOCK,  /* the number of blocks being used emulation */
    0,                /* the number of dead block */
    0,                /* the active block index */
    FALSE,            /* the writing flag of the blocks */
    &cTable,          /* the cache table structure */
    blockConfig,      /* block configuration array */
    TRUE              /* the flag to enable/disable the cache table */
};


extern void DisableFlashControllerCache(UINT32 *origin_pflash_pfcr1,
                                 UINT32 *origin_pflash_pfcr2);

extern void RestoreFlashControllerCache(UINT32 pflash_pfcr1,
                                 UINT32 pflash_pfcr2);

extern void dcache_enable(void);
extern void dcache_disable(void);

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t eeprom_init(void){
	uint32_t ret = 0, i = 0;
	uint32_t pflash_pfcr1 = 0, pflash_pfcr2 = 0;

	/* Unlock all eeprom blocks */
	UNLOCK_EBLOCKS
	/* Disable Data cache if present */
	dcache_disable();
    /* Invalidate flash controller cache */
    DisableFlashControllerCache(&pflash_pfcr1,&pflash_pfcr2);
    /* Init eeprom cache */
    for(i = 0; i < EE_CACHE_SIZE; i = i+4)
    {
    	WRITE32(cTable.startAddress + i, 0xFFFFFFFF);
    }
    /*---------------------------------------------------------------------*/
     /* Init eeprom, will try 5 times if failed */
     /*---------------------------------------------------------------------*/
     i = 0;
     ret = !EE_OK;
     while ((i <5) && (ret != EE_OK))
     {
         ret = FSL_InitEeprom(&eepromConf,NULL_CALLBACK);
         i++;
     }

     if(ret == EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD ||
    	ret == EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN){
    	 ret = EEPROM_ERR_BLOCK;
     }else if((ret == EE_ERROR_WRITE_IN_PROGRESS) ||
    		 (ret == EE_INFO_HVOP_INPROGRESS)){
    	 ret = EEPROM_ERR_BUSY;
     }else if(ret == EE_ERROR_NO_ENOUGH_SPACE){
    	 ret = EEPROM_ERR_NO_ENOUGH_SPACE;
     }else if(ret == EE_ERROR_PROGRAM_BLOCK_INDICATOR ||
    		 ret == EE_ERROR_PROGRAM_ERASE_CYCLE){
    	 ret = EEPROM_ERR_INIT;
     }else if(ret == EE_ERROR_BLOCK_CONFIG){
    	 ret = EEPROM_ERR_CONFIG;
     }else{
    	 ret = EEPROM_OK;
     }

    /* Restore flash controller cache */
    RestoreFlashControllerCache(pflash_pfcr1,pflash_pfcr2);
    /* Enable Data cache if present */
    dcache_enable();
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t eeprom_read(uint16_t data_id, uint16_t data_size, uint32_t *p_buff){
	uint32_t ret = 0;
	uint32_t pflash_pfcr1 = 0, pflash_pfcr2 = 0;
    /* Invalidate flash controller cache */
    DisableFlashControllerCache(&pflash_pfcr1,&pflash_pfcr2);

    ret = FSL_ReadEeprom(&eepromConf,data_id,data_size,(UINT32)p_buff, NULL_CALLBACK);

    if(ret == EE_ERROR_DATA_NOT_FOUND){
    	ret = EEPROM_ERR_NO_RECORD;
    }else if((ret == EE_ERROR_WRITE_IN_PROGRESS) || (ret == EE_INFO_HVOP_INPROGRESS)){
    	ret = EEPROM_ERR_BUSY;
    }else{
    	ret = EEPROM_OK;
    }

    /* Restore flash controller cache */
    RestoreFlashControllerCache(pflash_pfcr1,pflash_pfcr2);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t eeprom_write(uint16_t data_id, uint16_t data_size, uint32_t *p_buff){
	uint32_t ret = 0;
	uint32_t pflash_pfcr1 = 0, pflash_pfcr2 = 0;
	/* Disable Data cache if present */
	dcache_disable();
    /* Invalidate flash controller cache */
    DisableFlashControllerCache(&pflash_pfcr1,&pflash_pfcr2);

    ret = FSL_WriteEeprom(&eepromConf,data_id,data_size,(UINT32)p_buff, NULL_CALLBACK);
    if(ret == EE_OK)
    {
    	/*  Call to FSL_MainFunction if there is a swap operation */
		while(eraseStatus_Flag == ERASE_INPROGRESS)
		{
			ret = FSL_MainFunction(&eepromConf,NULL_CALLBACK);
		}

		if((eraseStatus_Flag == ERASE_SWAPERROR) ||  \
			(ret == EE_ERROR_PROGRAM_ERASE_CYCLE) || \
			(ret == EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN) || \
			(ret == EE_ERROR_PROGRAM_BLOCK_INDICATOR) )
		{
			ret = EEPROM_ERR_INIT;	/* Call FSL_InitEeprom to synchronize EEPROM system */
		}else if((ret == EE_OK) || (ret == EE_MAKE_DEAD_OK)){
			ret = EEPROM_OK;
		}else{
			ret = EEPROM_ERR_BUSY;
		}
     }else if(ret == EE_ERROR_NO_ENOUGH_SPACE){
    	 ret = EEPROM_ERR_NO_ENOUGH_SPACE;
     }else{
    	 ret = EEPROM_ERR_BUSY;
     }

    /* Restore flash controller cache */
    RestoreFlashControllerCache(pflash_pfcr1,pflash_pfcr2);
    /* Enable Data cache if present */
    dcache_enable();

	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t eeprom_delete_record(uint16_t data_id){
	uint32_t ret = 0;
	uint32_t pflash_pfcr1 = 0, pflash_pfcr2 = 0;
    /* Invalidate flash controller cache */
    DisableFlashControllerCache(&pflash_pfcr1,&pflash_pfcr2);

    /*---------------------------------------------------------------------*/
    /* Delete data from eeprom                                             */
    /*---------------------------------------------------------------------*/
    ret = FSL_DeleteRecord(&eepromConf,data_id,NULL_CALLBACK);

    if(ret == EE_OK){
    	ret = EEPROM_OK;
    }else if(ret == EE_ERROR_DATA_NOT_FOUND){
    	ret = EEPROM_ERR_NO_RECORD;
    }else{
    	ret = EEPROM_ERR_BUSY;
    }
    /* Restore flash controller cache */
    RestoreFlashControllerCache(pflash_pfcr1,pflash_pfcr2);
	return ret;
}

/*
**-----------------------------------------------------------------------------
**
**  Function Name:
**	Function Description:
**		This function is bus channel analog value
**
**  Inputs:
**		None
**	Outputs:
**		None
**  Returns:
**
**	Notes:
**
**-----------------------------------------------------------------------------
*/
uint32_t eeprom_erase(void){
	uint32_t ret = 0;
	uint32_t pflash_pfcr1 = 0, pflash_pfcr2 = 0;

	/* Unlock all eeprom blocks */
	UNLOCK_EBLOCKS
	/* Disable Data cache if present */
	dcache_disable();
    /* Invalidate flash controller cache */
    DisableFlashControllerCache(&pflash_pfcr1,&pflash_pfcr2);
    /* Erase all eeprom blocks*/
    ret = FSL_RemoveEeprom(&eepromConf, NULL_CALLBACK);

    if(ret == EE_ERROR_BLOCK_STATUS){
    	ret = EEPROM_ERR_CONFIG;
    }else if((ret == EE_ERROR_WRITE_IN_PROGRESS) || (ret == EE_INFO_HVOP_INPROGRESS)){
    	ret = EEPROM_ERR_BUSY;
    }else{
    	ret = EEPROM_OK;
    }
    /* Restore flash controller cache */
    RestoreFlashControllerCache(pflash_pfcr1,pflash_pfcr2);
    /* Enable Data cache if present */
    dcache_enable();

	return ret;
}



















