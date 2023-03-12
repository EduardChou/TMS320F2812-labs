//###########################################################################
//
// FILE:    DSP281x_CpuTimers.h
//
// TITLE:   DSP281x CPU 32-bit Timers Register Definitions.
//
// NOTES:   CpuTimer1 and CpuTimer2 are reserved for use with DSP BIOS and
//          other realtime operating systems.  
//
//          Do not use these two timers in your application if you ever plan
//          on integrating DSP-BIOS or another realtime OS. 
//
//          For this reason, the code to manipulate these two timers is
//          commented out and not used in these examples.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Corrected the definition of the TCR register
//###########################################################################

#ifndef DSP281x_CPU_TIMERS_H
#define DSP281x_CPU_TIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// CPU Timer Register Bit Definitions:
//CPU定时器寄存器位定义
//
// TCR: Control register bit definitions:
//TCR：定时器控制寄存器位定义
struct  TCR_BITS {          // bits  description
   Uint16    rsvd1:4;       // 3:0   reserved
                            //保留
   Uint16    TSS:1;         // 4     Timer Start/Stop
                            //定时器停止状态位。TSS是停止或启动定时器的一个标志位。
                            //要停止定时器，置TSS为1。要启动或重启动定时器，置TSS为0。
                            //在复位时，TSS清零并且定时器立即启动。
   Uint16    TRB:1;         // 5     Timer reload
                            //定时器重装位。当向TRB写1时，PRDH：PRD的值装入TIMH：TIM，
                            //并且把定时器分频寄存器（TDDRH：TDDR）中的值装入预定标寄存器
                            //（PSCH：PSC）。TRB位一直读作0。
   Uint16    rsvd2:4;       // 9:6   reserved
   Uint16    SOFT:1;        // 10    Emulation modes
   Uint16    FREE:1;        // 11
                            //定时器仿真方式
                            //00 在TIMH：TIM下一个减数后停止（硬停止）
                            //01 在TIMH：TIM减到0后停止（软停止）
                            //10 自由运行
                            //11 自由运行
   Uint16    rsvd3:2;       // 12:13 reserved
   Uint16    TIE:1;         // 14    Output enable
                            //定时器中断使能。如果定时器减到0，该位置1，定时器提出中断请求。
   Uint16    TIF:1;         // 15    Interrupt flag
                            //定时器中断标志，当定时器减到为0时，本标志置1，可通过软件写1
                            //清除该位，但置1只能通过定时器减到0来执行。对该位写1清除该位，
                            //写0无效。
}; 

union TCR_REG {
   Uint16           all;
   struct TCR_BITS  bit;
};

// TPR: Pre-scale low bit definitions:
//预定标寄存器低位
struct  TPR_BITS {        // bits  description                           
   Uint16     TDDR:8;     // 7:0   Divide-down low
   Uint16     PSC:8;      // 15:8  Prescale counter low
};

union TPR_REG {
   Uint16           all;
   struct TPR_BITS  bit;
};

// TPRH: Pre-scale high bit definitions:
//预定标寄存器高位
struct  TPRH_BITS {       // bits  description
   Uint16     TDDRH:8;      // 7:0   Divide-down high
   Uint16     PSCH:8;       // 15:8  Prescale counter high
};

union TPRH_REG {
   Uint16           all;
   struct TPRH_BITS bit;
};

// TIM, TIMH: Timer register definitions:
//定时器计数寄存器
struct TIM_REG {
   Uint16  LSW;
   Uint16  MSW;
};

union TIM_GROUP {
   Uint32          all;
   struct TIM_REG  half;
};

// PRD, PRDH: Period register definitions:
//周期寄存器
struct PRD_REG {
   Uint16  LSW;
   Uint16  MSW;
};

union PRD_GROUP {
   Uint32          all;
   struct PRD_REG  half;
};

//---------------------------------------------------------------------------
// CPU Timer Register File:
//CPU定时器寄存器文件
struct CPUTIMER_REGS {
   union TIM_GROUP TIM;   // Timer counter register
   union PRD_GROUP PRD;   // Period register
   union TCR_REG   TCR;   // Timer control register
   Uint16          rsvd1; // reserved
   union TPR_REG   TPR;   // Timer pre-scale low
   union TPRH_REG  TPRH;  // Timer pre-scale high
};

//---------------------------------------------------------------------------
// CPU Timer Support Variables:
//CPU定时器变量
struct CPUTIMER_VARS {
   volatile struct  CPUTIMER_REGS  *RegsAddr; //定义结构体指针，指向寄存器地址。
   Uint32    InterruptCount;
   float   CPUFreqInMHz;
   float   PeriodInUSec;
};

//---------------------------------------------------------------------------
// Function prototypes and external definitions:
//原型函数定义
void InitCpuTimers(void); //初始化定时器
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period);
//配置定时器
extern volatile struct CPUTIMER_REGS CpuTimer0Regs;
extern struct CPUTIMER_VARS CpuTimer0;

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
//extern volatile struct CPUTIMER_REGS CpuTimer1Regs;
//extern volatile struct CPUTIMER_REGS CpuTimer2Regs;

//extern struct CPUTIMER_VARS CpuTimer1;
//extern struct CPUTIMER_VARS CpuTimer2;

//---------------------------------------------------------------------------
// Usefull Timer Operations:
//
// Start Timer:
#define StartCpuTimer0()  CpuTimer0Regs.TCR.bit.TSS = 0

// Stop Timer:
#define StopCpuTimer0()   CpuTimer0Regs.TCR.bit.TSS = 1

// Reload Timer With period Value:
#define ReloadCpuTimer0() CpuTimer0Regs.TCR.bit.TRB = 1

// Read 32-Bit Timer Value:
#define ReadCpuTimer0Counter() CpuTimer0Regs.TIM.all

// Read 32-Bit Period Value:
#define ReadCpuTimer0Period() CpuTimer0Regs.PRD.all

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
// Do not use these two timers if you ever plan on integrating 
// DSP-BIOS or another realtime OS. 
//
// For this reason, the code to manipulate these two timers is
// commented out and not used in these examples.

// Start Timer:
//#define StartCpuTimer1()  CpuTimer1Regs.TCR.bit.TSS = 0
//#define StartCpuTimer2()  CpuTimer2Regs.TCR.bit.TSS = 0

// Stop Timer:
//#define StopCpuTimer1()   CpuTimer1Regs.TCR.bit.TSS = 1
//#define StopCpuTimer2()   CpuTimer2Regs.TCR.bit.TSS = 1

// Reload Timer With period Value:
//#define ReloadCpuTimer1() CpuTimer1Regs.TCR.bit.TRB = 1
//#define ReloadCpuTimer2() CpuTimer2Regs.TCR.bit.TRB = 1

// Read 32-Bit Timer Value:
//#define ReadCpuTimer1Counter() CpuTimer1Regs.TIM.all
//#define ReadCpuTimer2Counter() CpuTimer2Regs.TIM.all

// Read 32-Bit Period Value:
//#define ReadCpuTimer1Period() CpuTimer1Regs.PRD.all
//#define ReadCpuTimer2Period() CpuTimer2Regs.PRD.all


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP281x_CPU_TIMERS_H definition


//===========================================================================
// No more.
//===========================================================================
