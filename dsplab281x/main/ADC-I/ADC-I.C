/**************************************************************
** 功能描述: ADC程序，对ADCINB7通道采样，中断方式**
**************************************************************/

#include "DSP281x_Device.h"
interrupt void adc_isr(void);
//定义全局变量:
Uint16 LoopCount;//无效循环计数
Uint16 ConversionCount;//当前结果数0-20
Uint16 Voltage[20];//最后20个ADCRESULT0值

main() 
{
    InitSysCtrl();   

    EALLOW;
    SysCtrlRegs.HISPCP.all = 0x3;	// HSPCLK = SYSCLKOUT/6，得25HZ
    EDIS;

	DINT;
	IER = 0x0000;
	IFR = 0x0000;       // 禁止和清除所有CPU中断

	InitPieCtrl();       

	InitPieVectTable();	       
    InitAdc();    //初始化ADC模块，该函数在DSP28_Adc.c文件中
	EALLOW;	//使能写保护寄存器的写操作
	PieVectTable.ADCINT = &adc_isr;        //把用户中断服务的入口地址
//赋给中断向量表头文件中的对应向量
	EDIS;       // 禁止写保护寄存器的写操作

    PieCtrlRegs.PIEIER1.bit.INTx6 = 1; //使能PIE中的ADCINT中断

	IER |= M_INT1;					// // 使能 CPU 中断 1，使能全部 INT1

	EINT;   								// 使能全局中断 INTM
	ERTM;	  							// 使能全局实时中断 DBGM
    LoopCount = 0;           				//循环计数器清零
    ConversionCount = 0;      				//当前转化结果数清零
// 配置 ADC
    AdcRegs.ADCMAXCONV.all = 0x0000;       // 设置SEQ1的1个转化通道
  	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf; //转换通道选择:ADCINB7 

    AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1 = 1;  // 使能 EVASOC 去启动 SEQ1
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // 使能 SEQ1 中断 (每次 EOS)
// 配置 EVA
// 假设EVA已经在 InitSysCtrl()中使能;
    EvaRegs.T1CMPR =0x0080;               // 设置 T1 比较值
    EvaRegs.T1PR = 0xFFFF;                 // 设置周期寄存器
    EvaRegs.GPTCONA.bit.T1TOADC = 1; // 使能EVA中的 EVASOC(下溢中断启动ADC) 
    EvaRegs.T1CON.all = 0x1042;        // 使能定时器1比较操作 (增计数模式 )
//等ADC转换
    while (1)
    {
       LoopCount++;
    }
}
interrupt void  adc_isr(void)
{
  Voltage[ConversionCount] = AdcRegs.ADCRESULT0;
 
// 如果已记录了20结果则重新开始转换
  if(ConversionCount == 19) 
  {
     ConversionCount = 0;
  }
  else ConversionCount++;
// 重新初始化下一次ADC转换
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // 复位 SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;		// 清 INT SEQ1位
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // 清中断应答信号，准备接收下一次中断
  return;                      
}
