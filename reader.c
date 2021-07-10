#include "reader.h"

EXTI_InitTypeDef exti;
unsigned int wiegand=0, _wiegand=0, wiegand1=0, _wiegand1=0;
unsigned char code_len=0, code_len1=0;
//------------------------------------------------------------------------
/* Ф-я инициалицации прерываний линий считывателя  */
void init_reader(void)
{
	/*
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource6); //выбираем, какой вывод и какого порта будем использовать в качестве источника внешнего прерывания
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource8); //выбираем, какой вывод и какого порта будем использовать в качестве источника внешнего прерывания
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC , GPIO_PinSource9);
	exti.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Falling;		// прерывание по заднему фронту
  exti.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exti);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	*/
}
//=============================================================================
void EXTI9_5_IRQHandler()
{
/*
	if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)	// если прерывание по D0
  {	
		
		code_len++;
		if(code_len==WIEGAND_LEN)
		{
			wiegand=_wiegand>>1;
			_wiegand=0;
		code_len=0;	
		}else
		{
		_wiegand<<=1;	
		}
		
    EXTI_ClearITPendingBit(EXTI_Line6);
  }
	if(EXTI_GetFlagStatus(EXTI_Line7) != RESET) // если прерывание по D1
  {
		
		
		_wiegand |=1;
		code_len++;
		if(code_len==WIEGAND_LEN)
		{
			wiegand=_wiegand>>1;
			_wiegand=0;
		code_len=0;	
		}else
		{
			_wiegand<<=1;
		}
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
	//------------------------------------------------------------------
		if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)	// если прерывание по D0
  {	
		
		code_len1++;
		if(code_len1==WIEGAND_LEN)
		{
			wiegand1=_wiegand1>>1;
			_wiegand1=0;
		code_len1=0;	
		}else
		{
		_wiegand1<<=1;	
		}
		
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
	if(EXTI_GetFlagStatus(EXTI_Line9) != RESET) // если прерывание по D1
  {
		
		
		_wiegand1 |=1;
		code_len1++;
		if(code_len1==WIEGAND_LEN)
		{
			wiegand1=_wiegand1>>1;
			_wiegand1=0;
		code_len1=0;	
		}else
		{
			_wiegand1<<=1;
		}
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
	*/
}
//=================================================================================
/* Ф-я возвращает код карты*/
unsigned int Get_Wiegand(void)
{
	return(wiegand1);
}
