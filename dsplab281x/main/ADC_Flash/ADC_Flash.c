/***************************************************************************
** 功能描述:采用查询法, 对ADCINB7通道进行采样,显示实际值及对应的10进制数字值
***************************************************************************/
#include "DSP281x_Device.h"
// 这些变量由连接器定义(见F2812.cmd)。下面3条指令为Flash加载专用
//############################################################################################
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
//###########################################################################################
unsigned long int  AD1;  
unsigned long int AD2;  //实际AD值*1000
void Write_LED (int LEDReg);//通过SPI向LED发送显示数据
void spi_intial()
 {
 	SpiaRegs.SPICCR.all =0x0047;   // 使SPI处于复位方式, 下降沿, 八位数据  
	SpiaRegs.SPICTL.all =0x0006;   // 主控模式, 一般时钟模式,使能talk, 关闭SPI中断.
	SpiaRegs.SPIBRR =0x007F;       //配置波特率
	SpiaRegs.SPICCR.all =SpiaRegs.SPICCR.all|0x0080;  // 退出复位状态	 
	EALLOW;	
    GpioMuxRegs.GPFMUX.all=0x000F;	// 设置通用引脚为SPI引脚	 	
    EDIS;
  }

void gpio_init()
{ 
	EALLOW;
	GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11=0; //GPIOA11设置为一般I/O口
	GpioMuxRegs.GPADIR.bit.GPIOA11=1;		//把GPIOA11设置为输出						    
    EDIS;
    GpioDataRegs.GPADAT.bit.GPIOA11=0; 	//GPIOA11=0;该端口为74HC595锁存信号
}

/*LED显示子程序*/
void LEDdisplay(int LED8,int LED7,int LED6,int LED5,int LED4,int LED3,int LED2,int LED1)
{
  
      	GpioDataRegs.GPADAT.bit.GPIOA11=0; //给LACK信号一个低电平
	  	Write_LED (LED8);//向LED最高位写数据,显示LED8
		Write_LED (LED7);
		Write_LED (LED6);
		Write_LED (LED5);
		Write_LED (LED4);
		Write_LED (LED3);
		Write_LED (LED2);
		Write_LED (LED1);//向LED最低位写数据,显示LED1

     	GpioDataRegs.GPADAT.bit.GPIOA11=1; //给LACK信号一个高电平为锁存74HC595      	
} 
/*子程序结束*/

unsigned long int i = 0;

//ADC相关寄存器初始化
void Adc_Init()
{
	// Configure ADC @SYSCLKOUT = 150Mhz
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;      //双序列/级连选择:双序列工作模式
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;     //连续/并发选择:连续采样方式
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;      //启动-停止/连续转换选择:启动-停止方式
    AdcRegs.ADCTRL1.bit.CPS = 1;           //核时钟预定标器:ADC_CLK=ADCLKPS/2=3.125M
  	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;      //采集窗口大小:SH pulse/clock=16
   	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x2;    //核时钟分频:ADCLKPS=HSPCLK/4=6.25M
	AdcRegs.ADCMAXCONV.all = 0x0000;       //转换通道数:SEQ1序列的通道数为1
  	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf; //转换通道选择:ADCINB7 
}

//ADC模块上电延时
void Adc_PowerUP() 
{ 	
  	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;     //ADC带隙和参考电路加电
  	for (i=0; i<1000000; i++){}      //至少5ms延时
  	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;		 //ADC核模拟电路加电
  	for (i=0; i<10000; i++){}        //至少20us延时
}

main() 
{
    InitSysCtrl(); //系统初始化
     EALLOW; 
     SysCtrlRegs.HISPCP.all = 0x3;	//高速外设时钟HSPCLK=SYSCLKOUT/6=25Mhz
     EDIS;  
    DINT;           // 关闭总中断	
	IER = 0x0000;	// 关闭外围中断
	IFR = 0x0000;   // 清中断标志
	spi_intial();                    //SPI初始化子程序
	gpio_init();	                 //GPIO初始化子程序
	Adc_PowerUP();
   	Adc_Init();
   // RamfuncsLoadStart, RamfuncsLoadEnd,及RamfuncsRunStart符号由连接器生成。
		// 参考F2812.cmd文件,下面2条指令为Flash加载专用
//############################################################################################
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);		
   InitFlash();		// 调用Flash初始化函数以便设置Flash等待状态。这个函数必须驻留在RAM中。
//############################################################################################	 
   	while (1)
   	{
   	    int e=0;
   		AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;   //用S/W方式启动SEQ1转换序列
   		while (AdcRegs.ADCST.bit.SEQ1_BSY == 1){}    //判序列忙否
   		AD1 = AdcRegs.ADCRESULT0 >> 4;
		AD2=(AD1*3*1000)/4095;   //实际AD值*1000
   		e++;
          if(e>0)
            {   		
              LEDdisplay((AD2/1000)+20,(AD2%1000)/100,(AD2%100)/10,19,
              AD1/1000,(AD1%1000)/100,(AD1%100)/10,AD1%10);
              e=0;
            }
   	}
}
   	
//通过SPI向LED发送显示数据
void Write_LED (int LEDReg)
{
Uint16 LEDcode[30]={0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,
                    0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,
                    0x8c00,0xbf00,0xa700,0xff00,0x4000,0x7900,0x2400,0x3000,
                    0x1900,0x1200,0x0200,0x7800,0x0000,0x1000};//共阳字形码0~f, P，－，L，"灭",0.~9.	
	 		SpiaRegs.SPITXBUF =LEDcode[LEDReg]; //给数码管送数
    	 	 while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){} 		
    		SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF; //读出清标志
}
//=========================================================================================
// No more.
//=========================================================================================

