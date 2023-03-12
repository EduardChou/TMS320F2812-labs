 #include "DSP281x_Device.h"     // DSP281x头文件

    /*
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
    #define TIMER0PRD   *((volatile unsigned long int *) 0x0C02)
    #define TIMER0TCR   *((volatile unsigned int *) 0x0C04)
    #define TIMER0TPR   *((volatile unsigned int *) 0x0C06)
    #define TIMER0TPRH  *((volatile unsigned int *) 0x0C07)
    #define  PIECTRL    *((volatile unsigned int *) 0x0CE0)
    #define  PIEACK     *((volatile unsigned int *) 0x0CE1)
    #define  PIEIER1    *((volatile unsigned int *) 0x0CE2)
    #define  PIEIFR1    *((volatile unsigned int *) 0x0CE3)
    */
    Uint16 led=0x01;
    void InitPll(void)
    {   int i;
        EALLOW;
        SysCtrlRegs.PLLCR.bit.DIV = 10;//SYSCLK = 外部振荡频率 * PLLCR /2
        EDIS;
        for(i= 0; i< ( (131072/2)/12 ); i++) {;}
    }
    void InitPeripheralClocks(void)
    {   EALLOW;
        SysCtrlRegs.HISPCP.all = 1;  // HISPCLK= SYSCLK * HISPCP /2
        SysCtrlRegs.LOSPCP.all = 2;  // LOSPCLK= SYSCLK * LOSPCP /2
        SysCtrlRegs.PCLKCR.all = 0;// 开启部分外设
        EDIS;
    }
    void DisableDog(void)
    {   EALLOW;
        SysCtrlRegs.WDCR = 0x0068; //关闭看门狗；；
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
        GpioMuxRegs.GPFMUX.all =0x0000;
        GpioMuxRegs.GPFDIR.all =0xFF00;
        EDIS;
    }
    void IOinit() //I/O口初始化
    {
        EALLOW;
         //将GPIOB8~GPIOB15配置为一般I/O口,D0~D7
        GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all&0x00ff;
         //将GPIOB8~GPIOB15配置为输出,D0~D7
        GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all|0xff00;
        //将GPIOE0~GPIOE2配置为一般I/O口输出,作138译码
        GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all&0xfff8;
        GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all|0x0007;
        EDIS;
    }

    void InitCputimer(void)
    {   EALLOW;
        CpuTimer0Regs.TPR.all = 149;
        CpuTimer0Regs.TPRH.all= 0;
        CpuTimer0Regs.PRD.all = 999;
        CpuTimer0Regs.TCR.all =0xf000;
        EDIS;
    }
    //typedef interrupt void(*PINT)(void);
    interrupt void INT_1_7(void);
    void InitPIE(void)
    {   EALLOW;
        PieCtrlRegs.PIEIFR1.all=0x0000;
        PieCtrlRegs.PIEIER1.all=0x0040;
        PieCtrlRegs.PIECRTL.bit.ENPIE=0x1;
        PieCtrlRegs.PIEACK.all =0x1;
        EDIS;
    }
    void LedOut(Uint16 led)
    {
        Uint16  i;
        EALLOW;
        //将GPIOB8~GPIOB15配置为输出,D0~D7
        GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all|0xff00;
        EDIS;
        GpioDataRegs.GPEDAT.all = 0xfffb;    //LEDB选通
        GpioDataRegs.GPBDAT.all = ~led;     //显示高8位
        for (i=0; i<100; i++){}              //延时
        GpioDataRegs.GPEDAT.all = 0xffff;    //锁存高8位
        GpioDataRegs.GPEDAT.all = 0xfffa;    //LEDA选通
        GpioDataRegs.GPBDAT.all = ~(led<<8);//显示低8位
        for (i=0; i<100; i++){}
        GpioDataRegs.GPEDAT.all = 0xffff;    //锁存低8位
    }

    int a,b,c,d,e,f,g,h;
    void main(void)
    {
        InitSysCtrl();
        InitGpiof();
        InitCputimer();
        InitPIE();
        IOinit();
        EALLOW;
        PieVectTable.TINT0= & INT_1_7;//
        asm("  and IFR,#00H");
        asm("  or  IER,#01H");
        asm("  EINT");
        SysCtrlRegs.LPMCR0.all=0x0;
        EDIS;
        a=b=c=d=e=f=g=h=0;
        for(;;){asm(" IDLE");};

    }
    interrupt void INT_1_7(void)
    {a+=1;b+=1;c+=1;d+=1;e+=1;f+=1;g+=1;h+=1;
    if(a==1000)
    {led=led|0x0001;}
    if(a==2000)
    {led=led&0xfffe,a=0;}
    if(b==500)
    {led=led|0x8000;}
    if(b==1000)
    {led=led&0x7fff,b=0;}

    if(c==9)
    {led=led|0x0020;}
    if(c==10)
    {led=led&0xffdf,c=0;}
    if(d==1)
    {led=led|0x0400;}
    if(d==10)
    {led=led&0xfbff,d=0;}
    if(e==8)
    {led=led|0x0040;}
    if(e==10)
    {led=led&0xffbf,e=0;}

    if(f==3)
    {led=led|0x0200;}
    if(f==10)
    {led=led&0xfdff,f=0;}

    if(g==5)
    {led=led|0x0100;}
    if(g==10)
    {led=led&0xfeff,g=0;}

    if(h==7)
    {led=led|0x0080;}
    if(h==10)
    {led=led&0xff7f,h=0;}
      LedOut(led);
      PieCtrlRegs.PIEACK.all =0x1;
      CpuTimer0Regs.TCR.all  =0xf000;
    }

