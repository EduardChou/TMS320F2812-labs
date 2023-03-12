//###########################################################################
//
// FILE:   DSP281x_SysCtrl.h
//
// TITLE:  DSP281x Device System Control Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Added bit definitions for the PLLCR register,
//      |             |      | Low power mode LPMCR0 and LPMCR1 registers
//      |             |      | Corrected OTPWAIT bit field name in FOTPWAIT
//      |             |      | Changed SCIENCLKA to SCIAENCLK and SCIENCLKB to
//      |             |      |    SCIBENCLK to match documentation
//      |             |      | Removed bit definition for SCSR register.  This
//      |             |      |    register should only be written to using a
//      |             |      |    mask value else a read-modify-write will clear
//      |             |      |    the WDOVERRIDE bit. 
//###########################################################################

#ifndef DSP281x_SYS_CTRL_H
#define DSP281x_SYS_CTRL_H


#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// System Control Individual Register Bit Definitions:
//ϵͳ���ƼĴ���
// High speed peripheral clock register bit definitions:
//��������ʱ�ӼĴ���
struct HISPCP_BITS  {   // bits  description
   Uint16 HSPCLK:3;     // 2:0   Rate relative to SYSCLKOUT
                        //000 HSPCLK=SYSCLKOUT/1
                        //001 HSPCLK=SYSCLKOUT/2��ϵͳ��λĬ��ֵ)
                        //010 HSPCLK=SYSCLKOUT/4
                        //011 HSPCLK=SYSCLKOUT/6
                        //100 HSPCLK=SYSCLKOUT/8
                        //101 HSPCLK=SYSCLKOUT/10
                        //110 HSPCLK=SYSCLKOUT/12
                        //111 HSPCLK=SYSCLKOUT/14
   Uint16 rsvd1:13;     // 15:3  reserved
};

union HISPCP_REG {
   Uint16              all;
   struct HISPCP_BITS  bit;
};

// Low speed peripheral clock register bit definitions:
//��������ʱ�ӼĴ���
struct LOSPCP_BITS  {   // bits  description
   Uint16 LSPCLK:3;     // 2:0   Rate relative to SYSCLKOUT
                        //000 LSPCLK=SYSCLKOUT/1
                        //001 LSPCLK=SYSCLKOUT/2 
                        //010 LSPCLK=SYSCLKOUT/4 (ϵͳ��λĬ��ֵ)
                        //011 LSPCLK=SYSCLKOUT/8
                        //100 LSPCLK=SYSCLKOUT/10
                        //110 LSPCLK=SYSCLKOUT/12
                        //111 LSPCLK=SYSCLKOUT/14
   Uint16 rsvd1:13;     // 15:3  reserved
};

union LOSPCP_REG {
   Uint16              all;
   struct LOSPCP_BITS  bit;
};

// Peripheral clock control register bit definitions:
//����ʱ�ӿ��ƼĴ���
struct PCLKCR_BITS  {   // bits  description
   Uint16 EVAENCLK:1;   // 0     Enable high speed clk to EV-A
                        //�����λ��1����ʹEVA����ĸ���ʱ�ӣ�HSPCLK��
                        //��Ч�����ڵ͹��Ĳ��������û���λ����0��
   Uint16 EVBENCLK:1;   // 1     Enable high speed clk to EV-B
                        //�����λ��1����ʹEVB����ĸ���ʱ�ӣ�HSPCLK��
                        //��Ч�����ڵ͹��Ĳ��������û���λ����0��
   Uint16 rsvd1:1;      // 2 
   Uint16 ADCENCLK:1;   // 3     Enable high speed clk to ADC
   Uint16 rsvd2:4;      // 7:4   reserved
   Uint16 SPIENCLK:1;   // 8     Enable low speed clk to SPI
   Uint16 rsvd3:1;      // 9     reserved
   Uint16 SCIAENCLK:1;  // 10    Enable low speed clk to SCI-A
   Uint16 SCIBENCLK:1;  // 11    Enable low speed clk to SCI-B
   Uint16 MCBSPENCLK:1; // 12    Enable low speed clk to McBSP
   Uint16 rsvd4:1;      // 13    reserved
   Uint16 ECANENCLK:1;  // 14    Enable system clk to eCAN
};

union PCLKCR_REG {
   Uint16              all;
   struct PCLKCR_BITS  bit;
};   

// PLL control register bit definitions:
//PLL���ƼĴ���
//��λʱ�������/XF_PLLDIS�ĵ�ƽ״̬��
//���Ϊ�ߵ�ƽ��ʹ���ڲ�PLL��
//���Ϊ�͵�ƽ��ʹ��PLL��
//���ʹ��PLL�������PLLCR��ֵȷ��ʱ�ӵĴ�С��
struct PLLCR_BITS {      // bits  description
   Uint16 DIV:4;         // 3:0   Set clock ratio for the PLL
                         //DIV����PLL����·���߲�����·�����Ҳ�����·ʱ������PLL
                         //ʱ�ӵı�����
                         //0000 CLKIN=OSCCLK/2��PLL��·��
                         //0001 CLKIN=��OSCCLK*1��/2
                         //0010 CLKIN=��OSCCLK*2��/2
                         //......
                         //1010 CLKIN=��OSCCLK*10��/2
                         //......
                         //����
   Uint16 rsvd1:12;      // 15:4  reserved
};

union PLLCR_REG {
   Uint16             all;
   struct PLLCR_BITS  bit;
};

// Low Power Mode 0 control register bit definitions:
//�͹���ģʽ0���ƼĴ���
//���ӱ��÷�ʽ����LPMʱ��ѡ��OSCCLKʱ������������֤��ѡ�����������ơ�
struct LPMCR0_BITS {     // bits  description
   Uint16 LPM:2;         // 1:0   Set the low power mode
   Uint16 QUALSTDBY:6;   // 7:2   Qualification   
   Uint16 rsvd1:8;       // 15:8  reserved
};

union LPMCR0_REG {
   Uint16              all;
   struct LPMCR0_BITS  bit;
};

// Low Power Mode 1 control register bit definitions:
//�͹���ģʽ1���ƼĴ���
struct LPMCR1_BITS {     // bits  description
   Uint16 XINT1:1;       // 0     Enable XINT1 to wake the device from standby
   Uint16 XNMI:1;        // 1     Enable XMNI to wake the device from standby
   Uint16 WDINT:1;       // 2     Enable watchdog interrupt to wake the device from standby
   Uint16 T1CTRIP:1;     // 3     Enable T1CTRIP to wake the device from standby
   Uint16 T2CTRIP:1;     // 4     Enable T2CTRIP to wake the device from standby
   Uint16 T3CTRIP:1;     // 5     Enable T3CTRIP to wake the device from standby
   Uint16 T4CTRIP:1;     // 6     Enable T4CTRIP to wake the device from standby
   Uint16 C1TRIP:1;      // 7     Enable C1TRIP to wake the device from standby
   Uint16 C2TRIP:1;      // 8     Enable C2TRIP to wake the device from standby
   Uint16 C3TRIP:1;      // 9     Enable C3TRIP to wake the device from standby
   Uint16 C4TRIP:1;      // 10    Enable C4TRIP to wake the device from standby
   Uint16 C5TRIP:1;      // 11    Enable C5TRIP to wake the device from standby
   Uint16 C6TRIP:1;      // 12    Enable C6TRIP to wake the device from standby
   Uint16 SCIRXA:1;      // 13    Enable SCIRXA to wake the device from standby
   Uint16 SCIRXB:1;      // 14    Enable SCIRXB to wake the device from standby
   Uint16 CANRX:1;       // 15    Enable CANRX to wake the device from standby
};

union LPMCR1_REG {
   Uint16              all;
   struct LPMCR1_BITS  bit;
};

//---------------------------------------------------------------------------
// System Control Register File:
//ϵͳ���ƼĴ����ļ�
struct SYS_CTRL_REGS {
   Uint16  rsvd1[10];            // 0-9
   union   HISPCP_REG HISPCP;    // 10: High-speed peripheral clock pre-scaler
   union   LOSPCP_REG LOSPCP;    // 11: Low-speed peripheral clock pre-scaler
   union   PCLKCR_REG PCLKCR;    // 12: Peripheral clock control register
   Uint16             rsvd2;     // 13: reserved
   union   LPMCR0_REG LPMCR0;    // 14: Low-power mode control register 0
   union   LPMCR1_REG LPMCR1;    // 15: Low-power mode control register 1
   Uint16             rsvd3;     // 16: reserved
   union   PLLCR_REG  PLLCR;     // 17: PLL control register
   // No bit definitions are defined for SCSR because
   // a read-modify-write instruction can clear the WDOVERRIDE bit
   Uint16             SCSR;      // 18: System control and status register
   Uint16             WDCNTR;    // 19: WD counter register
   Uint16             rsvd4;     // 20
   Uint16             WDKEY;     // 21: WD reset key register
   Uint16             rsvd5[3];  // 22-24
   // No bit definitions are defined for WDCR because
   // the proper value must be written to the WDCHK field
   // whenever writing to this register. 
   Uint16             WDCR;      // 25: WD timer control register
   Uint16             rsvd6[6];  // 26-31
};


/* --------------------------------------------------- */
/* CSM Registers  ���밲ȫģ��Ĵ���                                     */
/*                                                     */
/* ----------------------------------------------------*/

/* CSM Status & Control register bit definitions */
//CSM״̬�Ϳ��ƼĴ���
struct  CSMSCR_BITS {      // bit   description
   Uint16     SECURE:1;    // 0     Secure flag
   //��λ��ֻ��λ������ӳ�������İ�ȫ״̬���Ը�λд��Ч��
   Uint16     rsvd1:14;    // 14-1  reserved
   Uint16     FORCESEC:1;  // 15    Force Secure control bit
   //����λ��Ϊ1ʱ�����ḴλKEY�Ĵ�����CSM�ڲ��߼������������������PMF��
   //��ִ�У�ʹ���������ܱ�����оƬ��λ��ʹ���������ܱ���������λ���Ƿ��ء�
}; 

/* Allow access to the bit fields or entire register */
union CSMSCR_REG {
   Uint16             all;
   struct CSMSCR_BITS bit;
};

/* CSM Register File */ 
struct  CSM_REGS {      
   //128λ��Կ�Ĵ������û��ɷ��ʡ�
   Uint16           KEY0;    // KEY reg bits 15-0 
   Uint16           KEY1;    // KEY reg bits 31-16 
   Uint16           KEY2;    // KEY reg bits 47-32
   Uint16           KEY3;    // KEY reg bits 63-48
   Uint16           KEY4;    // KEY reg bits 79-64
   Uint16           KEY5;    // KEY reg bits 95-80
   Uint16           KEY6;    // KEY reg bits 111-96
   Uint16           KEY7;    // KEY reg bits 127-112
   Uint16           rsvd1;   // reserved
   Uint16           rsvd2;   // reserved
   Uint16           rsvd3;   // reserved
   Uint16           rsvd4;   // reserved
   Uint16           rsvd5;   // reserved
   Uint16           rsvd6;   // reserved
   Uint16           rsvd7;   // reserved 
   union CSMSCR_REG CSMSCR;  // CSM Status & Control register
};

/* Password locations */
//������λ�ã��û����á�
struct  CSM_PWL {
   Uint16   PSWD0;  // PSWD bits 15-0
   Uint16   PSWD1;  // PSWD bits 31-16
   Uint16   PSWD2;  // PSWD bits 47-32
   Uint16   PSWD3;  // PSWD bits 63-48
   Uint16   PSWD4;  // PSWD bits 79-64
   Uint16   PSWD5;  // PSWD bits 95-80
   Uint16   PSWD6;  // PSWD bits 111-96
   Uint16   PSWD7;  // PSWD bits 127-112
};



/* Flash Registers */
//Flash�Ĵ���
//Ԥ����
#define FLASH_SLEEP   0x0000;
#define FLASH_STANDBY 0x0001;
#define FLASH_ACTIVE  0x0003;


/* Flash Option Register bit definitions */
//Flashѡ��Ĵ���
struct  FOPT_BITS {       // bit   description
   Uint16     ENPIPE:1;   // 0     Enable Pipeline Mode
   //ʹ����ˮ��ģʽλ������λΪ1ʱ����ˮ��ģʽ���ͨ��Ԥȡָ���ˮ��ģʽ������
   //ȡָ�����ܡ�
   Uint16     rsvd:15;    // 1-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOPT_REG {
   Uint16           all;
   struct FOPT_BITS bit;
};

/* Flash Power Modes Register bit definitions */
//Flash��Դ�Ĵ���
struct  FPWR_BITS {       // bit   description
   Uint16     PWR:2;      // 0-1   Power Mode bits
   //����Ĭ�ϵ�Դģʽλ����Щλ�����˴洢���õ�Ĭ�ϵ�Դģʽ��
   //00 �úʹ洢�����ߣ���͹��ģ�
   //01 �úʹ洢�����
   //10 ��������Ч��
   //11 �úʹ洢�弤���󹦺ģ�
   Uint16     rsvd:14;    // 2-15  reserved
};

/* Allow access to the bit fields or entire register */
union FPWR_REG {
   Uint16           all;
   struct FPWR_BITS bit;
};


/* Flash Status Register bit definitions */
//Flash״̬�Ĵ���
struct  FSTATUS_BITS {       // bit   description
   Uint16     PWRS:2;        // 0-1   Power Mode Status bits
   //��Դģʽ״̬λ����Щ�����������ĵ�ǰ��Դģʽ��
   //00 �úʹ洢�����ߣ���͹��ģ�
   //01 �úʹ洢�����
   //10 ��������Ч��
   //11 �úʹ洢�弤���󹦺ģ�
   Uint16     STDBYWAITS:1;  // 2     Bank/Pump Sleep to Standby Wait Counter Status bits
   //�洢��ͱô����ߵ������ȴ�������״̬λ����λ�����˸����ȴ���������һ�����ʵĶ�ʱ�Ƿ�
   //��ʱ������λΪ1������������ڼ���������λΪ0�����������������
   Uint16     ACTIVEWAITS:1; // 3     Bank/Pump Standby to Active Wait Counter Status bits
   //�洢��ͱôӴ���������ȴ�������״̬λ����λ�����˸����ȴ���������һ�����ʵĶ�ʱ�Ƿ�
   //��ʱ������λΪ1������������ڼ���������λΪ0�����������������
   Uint16     rsvd1:4;       // 4-7   reserved
   Uint16     V3STAT:1;      // 8     VDD3V Status Latch bit
   // VDD3V״̬����λ������λ��1ʱ���������Ա�ģ��� V3STAT�źű�Ϊ�ߵ�ƽ�����źű���3V��Դ
   //����������Χ��ͨ��д1�����λ��д0���ԡ�
   Uint16     rsvd2:7;       // 9-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTATUS_REG {
   Uint16              all;
   struct FSTATUS_BITS bit;
};

/* Flash Sleep to Standby Wait Counter Register bit definitions */
//Flash�����ȴ��Ĵ���
struct  FSTDBYWAIT_BITS {    // bit   description
   Uint16     STDBYWAIT:8;   // 0-7   Bank/Pump Sleep to Standby Wait Count bits
   Uint16     rsvd:8;        // 8-15  reserved
};

/* Allow access to the bit fields or entire register */
union FSTDBYWAIT_REG {
   Uint16                 all;
   struct FSTDBYWAIT_BITS bit;
};

/* Flash Standby to Active Wait Counter Register bit definitions */
//Flash�Ӵ���������ȴ������Ĵ���
struct  FACTIVEWAIT_BITS {   // bit   description
   Uint16     ACTIVEWAIT:8;  // 0-7   Bank/Pump Standby to Active Wait Count bits
   Uint16     rsvd:8;        // 8-15  reserved
};

/* Allow access to the bit fields or entire register */
union FACTIVEWAIT_REG {
   Uint16                  all;
   struct FACTIVEWAIT_BITS bit;
};

/* Bank Read Access Wait State Register bit definitions */
struct  FBANKWAIT_BITS {     // bit   description
   Uint16     RANDWAIT:4;    // 0-3   Flash Random Read Wait State bits
   Uint16     rsvd1:4;       // 4-7   reserved
   Uint16     PAGEWAIT:4;    // 8-11  Flash Paged Read Wait State bits
   Uint16     rsvd2:4;       // 12-15 reserved
};

/* Allow access to the bit fields or entire register */
union FBANKWAIT_REG {
   Uint16                all;
   struct FBANKWAIT_BITS bit;
};

/* OTP Read Access Wait State Register bit definitions */
struct  FOTPWAIT_BITS {      // bit   description
   Uint16     OTPWAIT:5;     // 0-4   OTP Read Wait State bits
   Uint16     rsvd:11;       // 5-15  reserved
};

/* Allow access to the bit fields or entire register */
union FOTPWAIT_REG {
   Uint16               all;
   struct FOTPWAIT_BITS bit;
};


struct FLASH_REGS {
   union FOPT_REG        FOPT;        // Option Register
   Uint16                rsvd1;       // reserved
   union FPWR_REG        FPWR;        // Power Modes Register
   union FSTATUS_REG     FSTATUS;     // Status Register
   union FSTDBYWAIT_REG  FSTDBYWAIT;  // Pump/Bank Sleep to Standby Wait State Register
   union FACTIVEWAIT_REG FACTIVEWAIT; // Pump/Bank Standby to Active Wait State Register
   union FBANKWAIT_REG   FBANKWAIT;   // Bank Read Access Wait State Register
   union FOTPWAIT_REG    FOTPWAIT;    // OTP Read Access Wait State Register
};

//---------------------------------------------------------------------------
// System Control External References & Function Declarations:
//
extern volatile struct SYS_CTRL_REGS SysCtrlRegs;
extern volatile struct CSM_REGS CsmRegs;
extern volatile struct CSM_PWL CsmPwl;
extern volatile struct FLASH_REGS FlashRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP281x_SYS_CTRL_H definition

//===========================================================================
// No more.
//===========================================================================
