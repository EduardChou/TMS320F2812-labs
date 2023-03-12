//###########################################################################
//
// FILE:	DSP281x_Gpio.c
//
// TITLE:	DSP281x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | No change since previous version (v.58 Alpha)
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known state.
//
void InitGpio(void)
{

// Set GPIO F port pins Output
// Input Qualifier =0, none
     EALLOW;
     GpioMuxRegs.GPFMUX.all=0x0000;     
     GpioMuxRegs.GPFDIR.all=0xFFFF;    	// upper byte as output/low byte as input
     EDIS;

}	
	
//===========================================================================
// No more.
//===========================================================================
