/****************************************
JinMing electronics
https://www.ebay.com/itm/223274591029
****************************************/

#ifndef __USART_H
#define __USART_H 

#include "MyType.h"

#define SEND_BUF_SIZE 1544	//Buffer size


extern u8 UsartBuff[SEND_BUF_SIZE];
extern UART_HandleTypeDef UART1_Handler; 

void uart_init(u32 bound);
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx);	


#endif