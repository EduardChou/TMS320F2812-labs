/****************************************************************************/
/* SINEWAVE.CMD - COMMAND FILE FOR LINKING TMS27xx/TMS28xx C PROGRAMS       */
/*                                                                          */
/*   Description: This file is a sample command file that can be used       */
/*                for linking programs built with the TMS320C28xx C         */
/*                Compiler.   Use it as a guideline; you may want to change */
/*                the allocation scheme according to the size of your       */
/*                program and the memory layout of your target system.      */
/****************************************************************************/
-stack 512
MEMORY
{
PAGE 0 : PROG(R)     : origin = 0x3D8000, length = 0x20000
PAGE 0 : BOOT(R)     : origin = 0x3FF000, length = 0xFC0   
PAGE 0 : RESET(R)    : origin = 0x3FFFC0, length = 0x2
PAGE 0 : VECS(R)  	 : origin = 0x3FFFC2, length = 0x3E
PAGE 0 : PIEVECS(R)  : origin = 0x000D00, length = 0xFF
PAGE 0 : H0RAM(RW)   : origin = 0x3F8000, length = 0x2000

PAGE 1 : M0RAM(RW)   : origin = 0x000000, length = 0x400
PAGE 1 : M1RAM(RW)   : origin = 0x000400, length = 0x400
PAGE 1 : L0L1RAM(RW) : origin = 0x008000, length = 0x2000

}
 
SECTIONS
{
   /* 22-bit program sections */
   .reset   : > RESET,   PAGE = 0
    vectors : > VECS, 	 PAGE = 0
   .pinit   : > H0RAM,   PAGE = 0
   .cinit   : > H0RAM,   PAGE = 0
   .text    : > H0RAM,   PAGE = 0
   FFTtf	>	H0RAM, 	 PAGE = 0 
   
   /* 16-Bit data sections */
   .const   : > L0L1RAM, PAGE = 1
   .bss     : > L0L1RAM, PAGE = 1
   .stack   : > M1RAM, 	 PAGE = 1
   .sysmem  : > M0RAM, 	 PAGE = 1
   
   FFTipcb	ALIGN(128)  : { } >    L0L1RAM PAGE 1         

   /* 32-bit data sections */
   .ebss    : > L0L1RAM, PAGE = 1
   .econst  : > L0L1RAM, PAGE = 1
   .esysmem : > L0L1RAM, PAGE = 1
}
