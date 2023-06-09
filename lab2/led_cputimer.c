#define EALLOW asm(" EALLOW")
#define EDIS   asm(" EDIS")
#define PLLCR   *((volatile unsigned int *) 0x7021)
#define PCLKCR  *((volatile unsigned int *) 0x701C)
#define HISPCP  *((volatile unsigned int *) 0x701A)
#define LOSPCP  *((volatile unsigned int *) 0x701B)
#define WDCR    *((volatile unsigned int *) 0x7029)
#define SCSR    *((volatile unsigned int *) 0x7022)
#define LPMCR0  *((volatile unsigned int *) 0x701E)
#define GPFMUX  *((volatile unsigned int *) 0x70D4)
#define GPFDIR  *((volatile unsigned int *) 0x70D5)
#define GPFDAT  *((volatile unsigned int *) 0x70F4)
#define GPBMUX  *((volatile unsigned int *) 0x70C4)
#define GPBDIR  *((volatile unsigned int *) 0x70C5)
#define GPBDAT  *((volatile unsigned int *) 0x70E4)
#define GPEMUX  *((volatile unsigned int *) 0x70D0)
#define GPEDIR  *((volatile unsigned int *) 0x70D1)
#define GPEDAT  *((volatile unsigned int *) 0x70F0)
#define TIMER0PRD   *((volatile unsigned long int *) 0x0C02)
#define TIMER0TCR   *((volatile unsigned int *) 0x0C04)
#define TIMER0TPR   *((volatile unsigned int *) 0x0C06)
#define TIMER0TPRH  *((volatile unsigned int *) 0x0C07)
#define  PIECTRL    *((volatile unsigned int *) 0x0CE0)
#define  PIEACK     *((volatile unsigned int *) 0x0CE1)
#define  PIEIER1    *((volatile unsigned int *) 0x0CE2)
#define  PIEIFR1    *((volatile unsigned int *) 0x0CE3)


void InitPll(void)
{   int i;
    EALLOW;
    PLLCR = 10;//SYSCLK = 外部振荡频率 * PLLCR /2
    EDIS;
    for(i= 0; i< ( (131072/2)/12 ); i++) {;}
}
void InitPeripheralClocks(void)
{   EALLOW;
    HISPCP = 1;  // HISPCLK= SYSCLK * HISPCP /2
    LOSPCP = 2;  // LOSPCLK= SYSCLK * LOSPCP /2
    PCLKCR = 0x909;// 开启部分外设
    EDIS;
}
void DisableDog(void)
{   EALLOW;
    WDCR = 0x0068; //关闭看门狗；；
    EDIS;
}
void InitSysCtrl(void)
{
    DisableDog();
    InitPll();
    InitPeripheralClocks();
}
void InitGpiof(void)
{   EALLOW;
    GPFMUX =0x0000;
    GPFDIR =0xFF00;
    EDIS;
}

void InitGpiob(void)
{   EALLOW;
    GPBMUX =0x0000;
    GPBDIR =0xFF00;
    EDIS;
}

void InitGpioe(void)
{   EALLOW;
    GPEMUX =0x0000;
    GPEDIR =0x0007;
    EDIS;
}

void InitCputimer(void)
{   EALLOW;
    TIMER0TPR=149;
    TIMER0TPRH=0;
    TIMER0PRD=999;
    TIMER0TCR=0xf000;
    EDIS;
}
void InitPIE(void)
{   EALLOW;
    PIEIFR1=0x0000;
    PIEIER1=0x0040;
    PIECTRL=0x1;
    PIEACK =0x1;
    EDIS;
}

typedef interrupt void(*PINT)(void);
interrupt void INT_1_7(void);
int k;
int i;
int j;
int a,b,c,d,e;
void main(void)
{   InitSysCtrl();
    InitGpiof();
    InitGpiob();
    InitGpioe();
    InitCputimer();
    InitPIE();
    EALLOW;
    GPEDAT=2;
    GPBDAT=0xfeff;
    GPEDAT=7;

    GPEDAT=3;
    GPBDAT=0x7fff;
    GPEDAT=7;
    * (PINT *) 0x0D4C = & INT_1_7;
    asm("  and IFR,#00H");
    asm("  or  IER,#01H");
    asm("  EINT");
    LPMCR0=0x0;
    EDIS;
    k=0;
    a=0;b=0;c=0;d=0;e=0;
    for(;;){asm(" IDLE");};
}

interrupt void INT_1_7(void)
{ k+=1;
  if(k==1000)
    {GPFDAT=GPFDAT&0xfeff;}
   if(k==2000)
    {k=0;GPFDAT=GPFDAT|0x0100;}
  PIEACK=0x1;
  TIMER0TCR=0xf000;

  a+=1;
    if(a==500)
      {GPFDAT=GPFDAT|0x0200;}
     if(a==1000)
      {a=0;GPFDAT=GPFDAT&0xfdff;}
    PIEACK=0x1;
    TIMER0TCR=0xf000;


    b+=1;
      if(b==100)
        {GPFDAT=GPFDAT&0xfbff;}
       if(b==200)
        {b=0;GPFDAT=GPFDAT|0x0400;}
      PIEACK=0x1;
      TIMER0TCR=0xf000;


      c+=1;
        if(c==10)
          {GPFDAT=GPFDAT&0xf7ff;}
         if(c==20)
          {c=0;GPFDAT=GPFDAT|0x0800;}
        PIEACK=0x1;
        TIMER0TCR=0xf000;


        d+=1;
          if(d==5)
            {GPFDAT=GPFDAT&0xefff;}
           if(d==20)
            {d=0;GPFDAT=GPFDAT|0x1000;}
          PIEACK=0x1;
          TIMER0TCR=0xf000;


          e+=1;
          GPFDAT=GPFDAT|0x2000;
          PIEACK=0x1;
          TIMER0TCR=0xf000;
}
