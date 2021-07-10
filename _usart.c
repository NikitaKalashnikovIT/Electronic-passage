#include "_usart.h"

USART_InitTypeDef USART_InitStructure;
unsigned char recv_buf[USART_BUF_RX];
unsigned char send_buf[USART_BUF_TX];
unsigned char rcv_cnt;
//=======================================================================================
void usart1_init(GPIO_InitTypeDef* GPIO_InitStruct)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
//Configure GPIO pin 
	GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct->GPIO_Pin = GPIO_Pin_9;		//	Tx
	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, GPIO_InitStruct);

	GPIO_InitStruct->GPIO_Pin = GPIO_Pin_10;	//	Rx
	GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, GPIO_InitStruct);
	
	
//	Configure UART
  USART_InitStructure.USART_BaudRate = BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig ( USART1 , USART_IT_RXNE , ENABLE );
	USART_Cmd ( USART1 , ENABLE );
	NVIC_EnableIRQ ( USART1_IRQn );
	rcv_cnt=0;
}
//=====================================================================================
/* Обработчик прерывания USART */
void USART1_IRQHandler ()
{
//Проверяем, действительно ли прерывание вызвано приемом нового байта
if ( USART_GetITStatus ( USART1 , USART_IT_RXNE ) != RESET )
{
recv_buf[rcv_cnt]  = USART_ReceiveData ( USART1 );
	rcv_cnt++;
}
}
//=====================================================================================
/* Ф-я передачи данных в USART 
buf - указатель на передающий буфер
len - кол-во передаваемых байт 
*/
void usart1_send(unsigned char *buf,unsigned char len)
{ unsigned char i;
	for(i=0; i<len; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET){}
			USART_SendData(USART1, buf[i]);
	}
	
}
//======================================================================================
/* Ф-я приема данных из USART 
buf - указатель на принимающий буфер
len - кол-во  байт на чтение
*/
void usart1_recv(unsigned char *buf,unsigned char len)
{unsigned char i;
	for(i=0; i<len; i++)
	{
		buf[i]=recv_buf[i];
		rcv_cnt--;
	}
}
//======================================================================================
/* Ф-я возвращает кол-во байт данных в приемном буфере*/
unsigned char get_rcv_buf()
{
	return(rcv_cnt);
}
