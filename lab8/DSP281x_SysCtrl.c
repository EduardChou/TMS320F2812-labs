//###########################################################################
//
// FILE:   DSP281x_SysCtrl.c
//
// TITLE:  DSP281x Device System Control Initialization & Support Functions.
//
// DESCRIPTION:
//
//         Example initialization of system resources.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Additional comments added to explain the PLL
//      |             |      |    initialization.
//      |             |      | Changed the PLL initialization to take into
//      |             |      |    account bit definitions for the PLLCR register
//      |             |      | Removed DFT initialization - no longer needed 
//      |             |      |    as of Rev C F2810/12 silicon
//      |             |      | Split some operations into their own function
//      |             |      |    for better modularity
//      |             |      | Added pipeline flush after the Flash Init
//###########################################################################


#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Functions that will be run from RAM need to be assigned to 
// a different section.  This section will then be mapped to a load and 
// run address using the linker cmd file.

#pragma CODE_SECTION(InitFlash, "ramfuncs");

//---------------------------------------------------------------------------
// InitSysCtrl: 
//---------------------------------------------------------------------------
// This function initializes the System Control registers to a known state.
// - Disables the watchdog
// - Set the PLLCR for proper SYSCLKOUT frequency 
// - Set the pre-scaler for the high and low frequency peripheral clocks
// - Enable the clocks to the peripherals

void InitSysCtrl(void)
{

// On F2812/F2810 TMX samples prior to rev C this initialization was 
// required.  For Rev C and after this is no longer required
/*
   EALLOW;
   DevEmuRegs.M0RAMDFT = 0x0300;
   DevEmuRegs.M1RAMDFT = 0x0300;
   DevEmuRegs.L0RAMDFT = 0x0300;
   DevEmuRegs.L1RAMDFT = 0x0300;
   DevEmuRegs.H0RAMDFT = 0x0300;
   EDIS;
*/
   // Disable the watchdog        
   DisableDog();
   
   // Initialize the PLLCR to 0xA
   InitPll();
   // Initialize the peripheral clocks
   InitPeripheralClocks();
}


//---------------------------------------------------------------------------
// Example: InitFlash: 
//---------------------------------------------------------------------------
// This function initializes the Flash Control registers

//                   CAUTION 
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results

void InitFlash(void)
{
   EALLOW;
   //Enable Flash Pipeline mode to improve performance
   //of code executed from Flash.
   FlashRegs.FOPT.bit.ENPIPE = 1;
   
   //                CAUTION
   //Minimum waitstates required for the flash operating
   //at a given CPU rate must be characterized by TI. 
   //Refer to the datasheet for the latest information.  

   //Set the Random Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = 5;
   
   //Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = 5;
   
   //                CAUTION
   //Minimum cycles required to move between power states
   //at a given CPU rate must be characterized by TI. 
   //Refer to the datasheet for the latest information.
     
   //For now use the default count
   //Set number of cycles to transition from sleep to standby
   FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;       
   
   //Set number of cycles to transition from standby to active
   FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;   
   EDIS;

   //Force a pipeline flush to ensure that the write to 
   //the last register configured occurs before returning.  

   asm(" RPT #7 || NOP");
}	


//---------------------------------------------------------------------------
// Example: KickDog: 
//---------------------------------------------------------------------------
// This function resets the watchdog timer.
// Enable this function for using KickDog in the application 

void KickDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// Example: DisableDog: 
//---------------------------------------------------------------------------
// This function disables the watchdog timer.

void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

//---------------------------------------------------------------------------
// Example: InitPll: 
//---------------------------------------------------------------------------
// This function initializes the PLLCR register.
//初始化PLL控制寄存器，设置PLL时钟的比例。
void InitPll(void)
{   int i;
    EALLOW;
    SysCtrlRegs.PLLCR.bit.DIV = 10;//SYSCLK = 外部振荡频率 * PLLCR /2
    EDIS;
    for(i= 0; i< ( (131072/2)/12 ); i++) {;}
}

//--------------------------------------------------------------------------
// Example: InitPeripheralClocks: 
//---------------------------------------------------------------------------
// This function initializes the clocks to the peripheral modules.
// First the high and low clock prescalers are set
// Second the clocks are enabled to each peripheral.
// To reduce power, leave clocks to unused peripherals disabled
// Note: If a peripherals clock is not enabled then you cannot 
// read or write to the registers for that peripheral 
//注意：如果一个外设的时钟没有使能，那么就不能访问该外设的寄存器。
void InitPeripheralClocks(void)
{
   EALLOW;
// HISPCP/LOSPCP prescale register settings, normally it will be set to default values
   SysCtrlRegs.HISPCP.all = 0x0001;
   SysCtrlRegs.LOSPCP.all = 0x0002;
   	
// Peripheral clock enables set for the selected peripherals.   
   SysCtrlRegs.PCLKCR.bit.EVAENCLK=1;
   SysCtrlRegs.PCLKCR.bit.EVBENCLK=1;
   SysCtrlRegs.PCLKCR.bit.SCIAENCLK=1;
   SysCtrlRegs.PCLKCR.bit.SCIBENCLK=1;
   SysCtrlRegs.PCLKCR.bit.MCBSPENCLK=1;
   SysCtrlRegs.PCLKCR.bit.SPIENCLK=1;
   SysCtrlRegs.PCLKCR.bit.ECANENCLK=1;
   SysCtrlRegs.PCLKCR.bit.ADCENCLK=1;
   EDIS;
}

	
//===========================================================================
// No more.
//===========================================================================
