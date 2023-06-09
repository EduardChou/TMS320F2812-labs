/***************************************************************************
** 功能描述:采用查询法, 对ADCINB7通道进行采样,显示实际值及对应的10进制数字值
***************************************************************************/
#include "DSP281x_Device.h"

unsigned long k,m;//k用于时钟，m用CPUTIMER0计数
unsigned int keyin=0xffff,key,e;//keyin读键盘值,key记录按下键值
unsigned long AD1,AD2;//AD2为电压模拟值，AD1为转换后数字值的1000倍
unsigned int LedCount;
Uint16 led=0x01;//用于红灯跑马灯
unsigned long timer[6];//数码管显示时分秒
void paoma(void);
void LedOut(Uint16 led);//跑马灯程序中记录一个循环红灯亮的个数
void Delay(Uint16 data);
interrupt void INT_1_7(void);
interrupt void T1PINT_ISR(void);//周期中断服务子函数
interrupt void CMP1INT_ISR(void);
interrupt void CMP2INT_ISR(void);
interrupt void CMP3INT_ISR(void);//三个比较中断子函数

unsigned int LEDCode[30]={0xc000,0xf900,0xA400,0xB000,0x9900,0x9200,0x8200,0xF800,
                           0x8000,0x9000,0x8800,0x8300,0xc600,0xa100,0x8600,0x8e00,
                           0x8c00,0xbf00,0xa700,0xff00,0x4000,0x7900,0x2400,0x3000,
                           0x1900,0x1200,0x0200,0x7800,0x0000,0x1000};//共阳字形码0~f, P，－，L，"灭",0.~9.

#define k0 0xfffe
#define k1 0xfffd
#define k2 0xfffb
#define k3 0xfff7
#define k4 0xffef
#define k5 0xffdf
#define k6 0xffbf
#define k7 0xff7f
#define k8 0xfeff
#define k9 0xfdff
#define k10 0xfbff
#define k11 0xf7ff
#define k12 0xefff
#define k13 0xdfff
#define k14 0xbfff
#define k15 0x7fff


void InitPll(void)
{
    int i;
    EALLOW;
    SysCtrlRegs.PLLCR.bit.DIV=10;//系统时钟150MHZ
    EDIS;
    for(i=0;i<100;i++){;}
}

void InitPeriheralClocks(void)
{
    EALLOW;
    SysCtrlRegs.HISPCP.all=1;//75Mhz
    SysCtrlRegs.LOSPCP.all=2;//37.5Mhz
    SysCtrlRegs.PCLKCR.all=0xffff;//开启部分外设
    EDIS;
}


void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR=0x68;//关闭看门狗
    EDIS;
}

void InitSysCtrl(void)//初始化串行外设接口spi的a组
{
    DisableDog();
    InitPll();
    InitPeriheralClocks();
}

void spi_initial()
{
    SpiaRegs.SPICCR.all=0x0047;
    //0100 0111 SPICCR低4位0111，表示移位序列每个字符长度为8位，即对应一个八段数码管
    //位6为1表示下降沿输出上升沿输入，无数据发送时SPICLK为
    SpiaRegs.SPICTL.all=0x0006;
    //0110 位0为0，禁止spi接受/发送中断 位1为1，使能发送且主控制器SPISTE引脚为低电平
    //位2位1，spi配置为主模式，位3为0，spi时钟相位为正常时钟模式，且时钟边沿有效
    SpiaRegs.SPIBRR=0x007F;
    //spi波特率设置寄存器SPICLK=LSPCLK/(SPIBRR+1)=37.5MHZ/128=0.29*10^6b/s
    SpiaRegs.SPICCR.all=SpiaRegs.SPICCR.all|0x0080;
    //将配置控制寄存器SPICCR位7写1，使其退出复位，准备接受或者发送新的字符
    EALLOW;
    GpioMuxRegs.GPFMUX.all=0x000F;
    //gpiof低四位为1，配置为外设功能引脚（spi）
    EDIS;
}

void InitGpiof(void)
{
    int i;
    EALLOW;
    GpioMuxRegs.GPAMUX.all=0x0000;
    GpioMuxRegs.GPADIR.all=0x0800;
    //GPIOA为通用IO,位11为输出
    GpioMuxRegs.GPBMUX.all=0x0000;
    //GpioMuxRegs.GPBDIR.all=0x00FF;
    //GPIOB各位都为IO
    GpioMuxRegs.GPEMUX.all=0x0000;
    GpioMuxRegs.GPEDIR.all=0xFFFF;
    //GPIOE低三位通用IO,输出
    GpioMuxRegs.GPFMUX.all=0x00ff;
    GpioMuxRegs.GPFDIR.all=0x3f00;
    GpioDataRegs.GPFDAT.all=0x3800;
    //GPIOF高八位IO,位8到13为输出
    GpioMuxRegs.GPBDIR.all=0xffff;
    //GPIOB都为输出
    GpioDataRegs.GPEDAT.all=2;
    GpioDataRegs.GPBDAT.all=0xffff;
    //高八位红灯灭
    for(i=0;i<100;i++){}
    GpioDataRegs.GPEDAT.all=3;
    GpioDataRegs.GPBDAT.all=0xffff;
    //低八位红灯灭
    for(i=0;i<100;i++){}
    EDIS;
}

void Adc_PowerUP(void)//上电
{
    int i;
    AdcRegs.ADCTRL3.bit.ADCBGRFDN=0x3;
    //ADC控制寄存器3的ADCBGRFDN=0x3，AD带隙和参考电路加电
    for(i=0;i<1000;i++){}
    AdcRegs.ADCTRL3.bit.ADCPWDN=1;
    //ADC控制寄存器操作，使得除去带隙和参考电路外的ADC其他模拟电路加电
    for(i=0;i<10000;i++){}
}

void Adc_Init(void)
{
    AdcRegs.ADCTRL1.bit.SEQ_CASC=0;//双序列发生器模式
    AdcRegs.ADCTRL3.bit.SMODE_SEL=0;//顺序采样方式
    AdcRegs.ADCTRL1.bit.CONT_RUN=0;//启动-停止方式
    AdcRegs.ADCTRL1.bit.CPS=1;//用于给HSPCLK分频，内核时钟CPS频率37.5MHZ
    AdcRegs.ADCTRL1.bit.ACQ_PS=0xf;
    AdcRegs.ADCTRL3.bit.ADCCLKPS=0x2;//内核时钟分频，ADCLKPS=HSPCLK/4
    AdcRegs.ADCMAXCONV.all=0x0000;//SEQ1通道数为1
    AdcRegs.ADCCHSELSEQ1.bit.CONV00=0xf;//转换通道为ADCINB7
}

void InitCputimer(void)
{
    EALLOW;
    CpuTimer0Regs.TPR.all=149;
    CpuTimer0Regs.TPRH.all=0;
    CpuTimer0Regs.PRD.all=99999;//CPU周期为0.1s
    CpuTimer0Regs.TCR.all=0xf000;
    EDIS;
}

void InitPIE(void)
{
    EALLOW;
    PieCtrlRegs.PIEIFR1.all=0x0000;//清除中断标志位
    PieCtrlRegs.PIEIER1.all=0x0040;//使能第一组第七个中断，即cputimer0
    PieCtrlRegs.PIEIFR2.all=0x0000;
    PieCtrlRegs.PIEIER2.all=0x000f;
    //使能第一组第1、2、3、4个中断：CMP1INT、CMP2INT、CMP3INT和T1PINT
    PieCtrlRegs.PIECRTL.bit.ENPIE=0x1;
    PieCtrlRegs.PIEACK.all=0x3;
    EDIS;
}

void Init_EV(void)
{
    EvaRegs.EXTCONA.all=1;//设置控制寄存器，独立比较输出模式
    EvaRegs.GPTCONA.all=0x0010;//定时器1比较输出使能
    EvaRegs.T1CON.all=0x17ca;
    //连续增模式，时钟预定标为HSPCLK/64,使能定时器操作，时钟源选择内部时钟（HSPCLK）
    //重载条件为立即重载
    EvaRegs.T1PR=586;//定时器1周期值586；
    EvaRegs.COMCONA.all=0x82e0;//比较器使能，重载条件为计数器T1CNT=0时即下溢中断重载
    //全比较器1、2、3输出使能
    EvaRegs.ACTRA.all=0x1;//比较输出引脚1低有效
    EvaRegs.DBTCONA.all=00;
    EvaRegs.CMPR1=587;
    EvaRegs.CMPR2=587;
    EvaRegs.CMPR3=587;//初始化中断比较值
    EvaRegs.EVAIMRA.all=0x8e;//使能定时器1周期中断和全比较器1到3的比较中断
    EvaRegs.EVAIFRA.all=0xffff;//中断标志置一，允许中断
}

void xianshi(unsigned int d)
{
    int i;
    EALLOW;
    GpioMuxRegs.GPBDIR.all=0xffff;
    for(i=0;i<100;i++){}
    EDIS;
    GpioDataRegs.GPEDAT.all=2;
    GpioDataRegs.GPBDAT.all=d;
    for(i=0;i<100;i++){}
    GpioDataRegs.GPEDAT.all=0xffff;//锁存选通
}

void kongge(void)
{
    SpiaRegs.SPITXBUF=0xbf00;//表示符号-
    while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){};//SPI数据寄存器发送完数据，准备完毕
    SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
}

void shizhong(int i)
{
    SpiaRegs.SPITXBUF=LEDCode[timer[i]];//把timer数组字形码发送给SPI
    while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){};
    SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;//接收
}

void adccv(int i)
{
    SpiaRegs.SPITXBUF=LEDCode[i];//将i的字形码发给数码管，最后这个i是AD2
    while(SpiaRegs.SPISTS.bit.INT_FLAG!=1){};
    SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
}

void keyscan(void)
{
    unsigned int key1,key2,i;
    EALLOW;
    GpioMuxRegs.GPBDIR.all=0x00ff;//高八位配置为输入
    for(i=0;i<100;i++){}
    EDIS;
    GpioDataRegs.GPEDAT.all=0;//GPEDAT低三位为0选通KEY高八位
    for(i=0;i<100;i++)
    key1=GpioDataRegs.GPBDAT.all;
    for(i=0;i<30000;i++)
    if(key1!=GpioDataRegs.GPBDAT.all)key1=0xffff;//消抖

    GpioDataRegs.GPEDAT.all=1;//低三位001，选通低八位
    for(i=0;i<100;i++){}
    key2=GpioDataRegs.GPBDAT.all;
    for(i=0;i<30000;i++)
    if(key2!=GpioDataRegs.GPBDAT.all)key2=0xffff;

    keyin=(key2&0xff00)+key1/256;//最高两位是用不上的，直接清除；
}

void shumaguan(void)//通过全局变量k来给timer数组赋值然后显示到数码管
{
    unsigned long a,b,c,i;//局部变量abc表示秒分时
    if(e==0){//全局变量e=0显示时钟
        c=k/3600;
        b=(k-3600*c)/60;
        a=k%60;
        timer[0]=c/10;
        timer[1]=c%10;
        timer[2]=b/10;
        timer[3]=b%10;
        timer[4]=a/10;
        timer[5]=a%10;
        GpioDataRegs.GPADAT.bit.GPIOA11=0;
        for(i=0;i<2;i++)
        {shizhong(i);}
        kongge();
        for(i=2;i<4;i++)
        {shizhong(i);}
        kongge();
        for(i=4;i<6;i++)
        {shizhong(i);}

        GpioDataRegs.GPADAT.bit.GPIOA11=1;
        for(i=0;i<10;i++){}
    }
    else if(e==1){
        GpioDataRegs.GPADAT.bit.GPIOA11=0;//显示ADC
        adccv((AD2/1000)+20);
        adccv((AD2%1000)/100);
        adccv((AD2%100)/10);
        adccv(17);
        adccv(AD1/1000);
        adccv((AD1%1000)/100);
        adccv((AD1%100)/10);
        adccv(AD1%10);
        GpioDataRegs.GPADAT.bit.GPIOA11=1;
        for(i=0;i<10;i++){}
    }
}

void waitkeyin(void)//将keyin值转换成象征含义键值key
{
    unsigned int temp;
    while(keyin==0xffff){keyscan();}
    temp=keyin;
    while(keyin!=0xffff){keyscan();}
    switch(temp)
    {
    case k0:key=0;break;
    case k1:key=1;break;
    case k2:key=2;break;
    case k3:key=3;break;
    case k4:key=4;break;
    case k5:key=5;break;
    case k6:key=6;break;
    case k7:key=7;break;
    case k8:key=8;break;
    case k9:key=9;break;
    case k10:key=10;break;
    case k11:key=11;break;
    case k12:key=12;break;
    case k13:key=13;break;
    case k14:key=14;break;
    case k15:key=15;break;
    default:key=16;break;
    }
}

void keyfunction2(void)//用于直接改写数码管显示时间
{
    int j;
    CpuTimer0Regs.TCR.all=0xf010;
    for(j=0;j<6;j++)
    {
        waitkeyin();
        timer[j]=key;
        if((timer[0]*10+timer[1])<=23){
            if((timer[2]*10+timer[3])<=59){
                if((timer[4]*10+timer[5])<=59){
                    k=(timer[4]*10+timer[5])+(timer[2]*10+timer[3])*60+(timer[0]*10+timer[1])*3600;
                    shumaguan();
                }
            }
        }
    }
    CpuTimer0Regs.TCR.all=0xf000;
}

void keyfunction1(void)
{
    e=0;//shumaguan函数显示时钟功能而不是ADC
    do{waitkeyin();
    if(key==0)
        {xianshi(0xff00);k=k+36000;if(k>=86400)k=k%86400;shumaguan();}
    if(key==1)
        {xianshi(0xfe00);k=k+3600;if(k>=86400)k=k%86400;shumaguan();}
    if(key==2)
        {xianshi(0xfd00);k=k+600;if(k>=86400)k=k%86400;shumaguan();}
    if(key==3)
        {xianshi(0xfc00);k=k+60;if(k>=86400)k=k%86400;shumaguan();}
    if(key==4)
        {xianshi(0xfa00);k=k+10;if(k>=86400)k=k%86400;shumaguan();}
    if(key==5)
        {xianshi(0xf900);k=k+1;if(k>=86400)k=k%86400;shumaguan();}
    if(key==6)
        {xianshi(0xf800);}
    if(key==7)
            {xianshi(0xf700);}
    }while(key<=7);//键值key0到7时候对应位加一
    if(key==13){xianshi(0xfd00);keyfunction2();}
    if(key==12){paoma();}
}

void keyfunction(void)
{
    waitkeyin();
    switch(key)
    {
    case 13:{xianshi(0xfe00);keyfunction1();break;}
    case 14:{e=1;xianshi(0xfe00);break;}
    case 15:{e=0;xianshi(0xfe00);break;}
    default:break;
    }
}

void paoma(void)
{
    while(key!=15)
    {
        LedOut(led);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        Delay(60000);
        led=led<<1;
        LedCount++;
        if(LedCount>=16)
        {
            LedCount=0;
            led=1;
        }
    }
}

void LedOut(Uint16 led)
{
    Uint16 i;
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


void Delay(Uint16 data)
{
    Uint16 i;
    for(i=0;i<data;i++){;}
}

void main(void)
{
    InitSysCtrl();
    InitGpiof();
    spi_initial();
    Adc_PowerUP();
    Adc_Init();
    InitCputimer();
    Init_EV();
    InitPIE();

    EALLOW;                                  //写EALLOW保护寄存器
    PieVectTable.TINT0 = &INT_1_7;    //PIE中断矢量表
    PieVectTable.T1PINT = &T1PINT_ISR;
    PieVectTable.CMP1INT = &CMP1INT_ISR;
    PieVectTable.CMP2INT = &CMP2INT_ISR;
    PieVectTable.CMP3INT = &CMP3INT_ISR;
    asm("   and IFR, #00H");
    asm("   or  IER, #03H");
    asm("   EINT");
    EDIS;
    k=0;
    m=0;
    e=1;

    for(;;){keyfunction();}
}

interrupt void INT_1_7(void)
{
    m++;
    if(m==10)
    {
        m=0;
        k++;
        if(k==86400) k=0;
    }
    AD1=AdcRegs.ADCRESULT0>>4;
    AD2=AD1*3*1000/4095;
    AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;
    shumaguan();
    EvaRegs.CMPR1=AD2*586/3000;
    EvaRegs.CMPR2=(AD2+1000)*586/5000;
    EvaRegs.CMPR3=(AD2+6000)*586/15000;
    PieCtrlRegs.PIEACK.all=0x1;
    CpuTimer0Regs.TCR.all=0xf000;
}


interrupt void T1PINT_ISR()//周期中断
{
    GpioDataRegs.GPFDAT.all=0x3800;

    EvaRegs.EVAIFRA.all=0x80;
    PieCtrlRegs.PIEACK.all= 2;
}

interrupt void CMP1INT_ISR()//比较中断
{
    if(EvaRegs.CMPR1!=0){
        GpioDataRegs.GPFTOGGLE.all=0x2100;
    }
    EvaRegs.EVAIFRA.all= 2;
    PieCtrlRegs.PIEACK.all = 2;
}

interrupt void CMP2INT_ISR()//比较中断
{
    if(EvaRegs.CMPR2!=0){
        GpioDataRegs.GPFTOGGLE.all=0x1200;
    }
    EvaRegs.EVAIFRA.all= 4;
    PieCtrlRegs.PIEACK.all = 2;
}

interrupt void CMP3INT_ISR()//比较中断
{
    if(EvaRegs.CMPR3!=0){
        GpioDataRegs.GPFTOGGLE.all=0x0c00;
    }
    EvaRegs.EVAIFRA.all= 8;
    PieCtrlRegs.PIEACK.all = 2;
}
