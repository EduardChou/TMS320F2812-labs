//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Adc.c
//
// TITLE:	DSP28 ADC Initialization & Support Functions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 20 May 2002 | L.H. | No change
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#include "DSP281x_Device.h"

//---------------------------------------------------------------------------
// InitAdc: 
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
void InitAdc(void)
{
	unsigned int i;
	
	

	
	AdcRegs.ADCTRL1.bit.RESET=1;
	NOP;
	AdcRegs.ADCTRL1.bit.RESET=0;
	AdcRegs.ADCTRL1.bit.SUSMOD=3;
	AdcRegs.ADCTRL1.bit.ACQ_PS=0;
	AdcRegs.ADCTRL1.bit.CPS=0;
	AdcRegs.ADCTRL1.bit.CONT_RUN=0;
	AdcRegs.ADCTRL1.bit.SEQ_CASC=1;
	
	AdcRegs.ADCTRL3.bit.ADCBGRFDN=3;
	for(i=0;i<10000;i++)	NOP;
	AdcRegs.ADCTRL3.bit.ADCPWDN=1;
	for(i=0;i<5000;i++)	    NOP;
	AdcRegs.ADCTRL3.bit.ADCCLKPS=15;
	AdcRegs.ADCTRL3.bit.SMODE_SEL=1;

	AdcRegs.ADCMAXCONV.bit.MAX_CONV1=15;
	
	AdcRegs.ADCCHSELSEQ1.bit.CONV00=0;
	AdcRegs.ADCCHSELSEQ1.bit.CONV01=1;
	AdcRegs.ADCCHSELSEQ1.bit.CONV02=2;
	AdcRegs.ADCCHSELSEQ1.bit.CONV03=3;
	
	AdcRegs.ADCCHSELSEQ2.bit.CONV04=4;
	AdcRegs.ADCCHSELSEQ2.bit.CONV05=5;
	AdcRegs.ADCCHSELSEQ2.bit.CONV06=6;
	AdcRegs.ADCCHSELSEQ2.bit.CONV07=7;
	
	AdcRegs.ADCCHSELSEQ3.bit.CONV08=8;
	AdcRegs.ADCCHSELSEQ3.bit.CONV09=9;
	AdcRegs.ADCCHSELSEQ3.bit.CONV10=10;
	AdcRegs.ADCCHSELSEQ3.bit.CONV11=11;
	
	AdcRegs.ADCCHSELSEQ4.bit.CONV12=12;
	AdcRegs.ADCCHSELSEQ4.bit.CONV13=13;
	AdcRegs.ADCCHSELSEQ4.bit.CONV14=14;
	AdcRegs.ADCCHSELSEQ4.bit.CONV15=15;
	
	AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
	AdcRegs.ADCST.bit.INT_SEQ2_CLR=1;	
	
	AdcRegs.ADCTRL2.bit.EVB_SOC_SEQ=0;
	AdcRegs.ADCTRL2.bit.RST_SEQ1=0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1=1;
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1=0;
	AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1=0;
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1=0;
	AdcRegs.ADCTRL2.bit.RST_SEQ2=0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ2=0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2=0;
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2=0;
	AdcRegs.ADCTRL2.bit.EVB_SOC_SEQ2=0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;
	
	
	
	
}	


//===========================================================================
// No more.
//===========================================================================
