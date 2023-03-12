;//###########################################################################
;//
;// FILE:  DSP281x_usDelay.asm
;//
;// TITLE: Simple delay function
;// 延时程序
;// DESCRIPTION:
;// 描述: 
;// This is a simple delay function that can be used to insert a specified
;// delay into code.  
;// 
;// This function is only accurate if executed from internal zero-waitstate
;// SARAM. If it is executed from waitstate memory then the delay will be
;// longer then specified. 
;// 此程序如果运行在0等待的SARAM中，延时是精确的;如果在运行在等待的存储器中,
;// 则延时的时间要比实际长.
;// To use this function:
;// 如何使用此函数
;//  1 - update the CPU clock speed in the DSP281x_Examples.h
;//    file. For example:
;//    #define CPU_CLOCK_SPEED 6.6667L // for a 150MHz CPU clock speed
;//  1 可以在DSP281x_Examples.h中修改CPU的主频来改变延时的时间.例如:
;//    #define CPU_CLOCK_SPEED 6.6667L //CPU的主频为150MHz
;//  2 - Call this function by using the DELAY_US(A) macro
;//    that is defined in the DSP281x_Examples.h file.  This macro
;//    will convert the number of microseconds specified
;//    into a loop count for use with this function.  
;//    This count will be based on the CPU frequency you specify.
;//  2 用宏 DELAY_US(A) 来调用此延时程序,单位为微秒,延时时间取决于CPU主频.
;//  3 - For the most accurate delay 
;//    - Execute this function in 0 waitstate RAM.  
;//    - Disable interrupts before calling the function
;//      If you do not disable interrupts, then think of
;//      this as an "at least" delay function as the actual
;//      delay may be longer. 
;//  3 使用此延时程序前,需要关掉中断.
;//  The C assembly call from the DELAY_US(time) macro will
;//  look as follows: 
;//  延时程序汇编语言代码
;//  extern void Delay(long LoopCount);                
;//
;//        MOV   AL,#LowLoopCount
;//        MOV   AH,#HighLoopCount
;//        LCR   _Delay
;//
;//  Or as follows (if count is less then 16-bits):
;//
;//        MOV   ACC,#LoopCount
;//        LCR   _Delay
;//
;//
;//###########################################################################
;//
;//  Ver | dd mmm yyyy | Who  | Description of changes
;// =====|=============|======|===============================================
;//  1.00| 11 Sep 2003 | L.H. | No changes since v.58
;//########################################################################### 	

       .def _DSP28x_usDelay
       .sect "ramfuncs"

       .global  __DSP28x_usDelay
_DSP28x_usDelay:
        SUB    ACC,#1
        BF     _DSP28x_usDelay,GEQ    ;; Loop if ACC >= 0
        LRETR 

;There is a 9/10 cycle overhead and each loop
;takes five cycles. The LoopCount is given by
;the following formula:
; DELAY_CPU_CYCLES = 9 + 5*LoopCount
; 根据下面的公式计算LoopCount
; LoopCount = (DELAY_CPU_CYCLES - 9) / 5
; The macro DELAY_US(A) performs this calculation for you
;==================================================
