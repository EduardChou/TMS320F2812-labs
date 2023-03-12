//###########################################################################
//
// FILE:   DSP281x_Adc.h
//
// TITLE:  DSP281x Device ADC Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Separated the MAX_CONV bit fields into two
//      |             |      | MAX_CONV1  // 3:0
//      |             |      | MAX_CONV2  // 6:4
//      |             |      | Added SEQ_OVRD bit to ADCTRL1 for RevC and after silicon
//###########################################################################

#ifndef DSP281x_ADC_H
#define DSP281x_ADC_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// ADC Individual Register Bit Definitions:
//ADC���ƼĴ���1
struct ADCTRL1_BITS {     // bits  description
    Uint16  rsvd1:4;      // 3:0   reserved
    //����
    Uint16  SEQ_CASC:1;   // 4     Cascaded sequencer mode
    //����������ģʽ
    //��λ������SEQ1��SEQ2��Ϊ����������8״̬��������������Ϊ1��16״̬
    //��������SEQ��������
    //0 ˫������ģʽ SEQ1��SEQ2��Ϊ����������8״̬����������
    //1 ����ģʽ SEQ1��SEQ2��Ϊ1��16״̬����������
    Uint16  SEQ_OVRD:1;   // 5     Sequencer override 
    Uint16  CONT_RUN:1;   // 6     Continuous run
    //�������� ��λ��������������������������ģʽ���ǿ�ʼ-ֹͣģʽ��
    //��һ��ת��������Чʱ�����ԶԸ�λ���в���������ת�����н���ʱ��
    //��λ������Ч�����磬Ϊʵ����Ч�Ĳ��������������EOS����֮ǰ
    //����λ��λ�����㡣������ת��ģʽ�в���Ҫ��λ��������Ȼ����
    //��ʼ-ֹͣģʽ�У����������뱻��λ����ʹת��������CONV00״̬��
    //0 ��ʼ-ֹͣģʽ������EOS�źź�������ֹͣ��������������λ��
    //������������ֹͣʱ��״̬��ʼ��
    //1 ����ת��ģʽ ����EOS�źź���������CONV00������SEQ1�ͼ�����������
    //��CONV08������SEQ2��״̬��ʼ��
    Uint16  CPS:1;        // 7     ADC core clock pre-scalar
    //ADC�ں�ʱ��Ԥ������
    //��Ԥ������Ӧ����������ʱ��HSPCLK��Ƶ
    //0 Fclk=CLK/1
    //1 Fclk=CLK/2
    //ע�⣺CLK=������HSPCLK��ADCCLKPS3~0��
    Uint16  ACQ_PS:4;     // 11:8  Acquisition window size
    //�ɼ����ڴ�С
    //��Щ����SOC������ͬʱҲ�����˲������رպ͵�ʱ�䡣SOC��
    //�����ǣ�ADCTRL[11:8]+1����ADCLK������
    Uint16  SUSMOD:2;     // 13:12 Emulation suspend mode
    //��������ģʽ
    //��2λ�����˲����������ʱִ�еĲ��������磬�����������ϵ㣩��
    //00 ������𱻺���
    //01 ��ǰ������ɺ��������������߼�ֹͣ�������������ս��������״̬����
    //10 ��ǰת����ɺ��������������߼�ֹͣ�������������ս��������״̬����
    //11 �������ʱ���������������߼�����ֹͣ��
    Uint16  RESET:1;      // 14    ADC reset
    //ADC��λ
    //��λ����ʹ����ADCģ�鸴λ����оƬ�ĸ�λ���ű����ͣ���һ���ϵ縴λ��ʱ��
    //���еļĴ�������������λ����ʼ״̬��
    //����һ��һ���Ե�Ӱ��λ��Ҳ����˵������1��������Զ����㡣
    //��ȡ��λʱ����0��ADC�ĸ�λ�ź���Ҫ����3��ʱ�����ڣ���ADC��λ��3��ʱ��
    //�����ڲ��ܸı�ADC�Ŀ��ƼĴ�����
    //0 û��Ӱ��
    //1 ��λ����ADCģ�飨ADC�����߼�����λ��0��
    //ע�⣺��ϵͳ��λ�ڼ䣬ADCģ�鱻��λ���������һʱ����Ҫ��ADCģ�鸴λ��
    //�û������λд1����12���ղ������û�����Ҫ������ֵд��ADCTR1�Ĵ�����
    //MOV ADCTR1,#01xxxxxxxxxxxxxx;��λADCģ�飨RESET=1��
    //RPT 12#
    //NOP ;��ADCTR1�Ĵ����ı�����ǰ��Ҫ���ӳ�
    //NOP
    //MOV ADCTR1,#00xxxxxxxxxxxxxx;����ADCTR1�Ĵ���
    //ע�⣺���Ĭ����������ϵͳҪ�󣬿��Բ�ʹ�õڶ���MOV�ı���ƼĴ��������á�
    Uint16  rsvd2:1;      // 15    reserved
    //������������0��дû��Ӱ�졣
};


union ADCTRL1_REG {
   Uint16                all;
   struct ADCTRL1_BITS   bit;
};

//ADC���ƼĴ���2
struct ADCTRL2_BITS {         // bits  description
    Uint16  EVB_SOC_SEQ2:1;   // 0     Event manager B SOC mask for SEQ2
    //SEQ2���¼�������B��SOC����λ
    //0 EVB�Ĵ����źŲ�������SEQ2
    //1 �����¼�������B�����ź�����SEQ2�����Զ��¼���������̣����ø����¼�����
    //ת����
    Uint16  rsvd1:1;          // 1     reserved
    //������������0��дû��Ӱ�졣
    Uint16  INT_MOD_SEQ2:1;   // 2     SEQ2 Interrupt mode
    //SEQ2�ж�ģʽ
    //��λѡ��SEQ2���ж�ģʽ����SEQ2ת�����н���ʱ����Ӱ��INT SEQ2�����á�
    //0 ÿ��SEQ2����ʱ��INT SEQ2��λ
    //1 ÿ��һ��SEQ2���н���ʱ��INT SEQ2��λ��
    Uint16  INT_ENA_SEQ2:1;   // 3     SEQ2 Interrupt enable
    //SEQ2�ж�ʹ��
    //0 ��ֹINT SEQ2�������ж�
    //1 ʹ��INT SEQ2�������ж�
    Uint16  rsvd2:1;          // 4     reserved
    Uint16  SOC_SEQ2:1;       // 5     Start of conversion for SEQ2
    //SEQ2�Ĵ����źţ�����������˫������ģʽ;�ڼ���ģʽ�в�ʹ�á�
    //���д�������ʹ��λ��λ��
    //S/W��������λд1
    //EVB  �¼�������B
    Uint16  RST_SEQ2:1;       // 6     Reset SEQ2
    //��λSEQ2
    //0 �޲���
    //1 ������λSEQ2����ʼ��"Ԥ����"״̬��������CONV08״̬�ȴ�����������
    //�˳�����ִ�е�ת�����С�
    Uint16  EXT_SOC_SEQ1:1;   // 7     External start of conversion for SEQ1
    //SEQ1���ⲿ�����ź�
    //0 �޲���
    //1 �ⲿADCSOC�����ź�����ADC�Զ�ת�����С�
    Uint16  EVA_SOC_SEQ1:1;   // 8     Event manager A SOC mask for SEQ1
    //SEQ1���¼�������B��SOC����λ
    //0 EVA�Ĵ����źŲ�������SEQ21
    //1 �����¼�������A�����ź�����SEQ1�����Զ��¼���������̣����ø����¼�����
    //ת����
    Uint16  rsvd3:1;          // 9     reserved
    Uint16  INT_MOD_SEQ1:1;   // 10    SEQ1 Interrupt mode
    //SEQ1�ж�ģʽ
    //��λѡ��SEQ1���ж�ģʽ����SEQ1ת�����н���ʱ����Ӱ��INT SEQ1�����á�
    //0 ÿ��SEQ1����ʱ��INT SEQ1��λ
    //1 ÿ��һ��SEQ1���н���ʱ��INT SEQ1��λ��
    Uint16  INT_ENA_SEQ1:1;   // 11    SEQ1 Interrupt enable
    //SEQ1�ж�ʹ��
    //0 ��ֹINT SEQ1�������ж�
    //1 ʹ��INT SEQ1�������ж�
    Uint16  rsvd4:1;          // 12    reserved
    Uint16  SOC_SEQ1:1;       // 13    Start of conversion trigger for SEQ1
    //SEQ1�Ĵ����ź�
    //���д�������ʹ��λ��λ��
    //S/W��������λд1
    //EVA  �¼�������A
    //EVB  �¼�������B
    //EXT  �ⲿ���ţ�����ADCSOC���ţ�
    //������Դ����ʱ,��3�ֿ��ܵ����
    //���1 SEQ1������SOCλ��0
    //SEQ1������ʼ(�ٲÿ���).�����κ�"����"��������.
    //���2 SEQ1æ��SOCλ��0
    //��λ����λ��ʾ��һ������������������.����ɵ�ǰת��,SEQ1
    //���¿�ʼʱ,��λ����0.
    //���3 SEQ1æ��SOCλ��λ
    //������������κδ�����������(��ʧ)
    //0 ���һ�����ڹ����SOC����.
    //ע��:����������Ѿ�����,��λ���Զ����,���,���λд0�������κ�����.
    //����,�������λ�ķ�������ֹͣһ���Ѿ�����������
    //1 ������� �ӵ�ǰֹͣ��λ������SEQ1(����,�ڿ���ģʽ��)
    //ע��:RST SEQ1(ADCTRL2.14)��SOC SEQ1(ADCTRL2.13)λ��Ӧ��ͬ����ָ������,
    //��Ḵλ������,����������������.��ȷ������������������� RST SEQ1λ,
    //Ȼ������һָ������SOC SEQ1λ.��ᱣ֤��λ������������һ���µ�����.
    //��������ҲӦ����RST SEQ2(ADCTRL2.6)��SOC SQE2(ADCTRL2.5)λ
    Uint16  RST_SEQ1:1;       // 14    Restart sequencer 1
    //0 ��������
    //1 ��������������λ��CONV00״̬   
    Uint16  EVB_SOC_SEQ:1;    // 15    EVB SOC enable
    //0 ��������
    //1 ��λ��λ,�����¼�������B���ź���������������,���Զ��¼����������,
    //ʹ�ø����¼�����ת��.
};


union ADCTRL2_REG {
   Uint16                all;
   struct ADCTRL2_BITS   bit;
};

//ADC�Զ�����״̬�Ĵ���
struct ADCASEQSR_BITS {       // bits   description
    Uint16  SEQ1_STATE:4;     // 3:0    SEQ1 state
    Uint16  SEQ2_STATE:3;     // 6:4    SEQ2 state
    //����TI����֮��,���ṩ���û�ʹ��.
    Uint16  rsvd1:1;          // 7      reserved
    Uint16  SEQ_CNTR:4;       // 11:8   Sequencing counter status 
    //����������״̬λ
    //SEQ1��SEQ2�ͼ���������ʹ��SEQ CNTRn4λ����״̬λ,�ڼ���ģʽ����SEQ2�޹�.
    //��ת����ʼ,�������ļ���λSEQCNTR(3~0)��ʼ��������MAX CONV�е�ֵ.ÿ���Զ�
    //����ת�����(��ͬ������ģʽ�е�һ��ת�����)��,������������1.
    //�ڵݼ�������������ʱ���Զ�ȡSEQ CNTRnλ,�����������״̬.��ȡ��ֵ��SEQ1
    //��SEQ2��æλһ���ʾ������ִ�е�����״̬.
    //SEQ CNTRn    �ȴ�ת����ͨ����
    //0000         1��0 ȡ����busy״̬
    //0001            2
    //0010            3
    //0011            4
    //...            ...
    //1111            16
    Uint16  rsvd2:4;          // 15:12  reserved  
};

union ADCASEQSR_REG {
   Uint16                 all;
   struct ADCASEQSR_BITS  bit;
};

//ADC���ת��ͨ���Ĵ���
struct ADCMAXCONV_BITS {      // bits  description
    Uint16  MAX_CONV1:4;      // 3:0   Max number of conversions
    Uint16  MAX_CONV2:3;      // 6:4   Max number of conversions
    //MAX CONVn�������Զ�ת�������ת����ͨ����,��λ�����������Ĺ���ģʽ
    //�仯���仯.
    //����SEQ1 ʹ��MAX_CONV1_2_0
    //����SEQ2 ʹ��MAX_CONV2_2_0
    //����SEQ  ʹ��MAX_CONV1_3_0
    //�Զ�ת���������Ǵӳ�ʼ״̬��ʼ,һ��������ת��ֱ������״̬,����ת��
    //�����˳��װ�ص�����Ĵ�����.ÿ��ת�����п���ת��1-(MAX CONVn+1)��
    //ͨ��,ת����ͨ�������Ա��.    
    Uint16  rsvd1:9;          // 15:7  reserved 
};

union ADCMAXCONV_REG {
   Uint16                  all;
   struct ADCMAXCONV_BITS  bit;
};

//ADC����ͨ��ѡ��������ƼĴ���
struct ADCCHSELSEQ1_BITS {    // bits   description
    Uint16  CONV00:4;         // 3:0    Conversion selection 00
    Uint16  CONV01:4;         // 7:4    Conversion selection 01
    Uint16  CONV02:4;         // 11:8   Conversion selection 02
    Uint16  CONV03:4;         // 15:12  Conversion selection 03
    //ÿ1��CONV0nΪ4λ,�ɱ��ѡ��16��ͨ���е�1��.
    //0000 ADCINA0
    //...  ...
    //0111 ADCINA7
    //1000 ADCINB0
    //...  ...
    //1111 ADCINB7
};

union  ADCCHSELSEQ1_REG{
   Uint16                    all;
   struct ADCCHSELSEQ1_BITS  bit;
};

struct ADCCHSELSEQ2_BITS {    // bits   description
    Uint16  CONV04:4;         // 3:0    Conversion selection 04
    Uint16  CONV05:4;         // 7:4    Conversion selection 05
    Uint16  CONV06:4;         // 11:8   Conversion selection 06
    Uint16  CONV07:4;         // 15:12  Conversion selection 07
};

union  ADCCHSELSEQ2_REG{
   Uint16                    all;
   struct ADCCHSELSEQ2_BITS  bit;
};

struct ADCCHSELSEQ3_BITS {    // bits   description
    Uint16  CONV08:4;         // 3:0    Conversion selection 08
    Uint16  CONV09:4;         // 7:4    Conversion selection 09
    Uint16  CONV10:4;         // 11:8   Conversion selection 10
    Uint16  CONV11:4;         // 15:12  Conversion selection 11
};

union  ADCCHSELSEQ3_REG{
   Uint16                    all;
   struct ADCCHSELSEQ3_BITS  bit;
};

struct ADCCHSELSEQ4_BITS {    // bits   description
    Uint16  CONV12:4;         // 3:0    Conversion selection 12
    Uint16  CONV13:4;         // 7:4    Conversion selection 13
    Uint16  CONV14:4;         // 11:8   Conversion selection 14
    Uint16  CONV15:4;         // 15:12  Conversion selection 15
};

union  ADCCHSELSEQ4_REG {
   Uint16                    all;
   struct ADCCHSELSEQ4_BITS  bit;
};
//ADC���ƼĴ���3
struct ADCTRL3_BITS {         // bits   description
    Uint16   SMODE_SEL:1;     // 0      Sampling mode select
    //����ģʽѡ��
    //0 ѡ��˳�����ģʽ
    //1 ѡ��ͬ������ģʽ
    Uint16   ADCCLKPS:4;      // 4:1    ADC core clock divider
    //ADC���ں�ʱ�ӷ�Ƶ��
    //0000 ADCLK=HSPCLK/(ADCTRL1[7]+1)
    //0001 ADCLK=HSPCLK/[2x(ADCTRL1[7]+1)]
    //...  ...   ...    ...
    //1111 ADCLK=HSPCLK/[30x(ADCTRL1[7]+1)]
    Uint16   ADCPWDN:1;       // 5      ADC powerdown
    //ADC��Դ����
    //0 ����϶�Ͳο���·���ADC����ģ���·����
    //1 ����϶�Ͳο���·���ADC����ģ���·�ϵ�
    Uint16   ADCBGRFDN:2;     // 7:6    ADC bandgap/ref power down
    //ADC��϶�Ͳο��ĵ�Դ����
    //0 ��϶�Ͳο���·����
    //1 ��϶�Ͳο���·�ϵ�
    Uint16   rsvd1:8;         // 15:8   reserved
}; 

union  ADCTRL3_REG {
   Uint16                all;
   struct ADCTRL3_BITS   bit;
};

//ADC״̬�ͱ�־�Ĵ���
struct ADCST_BITS {           // bits   description
    Uint16   INT_SEQ1:1;      // 0      SEQ1 Interrupt flag  
    Uint16   INT_SEQ2:1;      // 1      SEQ2 Interrupt flag
    Uint16   SEQ1_BSY:1;      // 2      SEQ1 busy status
    Uint16   SEQ2_BSY:1;      // 3      SEQ2 busy status
    Uint16   INT_SEQ1_CLR:1;  // 4      SEQ1 Interrupt clear
    Uint16   INT_SEQ2_CLR:1;  // 5      SEQ2 Interrupt clear
    Uint16   EOS_BUF1:1;      // 6      End of sequence buffer1
    Uint16   EOS_BUF2:1;      // 7      End of sequence buffer2
    Uint16   rsvd1:8;         // 15:8   reserved
};


union  ADCST_REG {
   Uint16             all;
   struct ADCST_BITS  bit;
};


struct ADC_REGS {
    union ADCTRL1_REG      ADCTRL1;       // ADC Control 1
    union ADCTRL2_REG      ADCTRL2;       // ADC Control 2
    union ADCMAXCONV_REG   ADCMAXCONV;    // Max conversions
    union ADCCHSELSEQ1_REG ADCCHSELSEQ1;  // Channel select sequencing control 1
    union ADCCHSELSEQ2_REG ADCCHSELSEQ2;  // Channel select sequencing control 2
    union ADCCHSELSEQ3_REG ADCCHSELSEQ3;  // Channel select sequencing control 3
    union ADCCHSELSEQ4_REG ADCCHSELSEQ4;  // Channel select sequencing control 4
    union ADCASEQSR_REG    ADCASEQSR;     // Autosequence status register
    //ADC����Ĵ���
    Uint16                 ADCRESULT0;    // Conversion Result Buffer 0
    Uint16                 ADCRESULT1;    // Conversion Result Buffer 1
    Uint16                 ADCRESULT2;    // Conversion Result Buffer 2
    Uint16                 ADCRESULT3;    // Conversion Result Buffer 3
    Uint16                 ADCRESULT4;    // Conversion Result Buffer 4
    Uint16                 ADCRESULT5;    // Conversion Result Buffer 5
    Uint16                 ADCRESULT6;    // Conversion Result Buffer 6
    Uint16                 ADCRESULT7;    // Conversion Result Buffer 7
    Uint16                 ADCRESULT8;    // Conversion Result Buffer 8
    Uint16                 ADCRESULT9;    // Conversion Result Buffer 9
    Uint16                 ADCRESULT10;   // Conversion Result Buffer 10
    Uint16                 ADCRESULT11;   // Conversion Result Buffer 11
    Uint16                 ADCRESULT12;   // Conversion Result Buffer 12
    Uint16                 ADCRESULT13;   // Conversion Result Buffer 13
    Uint16                 ADCRESULT14;   // Conversion Result Buffer 14
    Uint16                 ADCRESULT15;   // Conversion Result Buffer 15
    union ADCTRL3_REG      ADCTRL3;       // ADC Control 3  
    union ADCST_REG        ADCST;         // ADC Status Register
};


//---------------------------------------------------------------------------
// ADC External References & Function Declarations:
//
extern volatile struct ADC_REGS AdcRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of DSP281x_ADC_H definition

//===========================================================================
// No more.
//===========================================================================
