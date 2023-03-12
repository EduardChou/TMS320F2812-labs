//###########################################################################
//
// FILE:   DSP281x_PieCtrl.h
//
// TITLE:  DSP281x Device PIE Control Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | No change since previous version (v.58 Alpha)
//###########################################################################


#ifndef DSP281x_PIE_CTRL_H
#define DSP281x_PIE_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// PIE Control Register Bit Definitions:
//PIE控制寄存器
// PIECTRL: Register bit definitions:
struct PIECTRL_BITS {      // bits description
   Uint16  ENPIE:1;        // 0    Enable PIE block
   //从PIE块取回向量使能。当该位为1时，所有向量取自PIE向量表。
   //如果该位为0，PIE块无效，向量取自引导ROM的CPU向量表或XINTF7。
   //甚至当PIE块无效时，PIE块寄存器（PIEACK、PIEIFR、PIEIER）也
   //可以被访问。
   Uint16  PIEVECT:15;     // 15:1 Fetched vector address
   //这些位指示了从PIE向量表取回的向量地址。最低位忽略，只显示位1
   //到位15地址。用户可以读取向量值，以确定取回的向量是由哪一个中断产生的。
};

union PIECTRL_REG {
   Uint16                 all;
   struct PIECTRL_BITS  bit;
};  

// PIEIER: Register bit definitions:
//PIE中断使能寄存器
//有12个PIEIER寄存器，PIE模块（INT1`INT12）使用各个CPU中断中的一个
//这些寄存器位在一个组里单独使能，它们的行为非常
//像内核中断使能寄存器。把某位置1，可以使能中断服务，
//把某位置0，将使该位中断无效。
//注意：所有下述寄存器复位置1
struct PIEIER_BITS {       // bits description
   Uint16 INTx1:1;         // 0    INTx.1
   Uint16 INTx2:1;         // 1    INTx.2
   Uint16 INTx3:1;         // 2    INTx.3
   Uint16 INTx4:1;         // 3    INTx.4
   Uint16 INTx5:1;         // 4    INTx.5
   Uint16 INTx6:1;         // 5    INTx.6
   Uint16 INTx7:1;         // 6    INTx.7
   Uint16 INTx8:1;         // 7    INTx.8
   Uint16 rsvd:8;          // 15:8 reserved
};

union PIEIER_REG {
   Uint16              all;
   struct PIEIER_BITS  bit;
}; 

// PIEIFR: Register bit definitions:
//PIE中断标志寄存器
//这些位指示一个中断当前是否激活。它们的表现非常像内核
//中断标志寄存器。当中断激活时，各个寄存器位置1。当一个中断被处理
//完成或向该寄存器写0时，该位清0。该寄存器还可以被读取以确定哪个中断
//被激活或未处理。
//注意：所有下述寄存器复位置1
//在CPU访问PIEIFR寄存器时，硬件有优先级。
//在取回中断向量的过程中，PIEIFR寄存器位清0。
struct PIEIFR_BITS {       // bits description
   Uint16 INTx1:1;         // 0    INTx.1
   Uint16 INTx2:1;         // 1    INTx.2
   Uint16 INTx3:1;         // 2    INTx.3
   Uint16 INTx4:1;         // 3    INTx.4
   Uint16 INTx5:1;         // 4    INTx.5
   Uint16 INTx6:1;         // 5    INTx.6
   Uint16 INTx7:1;         // 6    INTx.7
   Uint16 INTx8:1;         // 7    INTx.8
   Uint16 rsvd:8;          // 15:8 reserved
};

union PIEIFR_REG {
   Uint16              all;
   struct PIEIFR_BITS  bit;
};

// PIEACK: Register bit definitions:
//PIE应答寄存器
struct PIEACK_BITS {       // bits description
   Uint16 ACK1:1;          // 0    Acknowledge PIE interrupt group 1
   Uint16 ACK2:1;          // 1    Acknowledge PIE interrupt group 2
   Uint16 ACK3:1;          // 2    Acknowledge PIE interrupt group 3
   Uint16 ACK4:1;          // 3    Acknowledge PIE interrupt group 4
   Uint16 ACK5:1;          // 4    Acknowledge PIE interrupt group 5
   Uint16 ACK6:1;          // 5    Acknowledge PIE interrupt group 6
   Uint16 ACK7:1;          // 6    Acknowledge PIE interrupt group 7
   Uint16 ACK8:1;          // 7    Acknowledge PIE interrupt group 8
   Uint16 ACK9:1;          // 8    Acknowledge PIE interrupt group 9
   Uint16 ACK10:1;         // 9    Acknowledge PIE interrupt group 10
   Uint16 ACK11:1;         // 10   Acknowledge PIE interrupt group 11
   Uint16 ACK12:1;         // 11   Acknowledge PIE interrupt group 12
   Uint16 rsvd:4;          // 15:12 reserved
};

union PIEACK_REG {
   Uint16              all;
   struct PIEACK_BITS  bit;
};

//---------------------------------------------------------------------------
// PIE Control Register File:
//PIE控制寄存器文件
struct PIE_CTRL_REGS {
   union PIECTRL_REG PIECRTL;       // PIE control register
   union PIEACK_REG  PIEACK;        // PIE acknowledge
   union PIEIER_REG  PIEIER1;       // PIE INT1 IER register  
   union PIEIFR_REG  PIEIFR1;       // PIE INT1 IFR register
   union PIEIER_REG  PIEIER2;       // PIE INT2 IER register 
   union PIEIFR_REG  PIEIFR2;       // PIE INT2 IFR register
   union PIEIER_REG  PIEIER3;       // PIE INT3 IER register 
   union PIEIFR_REG  PIEIFR3;       // PIE INT3 IFR register
   union PIEIER_REG  PIEIER4;       // PIE INT4 IER register             
   union PIEIFR_REG  PIEIFR4;       // PIE INT4 IFR register
   union PIEIER_REG  PIEIER5;       // PIE INT5 IER register  
   union PIEIFR_REG  PIEIFR5;       // PIE INT5 IFR register
   union PIEIER_REG  PIEIER6;       // PIE INT6 IER register 
   union PIEIFR_REG  PIEIFR6;       // PIE INT6 IFR register
   union PIEIER_REG  PIEIER7;       // PIE INT7 IER register 
   union PIEIFR_REG  PIEIFR7;       // PIE INT7 IFR register
   union PIEIER_REG  PIEIER8;       // PIE INT8 IER register
   union PIEIFR_REG  PIEIFR8;       // PIE INT8 IFR register
   union PIEIER_REG  PIEIER9;       // PIE INT9 IER register  
   union PIEIFR_REG  PIEIFR9;       // PIE INT9 IFR register
   union PIEIER_REG  PIEIER10;      // PIE INT10 IER register 
   union PIEIFR_REG  PIEIFR10;      // PIE INT10 IFR register
   union PIEIER_REG  PIEIER11;      // PIE INT11 IER register 
   union PIEIFR_REG  PIEIFR11;      // PIE INT11 IFR register
   union PIEIER_REG  PIEIER12;      // PIE INT12 IER register
   union PIEIFR_REG  PIEIFR12;      // PIE INT12 IFR register
};     

#define PIEACK_GROUP1   0x0001;
#define PIEACK_GROUP2   0x0002;
#define PIEACK_GROUP3   0x0004;
#define PIEACK_GROUP4   0x0008;
#define PIEACK_GROUP5   0x0010;
#define PIEACK_GROUP6   0x0020;
#define PIEACK_GROUP7   0x0040;
#define PIEACK_GROUP8   0x0080;
#define PIEACK_GROUP9   0x0100;
#define PIEACK_GROUP10  0x0200;
#define PIEACK_GROUP11  0x0400;
#define PIEACK_GROUP12  0x0800;

//---------------------------------------------------------------------------
// PIE Control Registers External References & Function Declarations:
//
extern volatile struct PIE_CTRL_REGS PieCtrlRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP281x_PIE_CTRL_H definition

//===========================================================================
// No more.
//===========================================================================
