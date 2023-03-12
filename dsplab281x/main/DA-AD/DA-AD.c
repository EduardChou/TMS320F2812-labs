/******************************************************************************
**��������:DA-ADת��,�ü����趨ʮ������������,ͨ��MAX5742��OUTCͨ��DA0UT3ʹ��
��ת�����Ϊģ����, ��ģ������ΪADCINB6����ͨ����ת����ʮ����������������LED����ʾ**
******************************************************************************/

#include"DSP281x_Device.h"
unsigned int  DAinput=0;
unsigned int  ADoutput=0 ;	
unsigned int LEDReg;
unsigned int LED8=0;//DA����LED��ʼ��
unsigned int LED7=0;
unsigned int LED6=0;
unsigned int LED3=0;//AD���LED��ʼ��
unsigned int LED2=0;
unsigned int LED1=0;

unsigned int KeyReg1;
unsigned int KeyReg2;
unsigned long int i = 0;
void  IOinit(void);
int   KeyIn1(void);
void  KeyFunction1(unsigned int KeyReg1);
void  KeyFunction2(unsigned int KeyReg2);
void Write_LED (int LEDReg);//ͨ��SPI��LED������ʾ����
void  DAC(unsigned int DAinput);

Uint16 keyNum = 0x0000;  //��������

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

//�ж��ӳ��������
interrupt void   T4pint_isr(void);
interrupt void   adc_isr(void);

void IOinit()
{
 	EALLOW; 
    //��GPIOE0~GPIOE2����Ϊһ��I/O�����,��138����  
 	GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all&0xfff8; 
	GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all|0x0007;
    //��GPIOB8~GPIOB15����Ϊһ��I/O��,D0~D7
	GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all&0x00ff; 
	EDIS;				
}

	//SPI��ʼ���ӳ���
  void spi_intial()
 {
 	SpiaRegs.SPICCR.all =0x0047;   // ʹSPI���ڸ�λ��ʽ, �½���, ��λ����  
	SpiaRegs.SPICTL.all =0x0006;   // ����ģʽ, һ��ʱ��ģʽ,ʹ��talk, �ر�SPI�ж�.
	SpiaRegs.SPIBRR =0x007F;       //���ò�����
	SpiaRegs.SPICCR.all =SpiaRegs.SPICCR.all|0x0080;  // �˳���λ״̬	 
	EALLOW;	
    GpioMuxRegs.GPFMUX.all=0x000F;	// ����ͨ������ΪSPI����	 	
    EDIS;
  }

  void gpio_init()
{ 
	EALLOW;
	GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11=0; //GPIOA11����Ϊһ��I/O��
	GpioMuxRegs.GPADIR.bit.GPIOA11=1;		//��GPIOA11����Ϊ���   
	GpioMuxRegs.GPFMUX.bit.XF_GPIOF14 = 0;  //  GPIOF14����Ϊһ��I/O�˿�
	GpioMuxRegs.GPFDIR.bit.GPIOF14 = 1;	   //  ��GPIOF14����Ϊ���			
    EDIS;
    GpioDataRegs.GPADAT.bit.GPIOA11=0; 	//GPIOA11=0;�ö˿�Ϊ74HC595�����ź�
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 1; //  GPIOF14��1;�ö˿�Ϊʹ���ź�
}

//ADCģ���ϵ���ʱ
void Adc_PowerUP() 
{ 	
  	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;     //ADC��϶�Ͳο���·�ӵ�
  	for (i=0; i<1000000; i++){}      //����5ms��ʱ
  	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;		 //ADC��ģ���·�ӵ�
  	for (i=0; i<10000; i++){}        //����20us��ʱ
}

 void Adc_Init()
{
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;      //����-ֹͣ/����ת��ѡ��:����-ֹͣ��ʽ
    AdcRegs.ADCTRL1.bit.CPS = 1;           //��ʱ��Ԥ������:ADC_CLK=ADCLKPS/2=3.125M
  	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;      //�ɼ����ڴ�С:SH pulse/clock=16
   	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x5;    //��ʱ�ӷ�Ƶ:ADCLKPS=HSPCLK/4=6.25M
	AdcRegs.ADCMAXCONV.all = 0x0000;       //ת��ͨ����:SEQ1���е�ͨ����Ϊ1
	AdcRegs.ADCMAXCONV.all = 0x0000;       //ת��ͨ����:SEQ1���е�ͨ����Ϊ1
  	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xe; //ת��ͨ��ѡ��:ADCINB7  	 
	AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1=1;  //SEQl��EVA�Ĵ���Դ����
  	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1=1;  //ʹ���ж�		
}

void LEDdisplay(int LED8,int LED7,int LED6,int LED5,int LED4,int LED3,int LED2,int LED1)
{     
    GpioDataRegs.GPADAT.bit.GPIOA11=0; //��LACK�ź�һ���͵�ƽ
	Write_LED (LED8);//��LED���λд����,��ʾLED8
	Write_LED (LED7);
	Write_LED (LED6);
	Write_LED (LED5);
	Write_LED (LED4);
	Write_LED (LED3);
	Write_LED (LED2);
	Write_LED (LED1);//��LED���λд����,��ʾLED1

    GpioDataRegs.GPADAT.bit.GPIOA11=1; //��LACK�ź�һ���ߵ�ƽΪ����74HC595      	
} 

void main(void)
{		
   	asm (" NOP ");
   	asm (" NOP ");
   	InitSysCtrl();      //��ʼ��ϵͳ���ƼĴ���, ʱ��Ƶ��150M
	DINT;	        //�ر����жϣ����жϱ�־
	IER = 0x0000;   //�ر���Χ�ж�
	IFR = 0x0000;	//���жϱ�־
	spi_intial();   //SPI��ʼ���ӳ���
	gpio_init();    // �ر����ж�.
	IOinit();	    // I/O��ʼ���ӳ���
LEDdisplay(0,0,0,19,19,0,0,0);	//��ʼ��LED
//��ʼ��PIE���ƼĴ���
InitPieCtrl();       //DSP28_PieCtrl��C
//��ʼ��PIE�ж�������
InitPieVectTable();  //DSP28_PieVect��C;
//��ʼ��ADC�Ĵ���
 Adc_PowerUP();
 Adc_Init();

//T2������ADת��
	EvaRegs.T2PR=3750;	//T2��ʱ������
	EvaRegs.GPTCONA.bit.T2TOADC=2;	//�ɶ�ʱ��2�����ж�����A��Dת��
	EvaRegs.T2CON.all=0x084C;
	EvaRegs.T2CNT=0x0000;	//T2��������

//T4������ɨ��
	EvbRegs.T4PR=12000;	//T4��ʱ������
	EvbRegs.T4CON.all=0x1744;	//x��128��������10��24 ms
	EvbRegs.T4CNT=0x0000;//T4��������
	EvbRegs.EVBIMRB.bit.T4PINT=1;	//ʹ��T4PINT�ж�
	EvbRegs.EVBIFRB.bit.T4PINT=1;	//�����־

//#����ʼ��Xintf
	InitXintf();

//###���ж�ʹ��
	EALLOW;
//PIE�ж�ʸ����
	PieVectTable.T4PINT=&T4pint_isr;
	PieVectTable.ADCINT=&adc_isr;
	EDIS;
	PieCtrlRegs.PIEIER5.bit.INTx1=1;//ʹ��T4PINT
	PieCtrlRegs.PIEIER1.bit.INTx6=1;//ʹ��ADCINT
	IER |=(M_INT1 | M_INT5 );	//ʹ��INTl��INT5
	EINT;	//����ȫ���ж�
	ERTM;	//EnableGlobal realtime interrupt DBGM

//��ѭ��
	for(;;){;}
}

interrupt void   T4pint_isr(void)
	{
  	  LEDdisplay(LED8,LED7,LED6,19,19,ADoutput/256,(ADoutput%256)/16,ADoutput%16); //��ʾDAinput����ADoutput****  
  	    if (KeyIn1() == 1)     // ���ò��K1~K8�ӳ���
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
 	DAinput=LED8*256+LED7*16+LED6;  //DA�趨ֵ
	DAC(DAinput);   //DAת��
	EvbRegs.EVBIFRB.bit.T4PINT=1;	//�жϱ�־��
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP5; //�����´��ж�
	return;
 	    }
		if (KeyIn2() == 1)     // ���ò��K8~K16�ӳ���
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
 		DAinput=LED8*256+LED7*16+LED6;   //DA�趨ֵ 	    
		DAC(DAinput);   //DAת��
		}
	EvbRegs.EVBIFRB.bit.T4PINT=1;	//�жϱ�־��
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP5; //���´��ж�
	return; 
	}

interrupt void   adc_isr(void)
	{	
   		ADoutput=AdcRegs.ADCRESULT0 >> 4;//��AD���	
		 AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;//���жϱ�־
       PieCtrlRegs.PIEACK.all=PIEACK_GROUP1; //�����´��ж�       		                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	return;
	}

int KeyIn1(void)
{
 	 EALLOW;  
    //��GPIOB8~GPIOB15����Ϊ����,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all&0x00ff;
     EDIS;    
    GpioDataRegs.GPEDAT.all = 0xfff8;     //ѡͨKEY��8λ
    for (i=0; i<100; i++){}               //��ʱ
    //��K1~K8�Ƿ���
    if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
    {
    	for (i=0; i<100000; i++){}        //��ʱ����
        if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
        {
	   KeyReg1=GpioDataRegs.GPBDAT.all ;
    		while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //���Ƿ��Ϳ�
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
    //��GPIOB8~GPIOB15����Ϊ����,D0~D7
	GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all&0x00ff;
     EDIS;    
    GpioDataRegs.GPEDAT.all = 0xfff9;     //ѡͨKEY��8λ
    for (i=0; i<100; i++){}               //��ʱ
    //��K8~K16�Ƿ���
    if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
    {
    	for (i=0; i<100000; i++){}        //��ʱ����
        if ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff)
        {
	   KeyReg2=GpioDataRegs.GPBDAT.all ;
    		while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //���Ƿ��Ϳ�
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
//ͨ��SPI��LED������ʾ����
void Write_LED (int LEDReg)
{
Uint16 LEDcode[30]={0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,
                    0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,
                    0x8c00,0xbf00,0xa700,0xff00,0x4000,0x7900,0x2400,0x3000,
                    0x1900,0x1200,0x0200,0x7800,0x0000,0x1000};//����������0~f, P������L��"��",0.~9.	
	 		SpiaRegs.SPITXBUF =LEDcode[LEDReg]; //�����������
    	 	 while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){} 		
    		SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF; //�������־
}

//MAX5742  DACת��,SPICCR
void  DAC(unsigned int DAinput)
{  
 SpiaRegs.SPICCR.all =0x00cf;   // ʹSPI���ڸ�λ��ʽ, �½���, 16λ����
 	asm (" NOP ");
   	asm (" NOP ");
 	GpioDataRegs.GPFDAT.bit.GPIOF14 = 0;   //ƬѡDAC  
	SpiaRegs.SPITXBUF = 0xf010;                //  ��ʼ��ָ�DAC_A-Dʹ��
    	while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){} 	//�����Ƿ����
    SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;		//�������־
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 1;	//Ƭѡ�ߵ�ƽ��ʱ
    	for(i = 0; i<10; i++){}
   	GpioDataRegs.GPFDAT.bit.GPIOF14 = 0;

	SpiaRegs.SPITXBUF =DAinput|0x2000;          //  DAC_cͨ������
    	while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){}
   	SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;
    GpioDataRegs.GPFDAT.bit.GPIOF14 = 1; 
    	for(i=0; i<10; i++){}
	SpiaRegs.SPICCR.all =0x00c7; // ʹSPI���ڸ�λ��ʽ, �½���, 8λ����,�л�ΪLED��ʾ�� 
}
//==========================================================================================
// No more.
//===================================================================================






