//每个PWM周期都修改比较值的方式
//PWM周期为20ms，正弦波周期为2s，每周期100个PWM波形
//第n周期占空比为 y = [sin(0.02 * n * pi) + 1]/2
//PWM周期计算为11718，由此可算比较值
#include "DSP281x_Device.h"
//#include "PF.h"

void Init_Sys(void);    /*初始化系统*/
void Init_IOF(void);    /*初始化IO*/
void Init_timer0(void); /*初始化CPUTIMER*/
void Init_CPUint(void); /*初始化CPU_int*/
void Init_PIEint(void); /*初始化CPU_int*/
void Init_EV(void);     /*初始化EV*/

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
  Init_Sys();   /*初始化系统*/
  for(i=0;i<10000;i++){}
  Init_IOF();       /*初始化IO*/
//  Init_timer0();  /*初始化CPUTIMER*/
  Init_CPUint();    /*初始化CPU_int*/
       /*初始化EV*/
  Init_PIEint();    /*初始化CPU_int*/
  Init_EV();

  PieCtrlRegs.PIEACK.all =10;
  EDIS;
  EINT;

  led0_cmp=0;//初相位设置
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
void Init_Sys(void) /*初始化系统*/
{
  SysCtrlRegs.PLLCR.all=0xA;
  SysCtrlRegs.PCLKCR.all=0xffff;//使能外设时钟
  SysCtrlRegs.HISPCP.all=0x1;   //default默认设置
  SysCtrlRegs.LOSPCP.all=0x2;   //default
  SysCtrlRegs.WDCR=0x68;    //disable w d
  SysCtrlRegs.LPMCR0.all=0x0;   //Low power --Idle

}
void Init_IOF(void)     /*初始化IO*/
{int i;
  GpioMuxRegs.GPFMUX.all=0x00ff;    //f8~f13配置为通用io口
  GpioMuxRegs.GPFDIR.all=0x3f00;//方向设置为输出
  GpioDataRegs.GPFDAT.all=0x0700;//右边三灯先行亮起

  GpioMuxRegs.GPEMUX.all=0x0;   //e口全io
  GpioMuxRegs.GPEDIR.all=0xFF;//全输出
  GpioDataRegs.GPEDAT.all=0xFFFF;//全1

  GpioMuxRegs.GPBMUX.all=0x0;   //b口全io，但只用到高8位
  GpioMuxRegs.GPBDIR.all=0xFF00;//高8位为输出
  GpioDataRegs.GPBDAT.all=0xff00;//高8位全为1，全熄灭

  GpioDataRegs.GPEDAT.all=0xFFFA;//e口最低位1010/////////为什么这里要先行配置e口
  for(i=0;i<100;i++){}
  GpioDataRegs.GPEDAT.all=0xFFFB;//e口最低位1011，锁存
  for(i=0;i<100;i++){}
  GpioDataRegs.GPEDAT.all=0xFFFF;//e口低位1111，锁存

 // GpioMuxRegs.GPAMUX.all=0x1; //仅PWM1 active，因为这里采用中断的方式所以不用pwm波
 // GpioMuxRegs.GPADIR.all=0x3F;    //GPIOA_3~5 1111111低6位输出
 // GpioDataRegs.GPADAT.all=0x00;//给示波器用
}
void Init_timer0(void)  /*初始化CPUTIMER，但这里应该用不到定时器吧*/
{
    CpuTimer0Regs.TPR.all=150;  //25ms产生一次中断
    CpuTimer0Regs.TPRH.all=0;
    CpuTimer0Regs.PRD.all=(long)(1000*25);
    CpuTimer0Regs.TCR.all=0xf000;
}
void Init_CPUint(void)  /*初始化CPU_int*/
{
  IFR=0x0;//清除所有中断标志位
  IER=0X000f;//对最后四组中断使能
}
void Init_PIEint(void)  /*初始化CPU_int*/
{
    PieCtrlRegs.PIEIFR1.all=0;
    PieCtrlRegs.PIEIER2.all=0x1f;//2组中定时器周期中断和比较中断以及比较器比较中断使能，最低5位
    PieCtrlRegs.PIEIFR2.all=0;
    PieCtrlRegs.PIEIER4.all=0x1f;//4组中定时器比较中断以及比较器比较中断使能
    PieCtrlRegs.PIEIFR4.all=0;
    PieCtrlRegs.PIEACK.all =0;
    PieCtrlRegs.PIECRTL.all=1;

  PieVectTable.T1CINT= & eva_t1cmp_isr;//定时器1比较中断
  PieVectTable.CMP1INT= & eva_CMP1INT_isr;//取出所定义的实际中断函数的地址
  PieVectTable.CMP2INT= & eva_CMP2INT_isr;
  PieVectTable.CMP3INT= & eva_CMP3INT_isr;
  PieVectTable.T1PINT=  & eva_T1PINT_isr;//定时器1周期中断
  PieVectTable.T3PINT=  & evb_T3PINT_isr;
  PieVectTable.T3CINT= & evb_t3cmp_isr;//定时器3周期中断
  PieVectTable.CMP4INT= & evb_CMP4INT_isr;//取出所定义的实际中断函数的地址
  PieVectTable.CMP5INT= & evb_CMP5INT_isr;
  PieVectTable.CMP6INT= & evb_CMP6INT_isr;

}
void Init_EV(void)      /*初始化EV*/
{
    EvaRegs.EXTCONA.all=1;//设置为独立运行
    EvaRegs.GPTCONA.all=0x0010;//定时器1比较输出使能,比较输出极性为强制低,因为用不到pwm波
    EvaRegs.T1CON.all=0x17ca;//0001 0111 1100 1010连续增计数、预定标因子为/128,重载条件为立即重载，避免因为装载时间造成显示不准
    EvaRegs.T1PR   =586;
    EvaRegs.COMCONA.all=0x82e0;//1000 0010 1110 0000使能比较操作，使能比较器输出，对比较器1/2/3输出使能
    EvaRegs.ACTRA.all=0x1;//比较输出引脚CMP1输出极性设置为低有效,但其实并不用到此引脚
    EvaRegs.DBTCONA.all=0;//不加死区控制
    EvaRegs.T1CMPR=led0_cmp;
    EvaRegs.CMPR1=led1_cmp;
    EvaRegs.CMPR2=led2_cmp;
    EvaRegs.CMPR3=led3_cmp;
    EvaRegs.EVAIMRA.all=0x018e;//0000 0001 1000 1110定时器1比较和周期中断使能，比较单元1/2/3比较中断使能
    EvaRegs.EVAIFRA.all=0;//

    EvbRegs.EXTCONB.all=1;//设置为独立运行
    EvbRegs.GPTCONB.all=0x0010;//定时器3比较输出使能,比较输出极性为强制低,因为用不到pwm波
    EvbRegs.T3CON.all=0x17ca;//0001 0111 1100 1010连续增计数、预定标因子为/128,重载条件为立即重载，避免因为装载时间造成显示不准
    EvbRegs.T3PR   =586;
    EvbRegs.COMCONB.all=0x82e0;//1000 0010 1110 0000使能比较操作，使能比较器输出，对比较器1/2/3输出使能
    EvbRegs.ACTRB.all=0x1;//比较输出引脚CMP1输出极性设置为低有效,但其实并不用到此引脚
    EvbRegs.DBTCONB.all=0;//不加死区控制
    EvbRegs.T3CMPR=led4_cmp;
    EvbRegs.CMPR4=led5_cmp;
    EvbRegs.CMPR5=led6_cmp;
    EvbRegs.CMPR6=led7_cmp;
    EvbRegs.EVBIMRA.all=0x018e;//0000 0001 1000 1110定时器1比较和周期中断使能，比较单元1/2/3比较中断使能
    EvbRegs.EVBIFRA.all=0;//
}
void ledout(Uint16 led)//点灯程序
{
    //点左边灯
    //led=0x0000;
    int i;
    GpioDataRegs.GPBDAT.all=led;//
    GpioDataRegs.GPEDAT.all=2;
    for(i=0;i<10;i++){}
    GpioDataRegs.GPEDAT.all=7;
    //点右边灯
    GpioDataRegs.GPBDAT.all=led*256;
    GpioDataRegs.GPEDAT.all=3;
    for(i=0;i<10;i++){}
    GpioDataRegs.GPEDAT.all=7;
}

interrupt void CPU_timer0_isr()
{
    PieCtrlRegs.PIEACK.all=1;
}
interrupt void eva_T1PINT_isr()//周期中断1执行的操作,同时，另一部定时器也发生中断
{int i;
led=0x00ff;//高8位必须置为0,低8位必须置1
ledout(led);//点灯
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

  EvaRegs.EVAIFRA.all=0x80;//写1清除标志位
  EvbRegs.EVBIFRA.all=0x80;//写1清除标志位
  PieCtrlRegs.PIEACK.all= 2;
}
interrupt void evb_T3PINT_isr(){
    EvbRegs.EVBIFRA.all=0x80;//写1清除标志位
    PieCtrlRegs.PIEACK.all= 8;
}
interrupt void eva_t1cmp_isr()
{
    if(led_tab[led0_cmp]!=0){
        led=led^0x0101;//对两位进行翻转
        ledout(led);//点灯
        EvaRegs.EVAIFRA.all= 0x0100;
         PieCtrlRegs.PIEACK.all = 2;//中断位于第二组
    }
}
interrupt void eva_CMP1INT_isr()//比较操作1执行的操作
{//int i;
  if(led_tab[led1_cmp]!=0)//比较值为0会很难办
    {
/*      GpioDataRegs.GPFTOGGLE.all=0x0900;  //0000 1001 0000 0000翻转相应两位
     GpioDataRegs.GPATOGGLE.all=0x04;   //0000 0000 0000 0100

     GpioDataRegs.GPBTOGGLE.all=0x0900;
     GpioDataRegs.GPEDAT.all=0xFFFA;
     for(i=0;i<100;i++){}
     GpioDataRegs.GPEDAT.all=0xFFFF;*/

      led=led^0x0202;}
  ledout(led);//点灯
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
  ledout(led);//点灯
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
  ledout(led);//点灯
  EvaRegs.EVAIFRA.all= 8;
 PieCtrlRegs.PIEACK.all = 2;
}

interrupt void evb_t3cmp_isr()
{
    if(led_tab[led4_cmp]!=0){
        led=led^0x1010;//对两位进行翻转
    }
    ledout(led);//点灯
        EvbRegs.EVBIFRA.all= 0x0100;
         PieCtrlRegs.PIEACK.all = 8;

}
interrupt void evb_CMP4INT_isr()//比较操作1执行的操作
{//int i;
  if(led_tab[led5_cmp]!=0)
    {
/*      GpioDataRegs.GPFTOGGLE.all=0x0900;  //0000 1001 0000 0000翻转相应两位
     GpioDataRegs.GPATOGGLE.all=0x04;   //0000 0000 0000 0100

     GpioDataRegs.GPBTOGGLE.all=0x0900;
     GpioDataRegs.GPEDAT.all=0xFFFA;
     for(i=0;i<100;i++){}
     GpioDataRegs.GPEDAT.all=0xFFFF;
    */
      led=led^0x2020;}
  ledout(led);//点灯
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
  ledout(led);//点灯
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
  ledout(led);//点灯
  EvbRegs.EVBIFRA.all= 8;
 PieCtrlRegs.PIEACK.all = 8;
}

