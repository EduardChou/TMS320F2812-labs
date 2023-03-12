MEMORY
{
	PAGE 1:    /* Data */
   	ROM  : origin = 0x3FF000, length = 0x000400     /* Boot ROM available if MP/MCn=0 */          
}

SECTIONS
{
	IQmathTables  : > ROM         PAGE = 1, TYPE = NOLOAD      
}