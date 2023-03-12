/*=====================================================================
File name	: SPISE2P.H                   
                    
Originator	: Settu Duraisamy
			  C2000 Applications Team
		      Texas Instruments

Description	:  
			  Header file containing  object definitions, proto type 
			  declaration and default object initializers for 
			  SPI Serial EEPROM driver using VSPI
			  
Date		: 30/6/2003 (DD/MM/YYYY)
=======================================================================*/

#ifndef __SPISE2P_H__

#define __SPISE2P_H__ 

#define NULL	0    


#define SIXTEEN_BIT			15
#define EIGHT_BIT			07
 																
/***************************************************************/ 
/* Configurable Parameter for SPI bus Serial EEPROM	           */ 
/***************************************************************/
#define SPISE2P_DATA_WIDTH	EIGHT_BIT 	//数据线宽
#define SPISE2P_ADDR_WIDTH	SIXTEEN_BIT //地址线宽
#define SPIBAUD_REG_VAL		0x63 //SPI波特率 	
#define SPICLK_PHASE		1	//时钟相位为1		  
#define SPICLK_POLARITY		0   //时钟极性为低电平			

/**************************************************************/
/**************************************************************/
 
/* Serial EEPROM Command words, left justified				  */
#define SPISE2P_READ_CMD	0x0300 //读命令
#define SPISE2P_WRITE_CMD	0x0200 //写命令
#define SPISE2P_WRDI_CMD	0x0400 //禁止读命令
#define SPISE2P_WREN_CMD	0x0600 //使能读命令
#define SPISE2P_RDSR_CMD	0x0500 //读状态寄存器 
#define SPISE2P_WRSR_CMD	0x0100 //写状态寄存器
 
#define SPISE2P_DUMMY_DATA	0x0000
#define SPISE2P_BUSY_MASK	0x01

/* Symbolic constant for SPICCR to transfer 8bit or 16 bit value*/
#define SPISE2P_TFR16BIT	0x80|(SPICLK_POLARITY<<6)|SIXTEEN_BIT
#define SPISE2P_TFR8BIT		0x80|(SPICLK_POLARITY<<6)|EIGHT_BIT
  
/* Status valus 												*/
#define SPISE2P_WRRQ		1		/* Write Requset			*/
#define SPISE2P_RDRQ		2		/* Read request				*/  
#define SPISE2P_WRIP		4       /* Write in progress		*/
#define SPISE2P_RDIP		8		/* Read in progress			*/ 

/* Message declaration											*/
typedef struct { 
		unsigned int *dataPtr;		/* Data pointer				*/
		unsigned int nrData;		/* number of data			*/
		unsigned int se2pAddr;		/* se2pAddr					*/
	  	}SE2P_DATA; 


/* Object declaration											*/
//定义类
typedef struct { 
		SE2P_DATA *msgPtr;
		unsigned int csr;			/* control/status register	*/
		void (*init)(void *); 					
	  	void (*tick)(void *);
		void (*csset)(void); 					
	  	void (*csclr)(void);	  	 
	  	}SPISE2P_DRV; 
//对象的初始化  	
#define SPISE2P_DRV_DEFAULTS { NULL,\
		0,\
		(void (*)(void *))SPISE2P_DRV_init,\
	    (void (*)(void *))SPISE2P_DRV_tick,\
	    (void (*)(void))SPISE2P_DRV_csset,\
	    (void (*)(void))SPISE2P_DRV_csclr}  
	    
typedef SPISE2P_DRV *SPISE2P_DRV_handle;
//以类名加"_handle" 表示指针，用typedef实现指针的简洁定义。
void SPISE2P_DRV_init(SPISE2P_DRV * ); 
void SPISE2P_DRV_tick(SPISE2P_DRV *);
void SPISE2P_DRV_csset(void); 
void SPISE2P_DRV_csclr(void);

unsigned int spiSe2pFree(SPISE2P_DRV *se2p);
void spiSe2pWrite(SPISE2P_DRV *se2p,  SE2P_DATA *data);
void spiSe2pRead(SPISE2P_DRV *se2p,  SE2P_DATA *data);

#endif
        
