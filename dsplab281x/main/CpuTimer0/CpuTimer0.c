// ����:����CPU��ʱ��0ÿ�ζ�ʱ�����������������������DS22��DS23,ʵ��ָʾ�ƵĶ�ʱ��˸

#include "DSP281x_Device.h"     // DSP281xͷ�ļ�
#include "DSP281x_Examples.h"   // DSP281x Examples ͷ�ļ�

unsigned int Led_Flag;
#define		LED0_OFF	GpioDataRegs.GPFDAT.bit.GPIOF11=1
#define		LED0_ON		GpioDataRegs.GPFDAT.bit.GPIOF11=0 //����DS23
#define		LED1_OFF	GpioDataRegs.GPFDAT.bit.GPIOF10=1
#define		LED1_ON		GpioDataRegs.GPFDAT.bit.GPIOF10=0 //����DS22
void IOinit()
{
 	EALLOW;  
 	//��GPIOF8~GPIOF13����Ϊһ��I/O�ڣ����
 	GpioMuxRegs.GPFMUX.all = 0xc0ff;
    GpioMuxRegs.GPFDIR.all = 0x3f00;   
  	EDIS;			
}

interrupt void cpu_timer0_isr(void);//�ж�����

void main(void)
{

// Step 1. //ϵͳ��ʼ��(��DSP281x_SysCtrl.c��)
// PLL, WatchDog, enable Peripheral Clocks 
   InitSysCtrl();//ϵͳ��ʼ��

// Step 2. ��ʼ�� GPIO:
   IOinit();
   GpioDataRegs.GPFDAT.all = 0x3f00;//GPIOF8~GPIOF13�øߵ�ƽ,ָʾ����
  
   Led_Flag=1;	//����ָʾ�Ʊ�־λ
   
// Step 3. ���жϼ���ʼ��PIEʸ����:
   DINT;	/*��CPU�ж�*/

   InitPieCtrl();//��ʼ��PIE���ƼĴ��� DSP281x_PieCtrl.c   
   IER = 0x0000;//�ر���Χ�ж�
   IFR = 0x0000;//���жϱ�־
 
   InitPieVectTable();//��ʼ��PIEʸ����DSP281x_PieVect.c
  
   EALLOW;  //дEALLOW�����Ĵ���
   PieVectTable.TINT0 = &cpu_timer0_isr;//PIE�ж�ʸ����
   EDIS;    

   InitCpuTimers();   // For this example, only initialize the Cpu Timers
//��ʱ����һ��������£������ڼĴ�����PRDH:PRD�е�ֵװ��32λ�����Ĵ���TIMH:TIM��
//Ȼ������Ĵ�����C28X ��SYSCLKOUT���ʵݼ���������������0ʱ���ͻ����һ����ʱ��
//�ж�����źţ�һ���ж����壩��
// 150MHz CPU Freq, 1 second Period (in uSeconds)
//�����жϵļ��=�����ڼĴ����е�ֵ��/SYSCLKOUT/��Ƶϵ��
//���ڼĴ����е�ֵ=(Freq*Period)
   ConfigCpuTimer(&CpuTimer0, 150, 1000000);
   StartCpuTimer0();

   IER |= M_INT1;	//ʹ��CPU INT1 

   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//CPU-Timer0ʹ��λ��PIE��1���7��������ʹ��

   EINT;   //����ȫ���ж�

   for(;;);
} 


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
	if(Led_Flag == 1)
	{
		LED0_OFF;
		LED1_ON;	//����DS22
		Led_Flag = 0;
	}
	else
	{
		LED0_ON;	//����DS23
		LED1_OFF;
		Led_Flag = 1;
	}
  
     PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;// ���ж�Ӧ���źţ�׼��������һ���ж�
}

//===========================================================================
// No more.
//===========================================================================
