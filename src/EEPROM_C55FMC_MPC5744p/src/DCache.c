/**HEADER********************************************************************
*
* Copyright (c) 2014 Freescale Semiconductor;
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
* $FileName: ee_demo.c$
* $Version : 1.12$
* $Date    : Dec-19-2014$
*
* Comments:
*
*   
*
*END************************************************************************/

#include "derivative.h" /* include peripheral declarations */

#define E200CORE_SYNC()         asm(" msync") /* Memory synchronize  */
#define E200CORE_ISYNC()        asm ("se_isync") /* Instruction synchronize */
#define E200CORE_L1CSR0         1010 /* L1 Cache Control and Status Register 0 */
#define E200CORE_L1CSR0_CE      0x00000001 /* Data cache enable */


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : dcache_enable
* Returned Value   : none
* Comments         :
*   This function enables the data cache
*
*END*----------------------------------------------------------------------*/

void dcache_enable(void){ /* Body */
   register uint32_t i = 0;
   /* Read L1CSR0 */
   asm("mfspr %0, 1010":"=r"(i));
   /* enable copy-back mode,enable cache */
   i |= E200CORE_L1CSR0_CE;
   /* Instruction Synchronize */
   E200CORE_ISYNC();
   /* Memory Synchronize */
   E200CORE_SYNC();
   /* Write to L1CSR0 */
   asm("mtspr 1010, %0"::"r"(i));
   /* Instruction Synchronize */
   E200CORE_ISYNC();
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : dcache_disable
* Returned Value   : none
* Comments         :
*   This function disables the data cache (if possible)
*
*END*----------------------------------------------------------------------*/

void dcache_disable(void){ /* Body */
   asm(	   "__dcache_cfg: \n\t"
		   "e_li %r5, 0x2 \n\t"
		   "mtspr 1010, %r5 \n\t"
		   "e_li %r7, 0x4  \n\t"
		   "e_li %r8, 0x2  \n\t"
		   "e_lis %r11, 0xFFFFFFFB@h  \n\t"
		   "e_or2i %r11, 0xFFFFFFFB@l  \n\t"
		   "__dcache_inv: \n\t"
		   "mfspr %r9, 1010  \n\t"
		   "and. %r10, %r7, %r9 \n\t"
		   "e_beq __dcache_no_abort \n\t"
		   "and. %r10, %r11, %r9 \n\t"
		   "mtspr 1010, %r10 \n\t"
		   "e_b __dcache_cfg \n\t"
		   "__dcache_no_abort: \n\t"
		   "and. %r10, %r8, %r9 \n\t"
		   "e_bne __dcache_inv \n\t");
} /* Endbody */

/* EOF */
