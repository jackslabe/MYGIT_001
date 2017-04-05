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
* $FileName: ee_blocks.h$
* $Version : 1.2.0$
* $Date    : Jul-21-2015$
*
* Comments:
*
*   Provide information about blocks used for emulation
*
*END************************************************************************/

#ifndef _EE_BLOCKS_H_
#define _EE_BLOCKS_H_
#include "ee_emulation.h"

#define FLASH_REG_BASE              0xFFFE0000
#define LOCK0_REG_OFFSET            0x00000010
#define LOCK1_REG_OFFSET            0x00000014

#define MAIN_ARRAY_BASE             0x00800000              /* Base address for EE blocks */

/* Low space block 0 */
#define BLOCK0_OFFSET               0x00000000              /* Offset of low block 0 */
#define BLOCK0_SIZE                 0x4000                  /* 16KB size */
#define BLOCK0_MASK                 0x00000001
#define BLOCK0_PARTITION            (0x1<<0)
#define BLOCK0_ADD_SPACE            LOW_ADDRESS_SPACE

/* Low space block 1 */                 
#define BLOCK1_OFFSET               0x00004000              /* For Cut 2 only */
#define BLOCK1_SIZE                 0x4000                  /* For Cut 2 only */
#define BLOCK1_MASK                 0x00000002              /* For Cut 2 only */
#define BLOCK1_PARTITION            (0x1<<0)                /* For Cut 2 only */
#define BLOCK1_ADD_SPACE            LOW_ADDRESS_SPACE       /* For Cut 2 only */

/* Mid space block 0 */                
#define BLOCK2_OFFSET               0x00008000              /* For Cut 2 only */
#define BLOCK2_SIZE                 0x8000                  /* For Cut 2 only */
#define BLOCK2_MASK                 0x00000001              /* For Cut 2 only */
#define BLOCK2_PARTITION            (0x1<<2)                /* For Cut 2 only */
#define BLOCK2_ADD_SPACE            MID_ADDRESS_SPACE       /* For Cut 2 only */
                                       
/* Mid space block 1 */                
#define BLOCK3_OFFSET               0x00010000              /* For Cut 2 only */
#define BLOCK3_SIZE                 0x8000                  /* For Cut 2 only */
#define BLOCK3_MASK                 0x00000002              /* For Cut 2 only */
#define BLOCK3_PARTITION            (0x1<<3)                /* For Cut 2 only */
#define BLOCK3_ADD_SPACE            MID_ADDRESS_SPACE       /* For Cut 2 only */

#define UNLOCK_EBLOCKS              CLEAR32(FLASH_REG_BASE + LOCK0_REG_OFFSET, 0x00030003); /* For Cut 2 only */

#ifdef CUT1
/* Mid space block 0 */
#define BLOCK2_OFFSET               0x00004000              /* For Cut 1 only */
#define BLOCK2_SIZE                 0x8000                  /* For Cut 1 only */
#define BLOCK2_MASK                 0x00000001              /* For Cut 1 only */
#define BLOCK2_PARTITION            (0x1<<2)                /* For Cut 1 only */
#define BLOCK2_ADD_SPACE            MID_ADDRESS_SPACE       /* For Cut 1 only */

/* Mid space block 1 */
#define BLOCK3_OFFSET               0x0000C000              /* For Cut 1 only */
#define BLOCK3_SIZE                 0x8000                  /* For Cut 1 only */
#define BLOCK3_MASK                 0x00000002              /* For Cut 1 only */
#define BLOCK3_PARTITION            (0x1<<3)                /* For Cut 1 only */
#define BLOCK3_ADD_SPACE            MID_ADDRESS_SPACE       /* For Cut 1 only */

#define UNLOCK_EBLOCKS              CLEAR32(FLASH_REG_BASE + LOCK0_REG_OFFSET, 0x00010003); /* For Cut 1 only */
#endif /* CUT1 */

/* Block address bases  */
#define BASE_ADDR_aL_a16K           0x00800000
#define BASE_ADDR_aL_a32K           0xFFFFFFFF
#define BASE_ADDR_aL_a64K           0xFFFFFFFF
#define BASE_ADDR_aM_a16K           0xFFFFFFFF
#define BASE_ADDR_aM_a32K           0x00808000
#define BASE_ADDR_aM_a64K           0x00FA0000
#define BASE_ADDR_aH_a16K           0xFFFFFFFF
#define BASE_ADDR_aH_a32K           0xFFFFFFFF
#define BASE_ADDR_aH_a64K           0x00FA0000

#endif /* _EE_BLOCKS_H_ */
