/*****************************************************************************
* Файл содержит функции системного таймера
* 
*****************************************************************************/

#include "stm32f10x_conf.h"
#include "_Timers.h"
#include "AS5043.h"
unsigned short tick=0;
short *speed;
unsigned short *sensor_angle;
//============================================================================
/* Функция инициализации аппаратного таймера
-----------------------------------------------------------------------------*/
void InitTimers(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_InitStructure;
  TIM_TimeBaseStructInit(&TIM_InitStructure);
  TIM_InitStructure.TIM_Prescaler = 24000;  // 24MHz/24000 = 1kHz
  TIM_InitStructure.TIM_Period = 8; 
  TIM_TimeBaseInit(TIM6, &TIM_InitStructure);  
  TIM_Cmd(TIM6, ENABLE);

  NVIC_EnableIRQ(TIM6_DAC_IRQn);  
  TIM_ITConfig(TIM6, TIM_DIER_UIE, ENABLE);
	
}
//============================================================================
/* Ф-я обработчик аппаратного прерывания таймера
------------------------------------------------------------------------------*/
void TIM6_DAC_IRQHandler(void)
{ //static int state;
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    tick++;
  }
} 
//=============================================================================
/* Ф-я возвращает системный тик времени
-------------------------------------------------------------------------------*/
unsigned short GetSysTime(void)
{
	return tick;
	
}
//===================================================================================================

