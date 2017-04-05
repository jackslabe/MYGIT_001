/**HEADER********************************************************************
*
* Copyright (c) 2015 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: ee_C55_utils.c$
* $Version : 1.2.1$
* $Date    : Oct-19-2015$
*
* Comments:
*
*   Contains disable/enable flash controller cache.
*
*END************************************************************************/
#include "ee_emulation.h"

#ifdef MPC5777C
    #define FLASH_FMC             0xFFF68000
#else
    #define FLASH_FMC             0xFC030000
#endif

    /* Offsets of Platform Flash Configuration Register 1, 2 */
    #define FLASH_PFCR1           0             /* Offset address of PFCR1 */
    #define FLASH_PFCR2           4             /* Offset address of PFCR2 */
    #define FLASH_FMC_P0BFEN      0x00000001    /* Enable P0BFEN bit */
    #define FLASH_FMC_P1BFEN      0x00000001    /* Enable P1BFEN bit */

/*****************************************************************
*                        Disable Flash Cache                     *
******************************************************************/
void DisableFlashControllerCache(UINT32 *origin_pflash_pfcr1,
                                 UINT32 *origin_pflash_pfcr2)
{
    /* Read the values of two registers FLASH_PFCR1 and FLASH_PFCR2 */
    *origin_pflash_pfcr1 = READ32(FLASH_FMC + FLASH_PFCR1);
    *origin_pflash_pfcr2 = READ32(FLASH_FMC + FLASH_PFCR2);

    /* Disable Caches */
    CLEAR32(FLASH_FMC + FLASH_PFCR1, FLASH_FMC_P0BFEN);
    CLEAR32(FLASH_FMC + FLASH_PFCR2, FLASH_FMC_P1BFEN);
}

/*****************************************************************
*               Restore configuration register of FCM            *
******************************************************************/
void RestoreFlashControllerCache(UINT32 pflash_pfcr1,
                                 UINT32 pflash_pfcr2)
{
    WRITE32(FLASH_FMC + FLASH_PFCR1, pflash_pfcr1);
    WRITE32(FLASH_FMC + FLASH_PFCR2, pflash_pfcr2);
}


/*********************************** End of file */























