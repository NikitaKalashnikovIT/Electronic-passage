#include "stm32f10x_conf.h"

#ifndef __STM32_USART
#define __STM32_USART

#define USART_BUF_RX		24			// размер приемного буфера
#define USART_BUF_TX		36			// размер передающего буфера

#define BAUDRATE				9600		// скорость передачи данных по USART


void usart1_init(GPIO_InitTypeDef* GPIO_InitStruct);
void usart1_send(unsigned char *buf,unsigned char len);
void usart1_recv(unsigned char *buf,unsigned char len);
unsigned char get_rcv_buf(void);
#endif
