//###########################################################################
//
// FILE:	DSP281x_Sci.c
//
// TITLE:	DSP281x SCI Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | No change since previous version (v.58 Alpha)
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File
unsigned int * UART_MODE = (unsigned int *) 0x4010;
//---------------------------------------------------------------------------
// InitSPI: 
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSci(void)
{	// Initialize SCI-A:
	*UART_MODE = 0x44;
	
	EALLOW;
	GpioMuxRegs.GPFMUX.all = 0x0030;
	EDIS;
	/* loopback   8 bit data */
	SciaRegs.SCICCR.all = 0x07;
	
	SciaRegs.SCICTL1.all = 0x03;
	SciaRegs.SCICTL2.all = 0x03;
	
	SciaRegs.SCIHBAUD = 0x00;
	SciaRegs.SCILBAUD = 0xF3;
	
	SciaRegs.SCICTL1.all = 0x23;
	
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
	//tbd...
 	

	// Initialize SCI-B:

	//tbd...
}	
/********************************************************************************
	name:	int SciaTx_Ready(void)
	input:	none
	output:	i	1:	ready
			0:	busy
*********************************************************************************/

int SciaTx_Ready(void)
{
	unsigned int i;
	if(SciaRegs.SCICTL2.bit.TXRDY == 1)
	{
		i = 1;
	}
	else
	{
		i = 0;
	}
	return(i);
}

/********************************************************************************
	name:	int SciaRx_Ready(void)
	input:	none
	output:	i	1:	new data
			0:	none
*********************************************************************************/

int SciaRx_Ready(void)
{
	unsigned int i;
	if(SciaRegs.SCIRXST.bit.RXRDY == 1)
	{
		i = 1;
	}
	else
	{
		i = 0;
	}
	return(i);
}







	
//===========================================================================
// No more.
//===========================================================================
	
