/*
Библиотека функций работы с считывателем формата wiegand 
	DI8- Data0
	DI9- Data1
*/

#include "define.h"
#include "stm32f10x_exti.h"

#ifndef __STM32_READER
#define __STM32_READER

#define WIEGAND_LEN				26

#define READER_D0					GPIOC, GPIO_Pin_6
#define READER_D1					GPIOC, GPIO_Pin_7

void init_reader(void);
unsigned int Get_Wiegand(void);

#endif
