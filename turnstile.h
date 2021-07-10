/* Файл содержит базовые функции работы турникета */
#include "define.h"
#include "messages.h"
#include "keyboard.h"
#include "_Timers.h"
#include "AS5043.h"
#include "flash.h"
#include "periphery.h"
#ifndef __STM32_TURNSTILE
#define __STM32_TURNSTILE

/*typedef struct{
	unsigned char alarm;
	unsigned char turnstile_mode;
}Turnstile_InitTypeDef;*/

//------------------------------------------------------------------
void FSM_turnstile(Turnstile_InitTypeDef *ts, EEPROM_InitTypeDef *eeprom);
void FSMlocation(Sensor_InitTypeDef* sensor, EEPROM_InitTypeDef *eeprom, Turnstile_InitTypeDef *ts);
void FSM_permit(Turnstile_InitTypeDef *ts);
void FSM_indicators(Turnstile_InitTypeDef *ts);
void FSM_blocking(Turnstile_InitTypeDef *ts, EEPROM_InitTypeDef *eeprom);
void FSM_speed(Sensor_InitTypeDef* sensor, Turnstile_InitTypeDef *ts);
void FSM_calibr(Sensor_InitTypeDef* sensor, EEPROM_InitTypeDef *eeprom, Turnstile_InitTypeDef *ts);
void FSM_Fact_Pass(Turnstile_InitTypeDef *ts);
void FSM_Sound(Turnstile_InitTypeDef *ts);
void FSM_Sensor_Damage(Sensor_InitTypeDef* sensor, Turnstile_InitTypeDef *ts);
#endif
