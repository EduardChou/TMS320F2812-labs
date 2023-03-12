/**************************************************************
** ��������: ADC���򣬶�ADCINB7ͨ���������жϷ�ʽ**
**************************************************************/

#include "DSP281x_Device.h"
interrupt void adc_isr(void);
//����ȫ�ֱ���:
Uint16 LoopCount;//��Чѭ������
Uint16 ConversionCount;//��ǰ�����0-20
Uint16 Voltage[20];//���20��ADCRESULT0ֵ

main() 
{
    InitSysCtrl();   

    EALLOW;
    SysCtrlRegs.HISPCP.all = 0x3;	// HSPCLK = SYSCLKOUT/6����25HZ
    EDIS;

	DINT;
	IER = 0x0000;
	IFR = 0x0000;       // ��ֹ���������CPU�ж�

	InitPieCtrl();       

	InitPieVectTable();	       
    InitAdc();    //��ʼ��ADCģ�飬�ú�����DSP28_Adc.c�ļ���
	EALLOW;	//ʹ��д�����Ĵ�����д����
	PieVectTable.ADCINT = &adc_isr;        //���û��жϷ������ڵ�ַ
//�����ж�������ͷ�ļ��еĶ�Ӧ����
	EDIS;       // ��ֹд�����Ĵ�����д����

    PieCtrlRegs.PIEIER1.bit.INTx6 = 1; //ʹ��PIE�е�ADCINT�ж�

	IER |= M_INT1;					// // ʹ�� CPU �ж� 1��ʹ��ȫ�� INT1

	EINT;   								// ʹ��ȫ���ж� INTM
	ERTM;	  							// ʹ��ȫ��ʵʱ�ж� DBGM
    LoopCount = 0;           				//ѭ������������
    ConversionCount = 0;      				//��ǰת�����������
// ���� ADC
    AdcRegs.ADCMAXCONV.all = 0x0000;       // ����SEQ1��1��ת��ͨ��
  	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf; //ת��ͨ��ѡ��:ADCINB7 

    AdcRegs.ADCTRL2.bit.EVA_SOC_SEQ1 = 1;  // ʹ�� EVASOC ȥ���� SEQ1
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // ʹ�� SEQ1 �ж� (ÿ�� EOS)
// ���� EVA
// ����EVA�Ѿ��� InitSysCtrl()��ʹ��;
    EvaRegs.T1CMPR =0x0080;               // ���� T1 �Ƚ�ֵ
    EvaRegs.T1PR = 0xFFFF;                 // �������ڼĴ���
    EvaRegs.GPTCONA.bit.T1TOADC = 1; // ʹ��EVA�е� EVASOC(�����ж�����ADC) 
    EvaRegs.T1CON.all = 0x1042;        // ʹ�ܶ�ʱ��1�Ƚϲ��� (������ģʽ )
//��ADCת��
    while (1)
    {
       LoopCount++;
    }
}
interrupt void  adc_isr(void)
{
  Voltage[ConversionCount] = AdcRegs.ADCRESULT0;
 
// ����Ѽ�¼��20��������¿�ʼת��
  if(ConversionCount == 19) 
  {
     ConversionCount = 0;
  }
  else ConversionCount++;
// ���³�ʼ����һ��ADCת��
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // ��λ SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;		// �� INT SEQ1λ
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // ���ж�Ӧ���źţ�׼��������һ���ж�
  return;                      
}