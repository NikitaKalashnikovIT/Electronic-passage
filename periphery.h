/* Файл содержит функции подключаемой периферии */
#include "define.h"
#include "_Timers.h"
#ifndef __STM32_PERIPHERY
#define __STM32_PERIPHERY

//скан-коды СКУД
#define ACS_OPEN1							1
#define ACS_OPEN2							2
#define ACS_OPEN1_OPEN2				3
//------------------------------------------------------------------
void FSM_ACS(void);
void FSM_Fire_Alarm(void);
unsigned char GetACSCode(void);
unsigned char GetFireState(void);
#endif
