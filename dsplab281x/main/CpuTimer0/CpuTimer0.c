// 功能:利用CPU定时器0每次定时器到，轮流点亮发光二极管DS22和DS23,实现指示灯的定时闪烁

#include "DSP281x_Device.h"     // DSP281x头文件
#include "DSP281x_Examples.h"   // DSP281x Examples 头文件

unsigned int Led_Flag;
#define		LED0_OFF	GpioDataRegs.GPFDAT.bit.GPIOF11=1
#define		LED0_ON		GpioDataRegs.GPFDAT.bit.GPIOF11=0 //点亮DS23
#define		LED1_OFF	GpioDataRegs.GPFDAT.bit.GPIOF10=1
#define		LED1_ON		GpioDataRegs.GPFDAT.bit.GPIOF10=0 //点亮DS22
void IOinit()
{
 	EALLOW;  
 	//将GPIOF8~GPIOF13配置为一般I/O口，输出
 	GpioMuxRegs.GPFMUX.all = 0xc0ff;
    GpioMuxRegs.GPFDIR.all = 0x3f00;   
  	EDIS;			
}

interrupt void cpu_timer0_isr(void);//中断声明

void main(void)
{

// Step 1. //系统初始化(在DSP281x_SysCtrl.c中)
// PLL, WatchDog, enable Peripheral Clocks 
   InitSysCtrl();//系统初始化

// Step 2. 初始化 GPIO:
   IOinit();
   GpioDataRegs.GPFDAT.all = 0x3f00;//GPIOF8~GPIOF13置高电平,指示灯灭
  
   Led_Flag=1;	//设置指示灯标志位
   
// Step 3. 清中断及初始化PIE矢量表:
   DINT;	/*关CPU中断*/

   InitPieCtrl();//初始化PIE控制寄存器 DSP281x_PieCtrl.c   
   IER = 0x0000;//关闭外围中断
   IFR = 0x0000;//清中断标志
 
   InitPieVectTable();//初始化PIE矢量表DSP281x_PieVect.c
  
   EALLOW;  //写EALLOW保护寄存器
   PieVectTable.TINT0 = &cpu_timer0_isr;//PIE中断矢量表
   EDIS;    

   InitCpuTimers();   // For this example, only initialize the Cpu Timers
//定时器的一般操作如下：将周期寄存器的PRDH:PRD中的值装入32位计数寄存器TIMH:TIM。
//然后计数寄存器以C28X 的SYSCLKOUT速率递减。当计数器减到0时，就会产生一个定时器
//中断输出信号（一个中断脉冲）。
// 150MHz CPU Freq, 1 second Period (in uSeconds)
//产生中断的间隔=（周期寄存器中的值）/SYSCLKOUT/分频系数
//周期寄存器中的值=(Freq*Period)
   ConfigCpuTimer(&CpuTimer0, 150, 1000000);
   StartCpuTimer0();

   IER |= M_INT1;	//使能CPU INT1 

   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//CPU-Timer0使能位于PIE第1组第7个，将其使能

   EINT;   //开放全局中断

   for(;;);
} 


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
	if(Led_Flag == 1)
	{
		LED0_OFF;
		LED1_ON;	//点亮DS22
		Led_Flag = 0;
	}
	else
	{
		LED0_ON;	//点亮DS23
		LED1_OFF;
		Led_Flag = 1;
	}
  
     PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;// 清中断应答信号，准备接收下一次中断
}

//===========================================================================
// No more.
//===========================================================================
