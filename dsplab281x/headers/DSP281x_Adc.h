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
//ADC控制寄存器1
struct ADCTRL1_BITS {     // bits  description
    Uint16  rsvd1:4;      // 3:0   reserved
    //保留
    Uint16  SEQ_CASC:1;   // 4     Cascaded sequencer mode
    //级联排序器模式
    //该位决定了SEQ1和SEQ2作为两个独立的8状态排序器，还是作为1个16状态
    //排序器（SEQ）工作。
    //0 双排序器模式 SEQ1和SEQ2作为两个独立的8状态排序器工作
    //1 级联模式 SEQ1和SEQ2作为1个16状态排序器工作
    Uint16  SEQ_OVRD:1;   // 5     Sequencer override 
    Uint16  CONT_RUN:1;   // 6     Continuous run
    //连续运行 该位决定了排序器工作在连续运行模式还是开始-停止模式。
    //在一个转换序列有效时，可以对该位进行操作，当该转换序列结束时，
    //该位将会生效。例如，为实现有效的操作，软件可以在EOS产生之前
    //将该位置位或清零。在连续转换模式中不需要复位排序器。然而在
    //开始-停止模式中，排序器必须被复位，以使转换器处于CONV00状态。
    //0 开始-停止模式。产生EOS信号后，排序器停止，除非排序器复位，
    //否则排序器从停止时的状态开始。
    //1 连续转换模式 产生EOS信号后，排序器从CONV00（对于SEQ1和级联排序器）
    //或CONV08（对于SEQ2）状态开始。
    Uint16  CPS:1;        // 7     ADC core clock pre-scalar
    //ADC内核时钟预定标器
    //该预定标器应用来对外设时钟HSPCLK分频
    //0 Fclk=CLK/1
    //1 Fclk=CLK/2
    //注意：CLK=定标后的HSPCLK（ADCCLKPS3~0）
    Uint16  ACQ_PS:4;     // 11:8  Acquisition window size
    //采集窗口大小
    //这些控制SOC的脉宽，同时也决定了采样开关闭和的时间。SOC的
    //脉宽是（ADCTRL[11:8]+1）个ADCLK周期数
    Uint16  SUSMOD:2;     // 13:12 Emulation suspend mode
    //仿真悬挂模式
    //这2位决定了产生仿真挂起时执行的操作（例如，调试器遇到断点）。
    //00 仿真挂起被忽略
    //01 当前排序完成后排序器和其他逻辑停止工作，锁存最终结果，更新状态机。
    //10 当前转换完成后排序器和其他逻辑停止工作，锁存最终结果，更新状态机。
    //11 仿真挂起时，排序器和其他逻辑立即停止。
    Uint16  RESET:1;      // 14    ADC reset
    //ADC复位
    //该位可以使整个ADC模块复位，当芯片的复位引脚被拉低（或一个上电复位）时，
    //所有的寄存器和排序器复位到初始状态。
    //这是一个一次性的影响位，也就是说，它置1后会立即自动清零。
    //读取该位时返回0，ADC的复位信号需要琐存3个时钟周期（即ADC复位后，3个时钟
    //周期内不能改变ADC的控制寄存器）
    //0 没有影响
    //1 复位整个ADC模块（ADC控制逻辑将该位清0）
    //注意：在系统复位期间，ADC模块被复位后。如果在任一时间需要对ADC模块复位，
    //用户可向该位写1。在12个空操作后，用户将需要的配置值写到ADCTR1寄存器：
    //MOV ADCTR1,#01xxxxxxxxxxxxxx;复位ADC模块（RESET=1）
    //RPT 12#
    //NOP ;在ADCTR1寄存器改变配置前必要的延迟
    //NOP
    //MOV ADCTR1,#00xxxxxxxxxxxxxx;配置ADCTR1寄存器
    //注意：如果默认配置满足系统要求，可以不使用第二个MOV改变控制寄存器的配置。
    Uint16  rsvd2:1;      // 15    reserved
    //保留，读返回0。写没有影响。
};


union ADCTRL1_REG {
   Uint16                all;
   struct ADCTRL1_BITS   bit;
};

//ADC控制寄存器2
struct ADCTRL2_BITS {         // bits  description
    Uint16  EVB_SOC_SEQ2:1;   // 0     Event manager B SOC mask for SEQ2
    //SEQ2的事件管理器B的SOC屏蔽位
    //0 EVB的触发信号不能启动SEQ2
    //1 允许事件管理器B触发信号启动SEQ2，可以对事件管理器编程，采用各种事件启动
    //转换。
    Uint16  rsvd1:1;          // 1     reserved
    //保留，读返回0，写没有影响。
    Uint16  INT_MOD_SEQ2:1;   // 2     SEQ2 Interrupt mode
    //SEQ2中断模式
    //该位选择SEQ2的中断模式，在SEQ2转换序列结束时，它影响INT SEQ2的设置。
    //0 每个SEQ2结束时，INT SEQ2置位
    //1 每隔一个SEQ2序列结束时，INT SEQ2置位。
    Uint16  INT_ENA_SEQ2:1;   // 3     SEQ2 Interrupt enable
    //SEQ2中断使能
    //0 禁止INT SEQ2产生的中断
    //1 使能INT SEQ2产生的中断
    Uint16  rsvd2:1;          // 4     reserved
    Uint16  SOC_SEQ2:1;       // 5     Start of conversion for SEQ2
    //SEQ2的触发信号，仅仅适用于双排序器模式;在级联模式中不使用。
    //下列触发可以使该位置位：
    //S/W：软件向该位写1
    //EVB  事件管理器B
    Uint16  RST_SEQ2:1;       // 6     Reset SEQ2
    //复位SEQ2
    //0 无操作
    //1 立即复位SEQ2到初始的"预触发"状态。例如在CONV08状态等待触发，将会
    //退出正在执行的转换序列。
    Uint16  EXT_SOC_SEQ1:1;   // 7     External start of conversion for SEQ1
    //SEQ1的外部触发信号
    //0 无操作
    //1 外部ADCSOC引脚信号启动ADC自动转换序列。
    Uint16  EVA_SOC_SEQ1:1;   // 8     Event manager A SOC mask for SEQ1
    //SEQ1的事件管理器B的SOC屏蔽位
    //0 EVA的触发信号不能启动SEQ21
    //1 允许事件管理器A触发信号启动SEQ1，可以对事件管理器编程，采用各种事件启动
    //转换。
    Uint16  rsvd3:1;          // 9     reserved
    Uint16  INT_MOD_SEQ1:1;   // 10    SEQ1 Interrupt mode
    //SEQ1中断模式
    //该位选择SEQ1的中断模式，在SEQ1转换序列结束时，它影响INT SEQ1的设置。
    //0 每个SEQ1结束时，INT SEQ1置位
    //1 每隔一个SEQ1序列结束时，INT SEQ1置位。
    Uint16  INT_ENA_SEQ1:1;   // 11    SEQ1 Interrupt enable
    //SEQ1中断使能
    //0 禁止INT SEQ1产生的中断
    //1 使能INT SEQ1产生的中断
    Uint16  rsvd4:1;          // 12    reserved
    Uint16  SOC_SEQ1:1;       // 13    Start of conversion trigger for SEQ1
    //SEQ1的触发信号
    //下列触发可以使该位置位：
    //S/W：软件向该位写1
    //EVA  事件管理器A
    //EVB  事件管理器B
    //EXT  外部引脚（例如ADCSOC引脚）
    //当触发源到来时,有3种可能的情况
    //情况1 SEQ1空闲且SOC位清0
    //SEQ1立即开始(仲裁控制).允许任何"挂起"触发请求.
    //情况2 SEQ1忙且SOC位清0
    //该位的置位表示有一个触发请求正被挂起.当完成当前转换,SEQ1
    //重新开始时,该位被清0.
    //情况3 SEQ1忙且SOC位置位
    //在这种情况下任何触发都被忽略(丢失)
    //0 清除一个正在挂起的SOC触发.
    //注意:如果排序器已经启动,该位会自动清除,因而,向该位写0不会起任何作用.
    //例如,用清除该位的方法不能停止一个已经启动的排序
    //1 软件触发 从当前停止的位置启动SEQ1(例如,在空闲模式中)
    //注意:RST SEQ1(ADCTRL2.14)和SOC SEQ1(ADCTRL2.13)位不应用同样的指令设置,
    //这会复位排序器,但不会启动排序器.正确的排序操作是首先设置 RST SEQ1位,
    //然后在下一指令设置SOC SEQ1位.这会保证复位排序器并启动一个新的排序.
    //这种排序也应用于RST SEQ2(ADCTRL2.6)和SOC SQE2(ADCTRL2.5)位
    Uint16  RST_SEQ1:1;       // 14    Restart sequencer 1
    //0 不起作用
    //1 将排序器立即复位到CONV00状态   
    Uint16  EVB_SOC_SEQ:1;    // 15    EVB SOC enable
    //0 不起作用
    //1 该位置位,允许事件管理器B的信号启动级联排序器,可以对事件管理器编程,
    //使用各种事件启动转换.
};


union ADCTRL2_REG {
   Uint16                all;
   struct ADCTRL2_BITS   bit;
};

//ADC自动排序状态寄存器
struct ADCASEQSR_BITS {       // bits   description
    Uint16  SEQ1_STATE:4;     // 3:0    SEQ1 state
    Uint16  SEQ2_STATE:3;     // 6:4    SEQ2 state
    //留给TI调试之用,不提供给用户使用.
    Uint16  rsvd1:1;          // 7      reserved
    Uint16  SEQ_CNTR:4;       // 11:8   Sequencing counter status 
    //排序器计数状态位
    //SEQ1、SEQ2和级联排序器使用SEQ CNTRn4位计数状态位,在级联模式中与SEQ2无关.
    //在转换开始,排序器的计数位SEQCNTR(3~0)初始化在序列MAX CONV中的值.每次自动
    //序列转换完成(或同步采样模式中的一对转换完成)后,排序器计数减1.
    //在递减计数过程中随时可以读取SEQ CNTRn位,检查排序器的状态.读取的值与SEQ1
    //和SEQ2的忙位一起标示了正在执行的排序状态.
    //SEQ CNTRn    等待转换的通道数
    //0000         1或0 取决于busy状态
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

//ADC最大转换通道寄存器
struct ADCMAXCONV_BITS {      // bits  description
    Uint16  MAX_CONV1:4;      // 3:0   Max number of conversions
    Uint16  MAX_CONV2:3;      // 6:4   Max number of conversions
    //MAX CONVn定义了自动转换中最多转换的通道数,该位根据排序器的工作模式
    //变化而变化.
    //对于SEQ1 使用MAX_CONV1_2_0
    //对于SEQ2 使用MAX_CONV2_2_0
    //对于SEQ  使用MAX_CONV1_3_0
    //自动转化序列总是从初始状态开始,一次连续的转换直到结束状态,并将转换
    //结果按顺序装载到结果寄存器中.每个转换序列可以转换1-(MAX CONVn+1)个
    //通道,转换的通道数可以编程.    
    Uint16  rsvd1:9;          // 15:7  reserved 
};

union ADCMAXCONV_REG {
   Uint16                  all;
   struct ADCMAXCONV_BITS  bit;
};

//ADC输入通道选择排序控制寄存器
struct ADCCHSELSEQ1_BITS {    // bits   description
    Uint16  CONV00:4;         // 3:0    Conversion selection 00
    Uint16  CONV01:4;         // 7:4    Conversion selection 01
    Uint16  CONV02:4;         // 11:8   Conversion selection 02
    Uint16  CONV03:4;         // 15:12  Conversion selection 03
    //每1个CONV0n为4位,可编程选择16个通道中的1个.
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
//ADC控制寄存器3
struct ADCTRL3_BITS {         // bits   description
    Uint16   SMODE_SEL:1;     // 0      Sampling mode select
    //采样模式选择
    //0 选择顺序采样模式
    //1 选择同步采样模式
    Uint16   ADCCLKPS:4;      // 4:1    ADC core clock divider
    //ADC的内核时钟分频器
    //0000 ADCLK=HSPCLK/(ADCTRL1[7]+1)
    //0001 ADCLK=HSPCLK/[2x(ADCTRL1[7]+1)]
    //...  ...   ...    ...
    //1111 ADCLK=HSPCLK/[30x(ADCTRL1[7]+1)]
    Uint16   ADCPWDN:1;       // 5      ADC powerdown
    //ADC电源控制
    //0 除带隙和参考电路外的ADC其他模拟电路掉电
    //1 除带隙和参考电路外的ADC其他模拟电路上电
    Uint16   ADCBGRFDN:2;     // 7:6    ADC bandgap/ref power down
    //ADC带隙和参考的电源控制
    //0 带隙和参考电路掉电
    //1 带隙和参考电路上电
    Uint16   rsvd1:8;         // 15:8   reserved
}; 

union  ADCTRL3_REG {
   Uint16                all;
   struct ADCTRL3_BITS   bit;
};

//ADC状态和标志寄存器
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
    //ADC结果寄存器
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
