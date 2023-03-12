#define 	Set16(addr, val)  	(* (int  *) addr) = (val)
#define 	Get16(addr)       	(* (int  *) addr)		
#define 	Set32(addr, val)  	(* (long *) addr) = (val)
typedef interrupt void(*PINT)(void);
#define 	SetVct(addr, val)  	(* (PINT *) addr) = (val)
//systemclock
#define PLLCR	0x7021
#define PCLKCR	0x701C
#define HISPCP	0x701A
#define LOSPCP	0x701B
#define WDCR	0x7029
#define SCSR	0x7022
#define LPMCR0	0x701E

//GPIOF
#define GPFMUX	0x70D4
#define GPFDIR	0x70D5
#define GPFDAT	0x70F4
#define GPFSET	0x70F5
#define GPFCLEAR	0x70F6
#define GPFTOGGLE	0x70F7

//GPIOB
#define GPBMUX	0x70C4
#define GPBDIR	0x70C5
#define GPBDAT	0x70E4
#define GPBSET	0x70E5
#define GPBCLEAR	0x70E6
#define GPBTOGGLE	0x70E7
//GPIOA
#define GPAMUX	0x70C0
#define GPADIR	0x70C1
#define GPADAT	0x70E0
#define GPASET	0x70E1
#define GPACLEAR	0x70E2
#define GPATOGGLE	0x70E3
//GPIOE
#define GPEMUX	0x70D0
#define GPEDIR	0x70D1
#define GPEDAT	0x70F0
#define GPESET	0x70F1
#define GPECLEAR	0x70F2
#define GPETOGGLE	0x70F3

//timer0
#define TIMER0TIM	0x0C00
#define TIMER0PRD	0x0C02
#define TIMER0TCR	0x0C04
#define TIMER0TPR	0x0C06
#define TIMER0TPRH	0x0C07

//CPU interrupt
extern cregister volatile unsigned int IFR;
extern cregister volatile unsigned int IER;
#define  EINT   asm(" clrc INTM")
#define  DINT   asm(" setc INTM")
#define  EALLOW asm(" EALLOW")
#define  EDIS   asm(" EDIS")

//PIE
#define  PIECTRL	0x0CE0
#define  PIEACK		0x0CE1
#define  PIEIER1	0x0CE2
#define  PIEIFR1	0x0CE3
#define  PIEIER2	0x0CE4
#define  PIEIFR2	0x0CE5

//PIEVector
#define  ENPIEVECT   asm(" SETC VMAP")
#define  PIEVect1_1	 0x000D40        
#define  PIEVect1_7	 (PIEVect1_1+6*2)        
#define  PIEVect2_1	 (PIEVect1_1+16+0*2)        
#define  PIEVect2_2	 (PIEVect1_1+16+1*2)
#define  PIEVect2_3	 (PIEVect1_1+16+2*2)
#define  PIEVect2_4	 (PIEVect1_1+16+3*2)
#define  TINT0Vector	PIEVect1_7
#define  CMP1INTVector	PIEVect2_1
#define  CMP2INTVector	PIEVect2_2
#define  CMP3INTVector	PIEVect2_3
#define  T1PINTVector	PIEVect2_4

//EV
#define  EXTCONA 0x007409 
#define  GPTCONA	0x007400
#define  T1CNT	0x007401
#define  T1CMPR	0x007402
#define  T1PR	0x007403
#define  T1CON	0x007404
#define  COMCONA	0x007411
#define  ACTRA		0x007413
#define  DBTCONA	0x007415
#define  CMPR1	0x007417
#define  CMPR2	0x007418
#define  CMPR3	0x007419
#define  EVAIMRA	0x742C
#define  EVAIFRA	0x742F
