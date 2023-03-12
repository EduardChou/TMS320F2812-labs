/***************************************************************************
** ��������:���ò�ѯ��, ��ADCINB7ͨ�����в���,��ʾʵ��ֵ����Ӧ��10��������ֵ
***************************************************************************/
#include "DSP281x_Device.h"
// ��Щ����������������(��F2812.cmd)������3��ָ��ΪFlash����ר��
//############################################################################################
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
//###########################################################################################
unsigned long int  AD1;  
unsigned long int AD2;  //ʵ��ADֵ*1000
void Write_LED (int LEDReg);//ͨ��SPI��LED������ʾ����
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
    EDIS;
    GpioDataRegs.GPADAT.bit.GPIOA11=0; 	//GPIOA11=0;�ö˿�Ϊ74HC595�����ź�
}

/*LED��ʾ�ӳ���*/
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
/*�ӳ������*/

unsigned long int i = 0;

//ADC��ؼĴ�����ʼ��
void Adc_Init()
{
	// Configure ADC @SYSCLKOUT = 150Mhz
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;      //˫����/����ѡ��:˫���й���ģʽ
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;     //����/����ѡ��:����������ʽ
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;      //����-ֹͣ/����ת��ѡ��:����-ֹͣ��ʽ
    AdcRegs.ADCTRL1.bit.CPS = 1;           //��ʱ��Ԥ������:ADC_CLK=ADCLKPS/2=3.125M
  	AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;      //�ɼ����ڴ�С:SH pulse/clock=16
   	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x2;    //��ʱ�ӷ�Ƶ:ADCLKPS=HSPCLK/4=6.25M
	AdcRegs.ADCMAXCONV.all = 0x0000;       //ת��ͨ����:SEQ1���е�ͨ����Ϊ1
  	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf; //ת��ͨ��ѡ��:ADCINB7 
}

//ADCģ���ϵ���ʱ
void Adc_PowerUP() 
{ 	
  	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;     //ADC��϶�Ͳο���·�ӵ�
  	for (i=0; i<1000000; i++){}      //����5ms��ʱ
  	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;		 //ADC��ģ���·�ӵ�
  	for (i=0; i<10000; i++){}        //����20us��ʱ
}

main() 
{
    InitSysCtrl(); //ϵͳ��ʼ��
     EALLOW; 
     SysCtrlRegs.HISPCP.all = 0x3;	//��������ʱ��HSPCLK=SYSCLKOUT/6=25Mhz
     EDIS;  
    DINT;           // �ر����ж�	
	IER = 0x0000;	// �ر���Χ�ж�
	IFR = 0x0000;   // ���жϱ�־
	spi_intial();                    //SPI��ʼ���ӳ���
	gpio_init();	                 //GPIO��ʼ���ӳ���
	Adc_PowerUP();
   	Adc_Init();
   // RamfuncsLoadStart, RamfuncsLoadEnd,��RamfuncsRunStart���������������ɡ�
		// �ο�F2812.cmd�ļ�,����2��ָ��ΪFlash����ר��
//############################################################################################
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);		
   InitFlash();		// ����Flash��ʼ�������Ա�����Flash�ȴ�״̬�������������פ����RAM�С�
//############################################################################################	 
   	while (1)
   	{
   	    int e=0;
   		AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;   //��S/W��ʽ����SEQ1ת������
   		while (AdcRegs.ADCST.bit.SEQ1_BSY == 1){}    //������æ��
   		AD1 = AdcRegs.ADCRESULT0 >> 4;
		AD2=(AD1*3*1000)/4095;   //ʵ��ADֵ*1000
   		e++;
          if(e>0)
            {   		
              LEDdisplay((AD2/1000)+20,(AD2%1000)/100,(AD2%100)/10,19,
              AD1/1000,(AD1%1000)/100,(AD1%100)/10,AD1%10);
              e=0;
            }
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
//=========================================================================================
// No more.
//=========================================================================================

