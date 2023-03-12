#include "DSP281x_Device.h"     // DSP281x头文件
#include "DSP281x_GlobalPrototypes.h"// Prototypes for global functions within the  .c files.

void InitGpiof(void)
{   EALLOW;
    GpioMuxRegs.GPFMUX.all =0x0000;
    GpioMuxRegs.GPFDIR.all =0xFF00;
    EDIS;
}

Uint16      Led_Flag = 0;
#define     LED0_OFF
#define     LED0_ON      //点亮DS23
#define     LED1_OFF
#define     LED1_ON      //点亮DS22

interrupt void cpu_timer0_isr(void);//中断声明

Uint16  Timer_10ms = 0;
Uint16  Clock_10ms = 0;
Uint16  Clock_ss = 45;
Uint16  Clock_mm = 59;
Uint16  Clock_hh = 23;
Uint32  main_counting = 0;

Uint16  Flag_TimerNClock = 1;
Uint16  Flag_TimerStartNStop = 1;

Uint16  LEDReg;
Uint16  KeyReg1;
Uint16  KeyReg2;
Uint32  i = 0;
Uint16  led_runner = 1;
Uint16  LedCount = 0;
int     h=0;

#define   K1        0xfeff
#define   K2        0xfdff
#define   K3        0xfbff
#define   K4        0xf7ff
#define   K5        0xefff
#define   K6        0xdfff
#define   K7        0xbfff
#define   K8        0x7fff
#define   K9        0xfeff
#define   K10       0xfdff
#define   K11       0xfbff
#define   K12       0xf7ff
#define   K13       0xefff
#define   K14       0xdfff
#define   K15       0xbfff
#define   K16       0x7fff

Uint16 LEDcode[30]={0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,    //0~F
                    0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,    //
                    0x8c00,0xbf00,0xa700,0xff00,0x4000,0x7900,0x2400,0x3000,
                    0x1900,0x1200,0x0200,0x7800,0x0000,0x1000};
                    //共阳字形码0~f, P，－，L，"灭",0.~9.

//...SPI初始化子程序
void spi_initial()
{
    SpiaRegs.SPICCR.all = 0x0047;                        // 使SPI处于复位方式, 下降沿, 八位数据
    SpiaRegs.SPICTL.all = 0x0006;                        // 主控模式, 一般时钟模式,使能talk, 关闭SPI中断.
    SpiaRegs.SPIBRR     = 0x007F;                        // 配置波特率
    SpiaRegs.SPICCR.all = SpiaRegs.SPICCR.all | 0x0080;  // 退出复位状态

    EALLOW;
        GpioMuxRegs.GPFMUX.all |= 0x000F;   // 设置通用引脚为SPI引脚
    EDIS;
}

//...IO初始化子程序
void gpio_init()
{
    EALLOW;
        //把GPIOA11设置为输出,为74HC595锁存使用
        GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11=0;     //GPIOA11设置为一般I/O口
        GpioMuxRegs.GPADIR.bit.GPIOA11=1;           //

        //将GPIOE0~GPIOE2配置为一般I/O口输出,作138译码
        GpioMuxRegs.GPEMUX.all = GpioMuxRegs.GPEMUX.all & 0xfff8;
        GpioMuxRegs.GPEDIR.all = GpioMuxRegs.GPEDIR.all | 0x0007;

        //将GPIOB8~GPIOB15配置为一般I/O口,D0~D7
        GpioMuxRegs.GPBMUX.all = GpioMuxRegs.GPBMUX.all & 0x00ff;

        //将GPIOF8~GPIOF13配置为一般I/O口，输出
        GpioMuxRegs.GPFMUX.all = 0xc0ff;
        GpioMuxRegs.GPFDIR.all = 0x3f00;

    EDIS;

    GpioDataRegs.GPADAT.bit.GPIOA11 = 0;    //GPIOA11=0; 该端口为74HC595锁存信号
}

//通过SPI向LED发送显示数据
void Write_LED(int LEDReg)
{

        SpiaRegs.SPITXBUF =LEDcode[LEDReg];     //给数码管送数
        while(SpiaRegs.SPISTS.bit.INT_FLAG != 1){}
        SpiaRegs.SPIRXBUF = SpiaRegs.SPIRXBUF;  //读出清标志
}

/*LED显示子程序*/
void LEDdisplay(int LED8,int LED7,int LED6,int LED5,int LED4,int LED3,int LED2,int LED1)
{
    GpioDataRegs.GPADAT.bit.GPIOA11=0; //给LACK信号一个低电平

    Write_LED (LED8);   //向LED最高位写数据,显示LED8
    Write_LED (LED7);
    Write_LED (LED6);
    Write_LED (LED5);
    Write_LED (LED4);
    Write_LED (LED3);
    Write_LED (LED2);
    Write_LED (LED1);   //向LED最低位写数据,显示LED1

    GpioDataRegs.GPADAT.bit.GPIOA11=1; //给LACK信号一个高电平为锁存74HC595
}

//...键扫描子程序K1~K8
int Keyscan1(void)
{
     EALLOW;
    //将GPIOB8~GPIOB15 配置为输入,D0~D7
    GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all & 0x00ff;
     EDIS;

    GpioDataRegs.GPEDAT.all = 0xfff8;    //选通KEY低8位
    for (i=0; i<100; i++){}              //延时

    //判K1~K8是否按下
    if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
    {
        for (i=0; i<30000; i++){}        //延时消抖
        if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
        {
            KeyReg1=GpioDataRegs.GPBDAT.all ; //读键值
            while ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)  //判K1~K8是否松开
            {
                GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
                for (i=0; i<1000; i++){}
            }
            return (1);
        }
    }
    return (0);
}



//...键扫描子程序K9~K16
int Keyscan2(void)
{
     EALLOW;
    //将GPIOB8~GPIOB15配置为输入,D0~D7
    GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all & 0x00ff;
     EDIS;

    GpioDataRegs.GPEDAT.all = 0xfff9;     //选通KEY高8位
    for (i=0; i<100; i++){}               //延时
    //判K8~K16是否按下
    if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
    {
        for (i=0; i<30000; i++){}        //延时消抖
        if ((GpioDataRegs.GPBDAT.all | 0x00ff) != 0xffff)
        {
            KeyReg2 = GpioDataRegs.GPBDAT.all ;//读键值
            while ((GpioDataRegs.GPBDAT.all|0x00ff)!=0xffff) //判K8~K16是否松开
            {
                GpioDataRegs.GPDDAT.bit.GPIOD1 = !GpioDataRegs.GPDDAT.bit.GPIOD1;
                for (i=0; i<1000; i++){}
            }
            return (1);
        }
    }
    return (0);
}

//...键散转子程序K1~K8
void  KeyFunction1(Uint16 KeyReg1)
{
    switch(KeyReg1)
    {
        case  K1: {
                    LEDReg = 0x00;
                  }
                  break;
        case  K2: {
                    LEDReg = 0x01;
                  }
                  break;
        case  K3: {
                    LEDReg = 0x02;
                  }
                  break;
        case  K4: {
                    LEDReg = 0x03;
                  }
                  break;
        case  K5: {
                    LEDReg = 0x04;
                  }
                  break;
        case  K6: {
                    LEDReg = 0x05;
                  }
                  break;
        case  K7: {
                    LEDReg = 0x06;
                  }
                  break;
        case  K8: {
                    LEDReg = 0x07;
                  }
                  break;
        default:
                  break;
    }
}

//...键散转子程序K9~K16
void    KeyFunction2(unsigned int KeyReg2)
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

void LedOut(Uint16 led)
{
    EALLOW;
    //将GPIOB8~GPIOB15配置为输出,D0~D7
    GpioMuxRegs.GPBDIR.all = GpioMuxRegs.GPBDIR.all | 0xff00;
    EDIS;

    GpioDataRegs.GPEDAT.all = 0xfffb;    //LEDB置0
    GpioDataRegs.GPBDAT.all = ~led;
    for (i=0; i<100; i++){}              //延时
    GpioDataRegs.GPEDAT.all = 0xffff;    //锁存高8位
    GpioDataRegs.GPEDAT.all = 0xfffa;    //LEDA置0
    GpioDataRegs.GPBDAT.all = ~(led<<8);
    for (i=0; i<100; i++){}
    GpioDataRegs.GPEDAT.all = 0xffff;    //锁存低8位
}

void Delay(Uint16  data)
{
    Uint16  i;
    for (i=0; i<data ;i++) { ; }
}

interrupt void INT_1_7(void);
void InitPIE(void)
{   EALLOW;
    PieCtrlRegs.PIEIFR1.all=0x0000;
    PieCtrlRegs.PIEIER1.all=0x0040;
    PieCtrlRegs.PIECRTL.bit.ENPIE=0x1;
    PieCtrlRegs.PIEACK.all =0x1;
    EDIS;
}

int k;
void main(void)
{   InitSysCtrl();
    spi_initial();
    gpio_init();
    GpioDataRegs.GPFDAT.all = 0xF600;    //GPIOF8~GPIOF13置高电平,指示灯灭
    Led_Flag=1;  //设置指示灯标志位
    for(i=0;i<5000;i++)
    {
     LEDdisplay( 1,
                 2,
                 3,
                 4,
                 5,
                 17,
                 8,
                 8
             );
     }

    DINT;                /*关CPU中断
    */
    IER = 0x0000;        //关闭外围中断
    IFR = 0x0000;        //清中断标志
    InitPieCtrl();       //初始化PIE控制寄存器 DSP281x_PieCtrl.c
    InitPieVectTable();  //初始化PIE矢量表DSP281x_PieVect.c
    EALLOW;                                  //写EALLOW保护寄存器
    PieVectTable.TINT0 = &cpu_timer0_isr;    //PIE中断矢量表
    EDIS;
    InitCpuTimers();     // For this example, only initialize the Cpu Timers
    ConfigCpuTimer(&CpuTimer0, 150, 10000);
    StartCpuTimer0();
    IER |= M_INT1;   //使能CPU INT1
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; //CPU-Timer0使能位于PIE第1组第7个，将其使能
    EINT;            //开放全局中断

    for(;;)
    {
         main_counting++;
     //check the Key
         if (Keyscan2() == 1)                // 调用键扫描K8~K16子程序
         {
             KeyFunction2(KeyReg2);
             if(LEDReg == 0x0A)
             {
                 Flag_TimerNClock = 1;       // A键，Timer
             }
             else if(LEDReg == 0x0E)
             {
                 Flag_TimerNClock = 0;       // E键，Clock
             }

             else if(LEDReg == 0x08) //Clock_hh +1
             {
                 if(++Clock_hh >= 24)        // 8键，小时+
                 {
                     Clock_hh = 0;
                 }
             }
             else if(LEDReg == 0x0C) //Clock_hh -1
             {
                 if(--Clock_hh >= 24)        // C键，小时-
                 {
                     Clock_hh = 23;
                 }
             }
             else if(LEDReg == 0x09) //Clock_mm +1
             {
                 if(++Clock_mm >= 60)        // 9键，分钟+
                 {
                     Clock_mm = 0;
                 }
             }
             else if(LEDReg == 0x0d) //Clock_mm -1
             {
                 if(--Clock_mm >= 60)        // D键，分钟-
                 {
                     Clock_mm = 59;
                 }
             }
             else if(LEDReg == 0x0B) //Timer Start
             {
                 Flag_TimerStartNStop = 1;       // B键，重启Timer
             }
             else if(LEDReg == 0x0f) //Timer Stop
             {
                 if(Flag_TimerStartNStop == 0)   // F键，暂停Timer；如果已经暂停了，清零Timer
                 {
                     Timer_10ms = 0;             // clear the timer if already stop
                 }
                 else
                 {
                     Flag_TimerStartNStop = 0;
                 }
             }

          }

     // BigLed

         if( (Flag_TimerStartNStop == 1) && (main_counting % 50) == 0)
         {
             led_runner = led_runner << 1;   //led的值左移1位
             if (LedCount++ >= 16)
             {
                 LedCount = 0 ;
                 led_runner = 1;
             }
         }

    //display at LED8
         if(Flag_TimerNClock == 1)       // display in format of Timer in 0.01s
         {
             LEDdisplay(  Timer_10ms/10000000,
                         (Timer_10ms%10000000)/1000000,
                         (Timer_10ms%1000000 )/100000,
                         (Timer_10ms%100000  )/10000,
                         (Timer_10ms%10000   )/1000,
                         (Timer_10ms%1000    )/100 + 20,
                         (Timer_10ms%100     )/10,
                         (Timer_10ms%10      )/1
                     );
         }
         else                            // display in format of Clock
         {
             LEDdisplay( Clock_hh/10,
                         Clock_hh%10,
                         19,
                         Clock_mm/10,
                         Clock_mm%10,
                         19,
                         Clock_ss/10,
                         Clock_ss%10
                         );
         }
         h+=1;
         if(h==3600){
             LedOut(0x0004);
         }
         if(h==7200){LedOut(0x0000);h=0;}

    }
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    if (Flag_TimerStartNStop == 1)
    {
        Timer_10ms++;
    }

    // for clock
    Clock_10ms++;

    if(Clock_10ms % 100 == 0)
    {
        if(++Clock_ss >= 60)
        {
            Clock_ss = 0;
            if(++Clock_mm >= 60)
            {
                Clock_mm = 0;
                if(++Clock_hh >= 24)
                {
                    Clock_hh = 0;
                }
            }
        }
    }

    // for LED at small LED runner
    if(Led_Flag == 1)
    {
        LED0_OFF;
        LED1_ON;    //点亮DS22
        Led_Flag = 0;
    }
    else
    {
        LED0_ON;    //点亮DS23
        LED1_OFF;
        Led_Flag = 1;
    }

     PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;// 清中断应答信号，准备接收下一次中断
}
