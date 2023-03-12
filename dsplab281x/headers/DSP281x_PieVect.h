//###########################################################################
//
// FILE:   DSP281x_PieVect.h
//
// TITLE:  DSP281x Devices PIE Vector Table Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Changed USER0-USER11 to USER1-USER12 to match the CPU guide
//###########################################################################

#ifndef DSP281x_PIE_VECT_H
#define DSP281x_PIE_VECT_H


#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// PIE Interrupt Vector Table Definition:
//PIE中断向量表
// Create a user type called PINT (pointer to interrupt):

typedef interrupt void(*PINT)(void);

// Define Vector Table:
//定义中断向量表
struct PIE_VECT_TABLE {

// Reset is never fetched from this table. 
// It will always be fetched from 0x3FFFC0 in either
// boot ROM or XINTF Zone 7 depending on the state of
// the XMP/MC input signal.  On the F2810 it is always
// fetched from boot ROM. 
//复位向量从不取自PIE 。
//它将根据XMP/MC输入信号的状态决定从boot ROM的0x3FFFC0处，还
//是从 XINTF Zone 7的0x3FFFC0处取得。
//对于F2810总是从boot ROM取得。

      PINT     PIE1_RESERVED;  
      PINT     PIE2_RESERVED;
      PINT     PIE3_RESERVED;
      PINT     PIE4_RESERVED;
      PINT     PIE5_RESERVED;
      PINT     PIE6_RESERVED;
      PINT     PIE7_RESERVED;
      PINT     PIE8_RESERVED;
      PINT     PIE9_RESERVED;
      PINT     PIE10_RESERVED;
      PINT     PIE11_RESERVED;
      PINT     PIE12_RESERVED;
      PINT     PIE13_RESERVED;

// Non-Peripheral Interrupts:
//非外设中断
      PINT     XINT13;    // XINT13
                          //外部可屏蔽中断13
      PINT     TINT2;     // CPU-Timer2
                          //定时器2中断
      PINT     DATALOG;   // Datalogging interrupt
                          //数据记录中断
      PINT     RTOSINT;   // RTOS interrupt
                          //实时操作系统中断
      PINT     EMUINT;    // Emulation interrupt
                          //仿真中断
      PINT     XNMI;      // Non-maskable interrupt
                          //不可屏蔽中断
      PINT     ILLEGAL;   // Illegal operation TRAP
                          //非法操作中断
      PINT     USER1;     // User Defined trap 1
                          //用户自定义中断1
      PINT     USER2;     // User Defined trap 2
      PINT     USER3;     // User Defined trap 3
      PINT     USER4;     // User Defined trap 4
      PINT     USER5;     // User Defined trap 5
      PINT     USER6;     // User Defined trap 6
      PINT     USER7;     // User Defined trap 7
      PINT     USER8;     // User Defined trap 8
      PINT     USER9;     // User Defined trap 9
      PINT     USER10;    // User Defined trap 10
      PINT     USER11;    // User Defined trap 11
      PINT     USER12;    // User Defined trap 12
                          //用户自定义中断12
      
// Group 1 PIE Peripheral Vectors:
//第1组外设中断
      PINT     PDPINTA;   // EV-A
      PINT     PDPINTB;   // EV-B
      PINT     rsvd1_3;
      PINT     XINT1;     
      PINT     XINT2;
      PINT     ADCINT;    // ADC
      PINT     TINT0;     // Timer 0
      PINT     WAKEINT;   // WD
           
// Group 2 PIE Peripheral Vectors:
//第2组外设中断
      PINT     CMP1INT;   // EV-A
      PINT     CMP2INT;   // EV-A
      PINT     CMP3INT;   // EV-A
      PINT     T1PINT;    // EV-A
      PINT     T1CINT;    // EV-A
      PINT     T1UFINT;   // EV-A
      PINT     T1OFINT;   // EV-A
      PINT     rsvd2_8;
      
// Group 3 PIE Peripheral Vectors:
//第3组外设中断
      PINT     T2PINT;    // EV-A
      PINT     T2CINT;    // EV-A
      PINT     T2UFINT;   // EV-A
      PINT     T2OFINT;   // EV-A
      PINT     CAPINT1;   // EV-A
      PINT     CAPINT2;   // EV-A
      PINT     CAPINT3;   // EV-A
      PINT     rsvd3_8;
      
// Group 4 PIE Peripheral Vectors:
//第4组外设中断
      PINT     CMP4INT;   // EV-B
      PINT     CMP5INT;   // EV-B
      PINT     CMP6INT;   // EV-B
      PINT     T3PINT;    // EV-B
      PINT     T3CINT;    // EV-B
      PINT     T3UFINT;   // EV-B
      PINT     T3OFINT;   // EV-B
      PINT     rsvd4_8;      
     
// Group 5 PIE Peripheral Vectors:
//第5组外设中断
      PINT     T4PINT;    // EV-B
      PINT     T4CINT;    // EV-B
      PINT     T4UFINT;   // EV-B
      PINT     T4OFINT;   // EV-B
      PINT     CAPINT4;   // EV-B
      PINT     CAPINT5;   // EV-B
      PINT     CAPINT6;   // EV-B
      PINT     rsvd5_8;      

// Group 6 PIE Peripheral Vectors:
//第6组外设中断
      PINT     SPIRXINTA; // SPI-A
      PINT     SPITXINTA; // SPI-A
      PINT     rsvd6_3;
      PINT     rsvd6_4;
      PINT     MRINTA;    // McBSP-A
      PINT     MXINTA;    // McBSP-A
      PINT     rsvd6_7;
      PINT     rsvd6_8;
      
// Group 7 PIE Peripheral Vectors:
//第7组外设中断
      PINT     rsvd7_1;
      PINT     rsvd7_2;
      PINT     rsvd7_3;
      PINT     rsvd7_4;
      PINT     rsvd7_5;
      PINT     rsvd7_6;
      PINT     rsvd7_7;
      PINT     rsvd7_8;

// Group 8 PIE Peripheral Vectors:
//第8组外设中断
      PINT     rsvd8_1;
      PINT     rsvd8_2;
      PINT     rsvd8_3;
      PINT     rsvd8_4;
      PINT     rsvd8_5;
      PINT     rsvd8_6;
      PINT     rsvd8_7;
      PINT     rsvd8_8; 

// Group 9 PIE Peripheral Vectors:
//第9组外设中断   
      PINT     RXAINT;    // SCI-A
      PINT     TXAINT;    // SCI-A
      PINT     RXBINT;    // SCI-B
      PINT     TXBINT;    // SCI-B
      PINT     ECAN0INTA; // eCAN
      PINT     ECAN1INTA; // eCAN
      PINT     rsvd9_7;
      PINT     rsvd9_8;

// Group 10 PIE Peripheral Vectors:
//第10组外设中断
      PINT     rsvd10_1;
      PINT     rsvd10_2;
      PINT     rsvd10_3;
      PINT     rsvd10_4;
      PINT     rsvd10_5;
      PINT     rsvd10_6;
      PINT     rsvd10_7;
      PINT     rsvd10_8;
            
// Group 11 PIE Peripheral Vectors:
//第11组外设中断
      PINT     rsvd11_1;
      PINT     rsvd11_2;
      PINT     rsvd11_3;
      PINT     rsvd11_4;
      PINT     rsvd11_5;
      PINT     rsvd11_6;
      PINT     rsvd11_7;
      PINT     rsvd11_8;

// Group 12 PIE Peripheral Vectors:
//第12组外设中断
      PINT     rsvd12_1;
      PINT     rsvd12_2;
      PINT     rsvd12_3;
      PINT     rsvd12_4;
      PINT     rsvd12_5;
      PINT     rsvd12_6;
      PINT     rsvd12_7;
      PINT     rsvd12_8;
};

//---------------------------------------------------------------------------
// PIE Interrupt Vector Table External References & Function Declarations:
//定义PieVectTable为中断向量表结构体变量
extern struct PIE_VECT_TABLE PieVectTable;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    // end of DSP281x_PIE_VECT_H definition

//===========================================================================
// No more.
//===========================================================================

