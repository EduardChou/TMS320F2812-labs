//ÿ��PWM���ڶ��޸ıȽ�ֵ�ķ�ʽ
//PWM����Ϊ20ms�����Ҳ�����Ϊ2s��ÿ����100��PWM����
//��n����ռ�ձ�Ϊ y = [sin(0.02 * n * pi) + 1]/2
//PWM���ڼ���Ϊ11718���ɴ˿���Ƚ�ֵ
#include "DSP281x_Device.h"
//#include "PF.h"

void Init_Sys(void);    /*��ʼ��ϵͳ*/
void Init_IOF(void);    /*��ʼ��IO*/
void Init_timer0(void); /*��ʼ��CPUTIMER*/
void Init_CPUint(void); /*��ʼ��CPU_int*/
void Init_PIEint(void); /*��ʼ��CPU_int*/
void Init_EV(void);     /*��ʼ��EV*/

//#define  PIEVect1_1  0x000D40
//#define  PIEVect1_7  (PIEVect1_1+6*2)
//#define  PIEVect2_1  (PIEVect1_1+16+0*2)
//#define  PIEVect2_2  (PIEVect1_1+16+1*2)
//#define  PIEVect2_3  (PIEVect1_1+16+2*2)
//#define  PIEVect2_4  (PIEVect1_1+16+3*2)
//#define  TINT0Vector    * ((PINT *) PIEVect1_7)
//#define  CMP1INTVector  * ((PINT *) PIEVect2_1)
//#define  CMP2INTVector  * ((PINT *) PIEVect2_2)
//#define  CMP3INTVector  * ((PINT *) PIEVect2_3)
//#define  T1PINTVector   * ((PINT *) PIEVect2_4)

//interrupt void CPU_timer0_isr(void);
interrupt void eva_T1PINT_isr(void);
interrupt void eva_t1cmp_isr(void);
interrupt void eva_CMP1INT_isr(void);
interrupt void eva_CMP2INT_isr(void);
interrupt void eva_CMP3INT_isr(void);
interrupt void evb_T3PINT_isr(void);
interrupt void evb_t3cmp_isr(void);
interrupt void evb_CMP4INT_isr(void);
interrupt void evb_CMP5INT_isr(void);
interrupt void evb_CMP6INT_isr(void);

Uint16 led;
int prdnum=0;
int led0_cmp;
int led1_cmp;
int led2_cmp;
int led3_cmp;
int led4_cmp;
int led5_cmp;
int led6_cmp;
int led7_cmp;
int led_tab[100]={
587,585,583,580,576,571,565,558,549,540,
530,518,506,493,479,465,449,434,417,400,
383,365,347,329,311,293,274,256,238,220,
202,185,168,151,136,120,106,92 ,79 ,67 ,
55 ,45 ,36 ,27 ,20 ,14 ,9  ,5  ,2  ,2  ,
2  ,2  ,2  ,5  ,9  ,14 ,20 ,27 ,36 ,45 ,
55 ,67 ,79 ,92 ,106,120,136,151,168,185,
202,220,238,256,274,292,311,329,347,365,
383,400,417,434,449,465,479,493,506,518,
530,540,549,558,565,571,576,580,583,585
    };

void main(void)
{ int i;
  DINT;
  EALLOW;
  Init_Sys();   /*��ʼ��ϵͳ*/
  for(i=0;i<10000;i++){}
  Init_IOF();       /*��ʼ��IO*/
//  Init_timer0();  /*��ʼ��CPUTIMER*/
  Init_CPUint();    /*��ʼ��CPU_int*/
       /*��ʼ��EV*/
  Init_PIEint();    /*��ʼ��CPU_int*/
  Init_EV();

  PieCtrlRegs.PIEACK.all =10;
  EDIS;
  EINT;

  led0_cmp=0;//����λ����
  led1_cmp=7;
  led2_cmp=14;
  led3_cmp=21;
  led4_cmp=28;
  led5_cmp=35;
  led6_cmp=42;
  led7_cmp=49;



  for(;;)
  {asm(" IDLE");
   asm(" IDLE");
  }
}   
void Init_Sys(void) /*��ʼ��ϵͳ*/
{
  SysCtrlRegs.PLLCR.all=0xA;
  SysCtrlRegs.PCLKCR.all=0xffff;//ʹ������ʱ��
  SysCtrlRegs.HISPCP.all=0x1;   //defaultĬ������
  SysCtrlRegs.LOSPCP.all=0x2;   //default
  SysCtrlRegs.WDCR=0x68;    //disable w d
  SysCtrlRegs.LPMCR0.all=0x0;   //Low power --Idle

}
void Init_IOF(void)     /*��ʼ��IO*/
{int i;
  GpioMuxRegs.GPFMUX.all=0x00ff;    //f8~f13����Ϊͨ��io��
  GpioMuxRegs.GPFDIR.all=0x3f00;//��������Ϊ���
  GpioDataRegs.GPFDAT.all=0x0700;//�ұ�������������

  GpioMuxRegs.GPEMUX.all=0x0;   //e��ȫio
  GpioMuxRegs.GPEDIR.all=0xFF;//ȫ���
  GpioDataRegs.GPEDAT.all=0xFFFF;//ȫ1

  GpioMuxRegs.GPBMUX.all=0x0;   //b��ȫio����ֻ�õ���8λ
  GpioMuxRegs.GPBDIR.all=0xFF00;//��8λΪ���
  GpioDataRegs.GPBDAT.all=0xff00;//��8λȫΪ1��ȫϨ��

  GpioDataRegs.GPEDAT.all=0xFFFA;//e�����λ1010/////////Ϊʲô����Ҫ��������e��
  for(i=0;i<100;i++){}
  GpioDataRegs.GPEDAT.all=0xFFFB;//e�����λ1011������
  for(i=0;i<100;i++){}
  GpioDataRegs.GPEDAT.all=0xFFFF;//e�ڵ�λ1111������

 // GpioMuxRegs.GPAMUX.all=0x1; //��PWM1 active����Ϊ��������жϵķ�ʽ���Բ���pwm��
 // GpioMuxRegs.GPADIR.all=0x3F;    //GPIOA_3~5 1111111��6λ���
 // GpioDataRegs.GPADAT.all=0x00;//��ʾ������
}
void Init_timer0(void)  /*��ʼ��CPUTIMER��������Ӧ���ò�����ʱ����*/
{
    CpuTimer0Regs.TPR.all=150;  //25ms����һ���ж�
    CpuTimer0Regs.TPRH.all=0;
    CpuTimer0Regs.PRD.all=(long)(1000*25);
    CpuTimer0Regs.TCR.all=0xf000;
}
void Init_CPUint(void)  /*��ʼ��CPU_int*/
{
  IFR=0x0;//��������жϱ�־λ
  IER=0X000f;//����������ж�ʹ��
}
void Init_PIEint(void)  /*��ʼ��CPU_int*/
{
    PieCtrlRegs.PIEIFR1.all=0;
    PieCtrlRegs.PIEIER2.all=0x1f;//2���ж�ʱ�������жϺͱȽ��ж��Լ��Ƚ����Ƚ��ж�ʹ�ܣ����5λ
    PieCtrlRegs.PIEIFR2.all=0;
    PieCtrlRegs.PIEIER4.all=0x1f;//4���ж�ʱ���Ƚ��ж��Լ��Ƚ����Ƚ��ж�ʹ��
    PieCtrlRegs.PIEIFR4.all=0;
    PieCtrlRegs.PIEACK.all =0;
    PieCtrlRegs.PIECRTL.all=1;

  PieVectTable.T1CINT= & eva_t1cmp_isr;//��ʱ��1�Ƚ��ж�
  PieVectTable.CMP1INT= & eva_CMP1INT_isr;//ȡ���������ʵ���жϺ����ĵ�ַ
  PieVectTable.CMP2INT= & eva_CMP2INT_isr;
  PieVectTable.CMP3INT= & eva_CMP3INT_isr;
  PieVectTable.T1PINT=  & eva_T1PINT_isr;//��ʱ��1�����ж�
  PieVectTable.T3PINT=  & evb_T3PINT_isr;
  PieVectTable.T3CINT= & evb_t3cmp_isr;//��ʱ��3�����ж�
  PieVectTable.CMP4INT= & evb_CMP4INT_isr;//ȡ���������ʵ���жϺ����ĵ�ַ
  PieVectTable.CMP5INT= & evb_CMP5INT_isr;
  PieVectTable.CMP6INT= & evb_CMP6INT_isr;

}
void Init_EV(void)      /*��ʼ��EV*/
{
    EvaRegs.EXTCONA.all=1;//����Ϊ��������
    EvaRegs.GPTCONA.all=0x0010;//��ʱ��1�Ƚ����ʹ��,�Ƚ��������Ϊǿ�Ƶ�,��Ϊ�ò���pwm��
    EvaRegs.T1CON.all=0x17ca;//0001 0111 1100 1010������������Ԥ��������Ϊ/128,��������Ϊ�������أ�������Ϊװ��ʱ�������ʾ��׼
    EvaRegs.T1PR   =586;
    EvaRegs.COMCONA.all=0x82e0;//1000 0010 1110 0000ʹ�ܱȽϲ�����ʹ�ܱȽ���������ԱȽ���1/2/3���ʹ��
    EvaRegs.ACTRA.all=0x1;//�Ƚ��������CMP1�����������Ϊ����Ч,����ʵ�����õ�������
    EvaRegs.DBTCONA.all=0;//������������
    EvaRegs.T1CMPR=led0_cmp;
    EvaRegs.CMPR1=led1_cmp;
    EvaRegs.CMPR2=led2_cmp;
    EvaRegs.CMPR3=led3_cmp;
    EvaRegs.EVAIMRA.all=0x018e;//0000 0001 1000 1110��ʱ��1�ȽϺ������ж�ʹ�ܣ��Ƚϵ�Ԫ1/2/3�Ƚ��ж�ʹ��
    EvaRegs.EVAIFRA.all=0;//

    EvbRegs.EXTCONB.all=1;//����Ϊ��������
    EvbRegs.GPTCONB.all=0x0010;//��ʱ��3�Ƚ����ʹ��,�Ƚ��������Ϊǿ�Ƶ�,��Ϊ�ò���pwm��
    EvbRegs.T3CON.all=0x17ca;//0001 0111 1100 1010������������Ԥ��������Ϊ/128,��������Ϊ�������أ�������Ϊװ��ʱ�������ʾ��׼
    EvbRegs.T3PR   =586;
    EvbRegs.COMCONB.all=0x82e0;//1000 0010 1110 0000ʹ�ܱȽϲ�����ʹ�ܱȽ���������ԱȽ���1/2/3���ʹ��
    EvbRegs.ACTRB.all=0x1;//�Ƚ��������CMP1�����������Ϊ����Ч,����ʵ�����õ�������
    EvbRegs.DBTCONB.all=0;//������������
    EvbRegs.T3CMPR=led4_cmp;
    EvbRegs.CMPR4=led5_cmp;
    EvbRegs.CMPR5=led6_cmp;
    EvbRegs.CMPR6=led7_cmp;
    EvbRegs.EVBIMRA.all=0x018e;//0000 0001 1000 1110��ʱ��1�ȽϺ������ж�ʹ�ܣ��Ƚϵ�Ԫ1/2/3�Ƚ��ж�ʹ��
    EvbRegs.EVBIFRA.all=0;//
}
void ledout(Uint16 led)//��Ƴ���
{
    //����ߵ�
    //led=0x0000;
    int i;
    GpioDataRegs.GPBDAT.all=led;//
    GpioDataRegs.GPEDAT.all=2;
    for(i=0;i<10;i++){}
    GpioDataRegs.GPEDAT.all=7;
    //���ұߵ�
    GpioDataRegs.GPBDAT.all=led*256;
    GpioDataRegs.GPEDAT.all=3;
    for(i=0;i<10;i++){}
    GpioDataRegs.GPEDAT.all=7;
}

interrupt void CPU_timer0_isr()
{
    PieCtrlRegs.PIEACK.all=1;
}
interrupt void eva_T1PINT_isr()//�����ж�1ִ�еĲ���,ͬʱ����һ����ʱ��Ҳ�����ж�
{int i;
led=0x00ff;//��8λ������Ϊ0,��8λ������1
ledout(led);//���
  for(i=0;i<100;i++){}
  //GpioDataRegs.GPEDAT.all=0xFFFF;

  prdnum++;
  if(prdnum>=20)
  {prdnum=0;
   led0_cmp++;
   if(led0_cmp>=100) led0_cmp=0;
   led1_cmp++;
   if(led1_cmp>=100) led1_cmp=0;
   led2_cmp++;
   if(led2_cmp>=100) led2_cmp=0;
   led3_cmp++;
   if(led3_cmp>=100) led3_cmp=0;
   led4_cmp++;
   if(led4_cmp>=100) led4_cmp=0;
   led5_cmp++;
   if(led5_cmp>=100) led5_cmp=0;
   led6_cmp++;
   if(led6_cmp>=100) led6_cmp=0;
   led7_cmp++;
   if(led7_cmp>=100) led7_cmp=0;
   EvaRegs.T1CMPR=led_tab[led0_cmp];
   EvaRegs.CMPR1=led_tab[led1_cmp];
   EvaRegs.CMPR2=led_tab[led2_cmp];
   EvaRegs.CMPR3=led_tab[led3_cmp];
   EvbRegs.T3CMPR=led_tab[led4_cmp];
   EvbRegs.CMPR4=led_tab[led5_cmp];
   EvbRegs.CMPR5=led_tab[led6_cmp];
   EvbRegs.CMPR6=led_tab[led7_cmp];
  }

  EvaRegs.EVAIFRA.all=0x80;//д1�����־λ
  EvbRegs.EVBIFRA.all=0x80;//д1�����־λ
  PieCtrlRegs.PIEACK.all= 2;
}
interrupt void evb_T3PINT_isr(){
    EvbRegs.EVBIFRA.all=0x80;//д1�����־λ
    PieCtrlRegs.PIEACK.all= 8;
}
interrupt void eva_t1cmp_isr()
{
    if(led_tab[led0_cmp]!=0){
        led=led^0x0101;//����λ���з�ת
        ledout(led);//���
        EvaRegs.EVAIFRA.all= 0x0100;
         PieCtrlRegs.PIEACK.all = 2;//�ж�λ�ڵڶ���
    }
}
interrupt void eva_CMP1INT_isr()//�Ƚϲ���1ִ�еĲ���
{//int i;
  if(led_tab[led1_cmp]!=0)//�Ƚ�ֵΪ0����Ѱ�
    {
/*      GpioDataRegs.GPFTOGGLE.all=0x0900;  //0000 1001 0000 0000��ת��Ӧ��λ
     GpioDataRegs.GPATOGGLE.all=0x04;   //0000 0000 0000 0100

     GpioDataRegs.GPBTOGGLE.all=0x0900;
     GpioDataRegs.GPEDAT.all=0xFFFA;
     for(i=0;i<100;i++){}
     GpioDataRegs.GPEDAT.all=0xFFFF;*/

      led=led^0x0202;}
  ledout(led);//���
  EvaRegs.EVAIFRA.all= 2;
 PieCtrlRegs.PIEACK.all = 2;
}
interrupt void eva_CMP2INT_isr()
{//int i;
  if(led_tab[led2_cmp]!=0)
    {
     // GpioDataRegs.GPFTOGGLE.all=0x1200;  //0001 0010 0000 0000

     //GpioDataRegs.GPBTOGGLE.all=0x1200;
     //GpioDataRegs.GPEDAT.all=0xFFFA;
    // for(i=0;i<100;i++){}
     //GpioDataRegs.GPEDAT.all=0xFFFF;
      led=led^0x0404;
    }
  ledout(led);//���
  EvaRegs.EVAIFRA.all= 4;
 PieCtrlRegs.PIEACK .all= 2;
}
interrupt void eva_CMP3INT_isr()
{//int i;
  if(led_tab[led3_cmp]!=0)
    {
      //GpioDataRegs.GPFTOGGLE.all=0x2400;  //0010 0100 0000 0000
      //GpioDataRegs.GPATOGGLE.all=0x10;    //0000 0000 0001 0000

      //GpioDataRegs.GPBTOGGLE.all=0x2400;
      //GpioDataRegs.GPEDAT.all=0xFFFA;
     //for(i=0;i<100;i++){}
     //GpioDataRegs.GPEDAT.all=0xFFFF;
      led=led^0x0808;
    }
  ledout(led);//���
  EvaRegs.EVAIFRA.all= 8;
 PieCtrlRegs.PIEACK.all = 2;
}

interrupt void evb_t3cmp_isr()
{
    if(led_tab[led4_cmp]!=0){
        led=led^0x1010;//����λ���з�ת
    }
    ledout(led);//���
        EvbRegs.EVBIFRA.all= 0x0100;
         PieCtrlRegs.PIEACK.all = 8;

}
interrupt void evb_CMP4INT_isr()//�Ƚϲ���1ִ�еĲ���
{//int i;
  if(led_tab[led5_cmp]!=0)
    {
/*      GpioDataRegs.GPFTOGGLE.all=0x0900;  //0000 1001 0000 0000��ת��Ӧ��λ
     GpioDataRegs.GPATOGGLE.all=0x04;   //0000 0000 0000 0100

     GpioDataRegs.GPBTOGGLE.all=0x0900;
     GpioDataRegs.GPEDAT.all=0xFFFA;
     for(i=0;i<100;i++){}
     GpioDataRegs.GPEDAT.all=0xFFFF;
    */
      led=led^0x2020;}
  ledout(led);//���
  EvbRegs.EVBIFRA.all= 2;
 PieCtrlRegs.PIEACK.all = 8;
}
interrupt void evb_CMP5INT_isr()
{//int i;
  if(led_tab[led6_cmp]!=0)
    {
     // GpioDataRegs.GPFTOGGLE.all=0x1200;  //0001 0010 0000 0000

     //GpioDataRegs.GPBTOGGLE.all=0x1200;
     //GpioDataRegs.GPEDAT.all=0xFFFA;
    // for(i=0;i<100;i++){}
     //GpioDataRegs.GPEDAT.all=0xFFFF;
      led=led^0x4040;
    }
  ledout(led);//���
  EvbRegs.EVBIFRA.all= 4;
 PieCtrlRegs.PIEACK .all= 8;
}
interrupt void evb_CMP6INT_isr()
{//int i;
  if(led_tab[led7_cmp]!=0)
    {
      //GpioDataRegs.GPFTOGGLE.all=0x2400;  //0010 0100 0000 0000
      //GpioDataRegs.GPATOGGLE.all=0x10;    //0000 0000 0001 0000

      //GpioDataRegs.GPBTOGGLE.all=0x2400;
      //GpioDataRegs.GPEDAT.all=0xFFFA;
     //for(i=0;i<100;i++){}
     //GpioDataRegs.GPEDAT.all=0xFFFF;
      led=led^0x8080;
    }
  ledout(led);//���
  EvbRegs.EVBIFRA.all= 8;
 PieCtrlRegs.PIEACK.all = 8;
}

