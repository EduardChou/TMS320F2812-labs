/******************************************************************************
**功能描述:DA-AD转换,用键盘设定十六进制数字量,通过MAX5742的OUTC通道DA0UT3使能
并转换输出为模拟量, 该模拟量作为ADCINB6输入通道再转换成十六进制数字量并在LED上显示**
******************************************************************************/

#include"DSP281x_Device.h"
unsigned int  DAinput=0;
unsigned int  ADoutput=0 ;	
unsigned int LEDReg;
unsigned int LED8=0;//DA输入LED初始化
unsigned int LED7=0;
unsigned int LED6=0;
unsigned int LED3=0;//AD输出LED初始化
unsigned int LED2=0;
unsigned int LED1=0;

unsigned int KeyReg1;
unsigned int KeyReg2;
unsigned long int i = 0;
void  IOinit(void);
int   KeyIn1(void);
void  KeyFunction1(unsigned int KeyReg1);
void  KeyFunction2(unsigned int KeyReg2);
void Write_LED (int LEDReg);//通过SPI向LED发送显示数据
void  DAC(unsigned int DAinput);

Uint16 keyNum = 0x0000;  //按键次数

#define	  K1		0xfeff
#define	  K2		0xfdff
#define	  K3		0xfbff
#define	  K4		0xf7ff
#define	  K5		0xefff
#define	  K6		0xdfff
#define	  K7		0xbfff
#define	  K8		0x7fff
#define	  K9		0xfeff
#define	  K10		0xfdff
#define	  K11		0xfbff
#define	  K12		0xf7ff
#define	  K13		0xefff
#define	  K14		0xdfff
#define	  K15		0xbfff
#define	  K16		0x7fff

//中断子程序的声明
interrupt void   T4pint_isr(void);
interrupt void   adc_isr(void);

void IOinit()
{
 	EALLOW; 
    //将GPIOE0~GPIOE2配置为一般I/O口输出,作138译码  
 	GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all&0xfff8; 
	GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all|0x0007;
    //将GPIOB8~GPIOB15配置为一般I/O口,D0~D7
	GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all&0x00ff; 
	EDIS;				
}

	//SPI初始化子程序
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
	GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;  //  GPIOF14设置为一般I/O端口
	GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;	   //  把GPIOF14设置为输出			
    EDIS;
    GpioDataRegs.GPADAT.bit.GPIOA11=0; 	//GPIOA11=0;该端口为74HC595锁存信号
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 1; //  GPIOF14＝1;该端口为使能信号
}

//ADC模块上电延时
void Adc_PowerUP() 
{ 	
  	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;     //ADC带隙和参考电路加电
  	for (i=0; i<1000000; i++){}      //至少5ms延时
  	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;		 //ADC核模拟电路加电
  	for (i=0; i<10000; i++){}        //至少20us延时
}

 void Adc_Init()
{
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;      //启动-停止/连续转换选择:启动-停止方式
    AdcRegs.ADCTRL1.bit.CPS = 1;           //核时钟预定标器:ADC_CLK=ADCLKPS/2=3.125M
  	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;      //采集窗口大小:SH pulse/clock=16
   	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x5;    //核时钟分频:ADCLKPS=HSPCLK/4=6.25M
	AdcRegs.ADCMAXCONV.all = 0x0000;       //转换通道数:SEQ1序列的通道数为1
	AdcRegs.ADCMAXCONV.all = 0x0000;       //转换通道数:SEQ1序列的通道数为1
  	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xe; //转换通道选择:ADCINB7  	 
	AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1=1;  //SEQl被EVA的触发源启动
  	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1=1;  //使能中断		
}

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

void main(void)
{		
   	asm (" NOP ");
   	asm (" NOP ");
   	InitSysCtrl();      //初始化系统控制寄存器, 时钟频率150M
	DINT;	        //关闭总中断，除中断标志
	IER = 0x0000;   //关闭外围中断
	IFR = 0x0000;	//清中断标志
	spi_intial();   //SPI初始化子程序
	gpio_init();    // 关闭总中断.
	IOinit();	    // I/O初始化子程序
LEDdisplay(0,0,0,19,19,0,0,0);	//初始化LED
//初始化PIE控制寄存器
InitPieCtrl();       //DSP28_PieCtrl．C
//初始化PIE中断向量表
InitPieVectTable();  //DSP28_PieVect．C;
//初始化ADC寄存器
 Adc_PowerUP();
 Adc_Init();

//T2：启动AD转换
	EvaRegs.T2PR=3750;	//T2定时器周期
	EvaRegs.GPTCONA.bit.T2TOADC=2;	//由定时器2周期中断启动A／D转换
	EvaRegs.T2CON.all=0x084C;
	EvaRegs.T2CNT=0x0000;	//T2计数器清

//T4：按键扫描
	EvbRegs.T4PR=12000;	//T4定时器周期
	EvbRegs.T4CON.all=0x1744;	//x／128，增计数10．24 ms
	EvbRegs.T4CNT=0x0000;//T4计数器清
	EvbRegs.EVBIMRB.bit.T4PINT=1;	//使能T4PINT中断
	EvbRegs.EVBIFRB.bit.T4PINT=1;	//先清标志

//#．初始化Xintf
	InitXintf();

//###．中断使能
	EALLOW;
//PIE中断矢量表
	PieVectTable.T4PINT=&T4pint_isr;
	PieVectTable.ADCINT=&adc_isr;
	EDIS;
	PieCtrlRegs.PIEIER5.bit.INTx1=1;//使能T4PINT
	PieCtrlRegs.PIEIER1.bit.INTx6=1;//使能ADCINT
	IER |=(M_INT1 | M_INT5 );	//使能INTl，INT5
	EINT;	//开放全局中断
	ERTM;	//EnableGlobal realtime interrupt DBGM

//主循环
	for(;;){;}
}

interrupt void   T4pint_isr(void)
	{
  	  LEDdisplay(LED8,LED7,LED6,19,19,ADoutput/256,(ADoutput%256)/16,ADoutput%16); //显示DAinput灭灭ADoutput****  
  	    if (KeyIn1() == 1)     // 调用查键K1~K8子程序
 	    {
 	    keyNum=keyNum+1; 	 
	    KeyFunction1(KeyReg1);	
		if(keyNum>3)
		{
		keyNum=keyNum-3;
		LED8=LEDReg;
		}
 	    if(keyNum==1)
		{
		LED8=LEDReg;
		}
		 if(keyNum==2)
		{
		LED7=LEDReg;
		}
		 if(keyNum==3)
		{
		LED6=LEDReg;
		}
 	DAinput=LED8*256+LED7*16+LED6;  //DA设定值
	DAC(DAinput);   //DA转换
	EvbRegs.EVBIFRB.bit.T4PINT=1;	//中断标志清
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP5; //允许下次中断
	return;
 	    }
		if (KeyIn2() == 1)     // 调用查键K8~K16子程序
 	    {
 	    KeyFunction2(KeyReg2);		
 	   	keyNum=keyNum+1;		
		if(keyNum>3)
		{
		keyNum=keyNum-3;
		LED8=LEDReg;
		}
 	    if(keyNum==1)
		{
		LED8=LEDReg;
		}
		 if(keyNum==2)
		{
		LED7=LEDReg;
		}
		 if(keyNum==3)
		{
		LED6=LEDReg;
		}
 		DAinput=LED8*256+LED7*16+LED6;   //DA设定值 	    
		DAC(DAinput);   //DA转换
		}
	EvbRegs.EVBIFRB.bit.T4PINT=1;	//中断标志清
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP5; //许下次中断
	return; 
	}

interrupt void   adc_isr(void)
	{	
   		ADoutput=AdcRegs.ADCRESULT0 >> 4;//读AD结果	
		 AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;//清中断标志
       PieCtrlRegs.PIEACK.all=PIEACK_GROUP1; //允许下次中断       		                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	return;
	}

int KeyIn1(void)
{
 	 EALLOW;  
    //将GPIOB8~GPIOB15配置为输入,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all&0x00ff;
     EDIS;    
    GpioDataRegs.GPEDAT.all = 0xfff8;     //选通KEY低8位
    for (i=0; i<100; i++){}               //延时
    //判K1~K8是否按下
    if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
    {
    	for (i=0; i<100000; i++){}        //延时消抖
        if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
        {
	   KeyReg1=GpioDataRegs.GPBDAT.all ;
    		while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //判是否送开
    		{
    			GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
    			for (i=0; i<1000; i++){}     
    		}
    		return (1);			      
    	}
    }
    return (0);
}

int KeyIn2(void)
{
 	 EALLOW;  
    //将GPIOB8~GPIOB15配置为输入,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all&0x00ff;
     EDIS;    
    GpioDataRegs.GPEDAT.all = 0xfff9;     //选通KEY高8位
    for (i=0; i<100; i++){}               //延时
    //判K8~K16是否按下
    if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
    {
    	for (i=0; i<100000; i++){}        //延时消抖
        if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
        {
	   KeyReg2=GpioDataRegs.GPBDAT.all ;
    		while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //判是否送开
    		{
    			GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
    			for (i=0; i<1000; i++){}     
    		}
    		return (1);			      
    	}
    }
    return (0);
}

void	KeyFunction1(unsigned int KeyReg1)
		{
			switch(KeyReg1)
				{
					case  K1: {
								LEDReg= 0x00; 							
							  }
							  break;
					case  K2: {
								LEDReg= 0x01;							
							  }
							  break;
					case  K3: {
								LEDReg= 0x02;
							  }
							  break;
					case  K4: {
								LEDReg= 0x03;
							  }
							  break;
					case  K5: {
								LEDReg= 0x04;
							  }
							  break;
					case  K6: {
								LEDReg= 0x05;	
							  }
							  break;
					case  K7: {
								LEDReg= 0x06;
							  }
							  break;
					case  K8: {
								LEDReg= 0x07;
							  }	 		 
							  break; 			  
					default:  
							  break;
			}				  				  			  				   			  			  			   		
		}

 void	KeyFunction2(unsigned int KeyReg2)
		{
			switch(KeyReg2)
				{
					case  K9: {
								LEDReg= 0x08; 							
							  }
							  break;
					case  K10: {
								LEDReg= 0x09;							
							  }
							  break;
					case  K11: {
								LEDReg= 0x0A;
							  }
							  break;
					case  K12: {
								LEDReg= 0x0B;
							  }
							  break;
					case  K13: {
								LEDReg= 0x0C;
							  }
							  break;
					case  K14: {
								LEDReg= 0x0D;	
							  }
							  break;
					case  K15: {
								LEDReg= 0x0E;
							  }
							  break;
					case  K16: {
								LEDReg= 0x0F;
							  }	 		 
							  break; 			  
					default:  
							  break;
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

//MAX5742  DAC转换,SPICCR
void  DAC(unsigned int DAinput)
{  
 SpiaRegs.SPICCR.all =0x00cf;   // 使SPI处于复位方式, 下降沿, 16位数据
 	asm (" NOP ");
   	asm (" NOP ");
 	GpioDataRegs.GPFDAT.bit.GPIOF14 = 0;   //片选DAC  
	SpiaRegs.SPITXBUF = 0xf010;                //  初始化指令：DAC_A-D使能
    	while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){} 	//发送是否完成
    SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;		//读出清标志
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 1;	//片选高电平延时
    	for(i = 0; i<10; i++){}
   	GpioDataRegs.GPFDAT.bit.GPIOF14 = 0;

	SpiaRegs.SPITXBUF =DAinput|0x2000;          //  DAC_c通道置数
    	while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){}
   	SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 1; 
    	for(i=0; i<10; i++){}
	SpiaRegs.SPICCR.all =0x00c7; // 使SPI处于复位方式, 下降沿, 8位数据,切换为LED显示用 
}
//==========================================================================================
// No more.
//===================================================================================






