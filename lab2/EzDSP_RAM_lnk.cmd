MEMORY
{
PAGE 0 :
   PRAMH0	: origin = 0x3F8000, length = 0x001000
   RESET 	: origin = 0x3FFFC0, length = 0x000002           
PAGE 1 : 
   RAMM1	: origin = 0x000400, length = 0x000400
   RAMH0	: origin = 0x3f9000, length = 0x001000
}
 
 
SECTIONS
{
   .text	: > PRAMH0,	PAGE = 0
   .cinit	: > PRAMH0,	PAGE = 0
   .reset	: > RESET,	PAGE = 0, TYPE = DSECT
   .stack	: > RAMM1,	PAGE = 1
	.ebss   : > RAMH0,	PAGE = 1
   
}
/*   .bss             : > RAMH0,       PAGE = 1	  	
   .ebss             : > RAMH0,       PAGE = 1   */



