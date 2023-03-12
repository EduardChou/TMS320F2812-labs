/*
//
//      TMDX BETA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	EzDSP_RAM_lnk.cmd
//
// TITLE:	Linker Command File For F2812 eZdsp examples that run out of RAM
//          This linker file assumes the user is booting up in Jump to H0 mode
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.51| 02 Apr 2002 | L.H. | Original Release.
//  0.56| 20 May 2002 | L.H. | No change
//  0.58| 18 Jun 2002 | L.H. | Added codestart section 
//      |             |      | Changed .reset section type to DSECT
//      |             |      | CPU Timer1 and CPU Timer2 are reserved for BIOS/RTOS
// -----|-------------|------|-----------------------------------------------
//###########################################################################
*/

MEMORY
{
PAGE 0 :

   /* SARAM  H0 is split between PAGE 0 and PAGE 1 */  
   PRAMH0     : origin = 0x3f8000 length = 0x001000
   
   /*  This memory block loaded with the reset vector only if 
       booting from XINTF Zone 7.  Otherwise reset vector is
       fetched from boot ROM. See .reset section below        */
   RESET      : origin = 0x3FFFC0, length = 0x000002           
         
PAGE 1 : 
   /* SARAM                     */     
   RAMM0    : origin = 0x000000, length = 0x00040
   RAMM0_1    : origin = 0x100000, length = 0x03000
   RAMM1    : origin = 0x000400, length = 0x000400
 /* SARAM  H0 is split between PAGE 0 and PAGE 1 */     
   DRAMH0     : origin = 0x3f9000, length = 0x001000   
         
}
 
 -stack 350
SECTIONS
{
   /* Allocate program areas: */
   
   /* Setup for "boot to H0" mode: 
      The codestart section (found in CodeStartBranch.asm)
      re-directs execution to the start of user code.  
      Place this section or the .text section at the
      start of H0  */
   codestart        : > PRAMH0,      PAGE = 0
   .text            : > PRAMH0,      PAGE = 0
   .cinit           : > PRAMH0,      PAGE = 0
   ramfuncs         : > PRAMH0,      PAGE = 0, TYPE = DSECT  /* not used when only using RAM */

   /* Allocate data areas: */
   .stack           : > RAMM1,       PAGE = 1
   .bss             : > DRAMH0,      PAGE = 1
   .ebss            : >  RAMM0_1,      PAGE = 1
   .const           : > DRAMH0,      PAGE = 1
   .econst          : > DRAMH0,      PAGE = 1      
   .sysmem          : > DRAMH0,      PAGE = 1
    chang			: > RAMM0_1,      PAGE = 1

   /* .reset indicates the start of _c_int00 for C Code.  
   /* When using the boot ROM this section is not needed.  
   /* Thus, the default type is set to DESECT */ 
   .reset           : > RESET,      PAGE = 0, TYPE = DSECT
   
}
