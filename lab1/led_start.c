#define EALLOW asm(" EALLOW")
#define EDIS   asm(" EDIS")
#define PLLCR	*((volatile unsigned int *) 0x7021) 
#define PCLKCR	*((volatile unsigned int *) 0x701C)
#define HISPCP	*((volatile unsigned int *) 0x701A)
#define LOSPCP	*((volatile unsigned int *) 0x701B) 
#define WDCR	*((volatile unsigned int *) 0x7029)
#define SCSR	*((volatile unsigned int *) 0x7022)
#define GPFMUX	*((volatile unsigned int *) 0x70D4)
#define GPFDIR	*((volatile unsigned int *) 0x70D5)
#define GPFDAT	*((volatile unsigned int *) 0x70F4)

void InitPll(void)
{	int i;
	EALLOW;
	PLLCR = 10;//SYSCLK = 外部振荡频率 * PLLCR /2
	EDIS;
	for(i= 0; i< ( (131072/2)/12 ); i++) {;}
}
void InitPeripheralClocks(void)
{	EALLOW;
	HISPCP = 1;	 // HISPCLK= SYSCLK * HISPCP /2
	LOSPCP = 2;	 // LOSPCLK= SYSCLK * LOSPCP /2
	PCLKCR = 0x909;// 开启部分外设
	EDIS;
}
void DisableDog(void)
{	EALLOW;
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
{	EALLOW;             
 	GPFMUX =0x0000;
 	GPFDIR =0xFF00; 
	EDIS;
}

void main(void)
{	long int i;
	InitSysCtrl();
	InitGpiof();
	for(;;)
	{GPFDAT = 0x5678;
	 for(i= 0; i< 500000; i++) {;}
	 GPFDAT = 0x1234;
	 for(i= 0; i< 500000; i++) {;}
	}
}

