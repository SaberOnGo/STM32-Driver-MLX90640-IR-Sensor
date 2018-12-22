/****************************************
JinMing electronics
https://www.ebay.com/itm/223274591029
****************************************/

#include "USART.h"
#include "stm32f4xx_hal.h"
#include "MyType.h"


u8 UsartBuff[SEND_BUF_SIZE];//Uart transfer buffer

UART_HandleTypeDef UART1_Handler; //UART Handle
DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA Handle

//Uart1 init  
//bound:BaudRate
void uart_init(u32 bound)
{	
	HAL_UART_DeInit(&UART1_Handler);
	
	//UART init
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //BaudRate
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    
	HAL_UART_Init(&UART1_Handler);					    

	MYDMA_Config(DMA2_Stream7,DMA_CHANNEL_4);//Init DMA
	HAL_DMA_Start(&UART1TxDMA_Handler,(u32)UsartBuff,(u32)&USART1->DR,SEND_BUF_SIZE);	
}


//DMA_Streamx:DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA channel choice,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx)
{ 
    __HAL_RCC_DMA2_CLK_ENABLE();//DMA1 clock enable
  
    __HAL_LINKDMA(&UART1_Handler,hdmatx,UART1TxDMA_Handler);    
    
    //Tx DMA
    UART1TxDMA_Handler.Instance=DMA_Streamx;                            
    UART1TxDMA_Handler.Init.Channel=chx;                                
    UART1TxDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;             
    UART1TxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                 
    UART1TxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                     
    UART1TxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    
    UART1TxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       
    UART1TxDMA_Handler.Init.Mode=DMA_CIRCULAR;                           
    UART1TxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;              
    UART1TxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
    UART1TxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;      
    UART1TxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                 
    UART1TxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;              
    
    HAL_DMA_DeInit(&UART1TxDMA_Handler);  
		
    HAL_DMA_Init(&UART1TxDMA_Handler);
} 

