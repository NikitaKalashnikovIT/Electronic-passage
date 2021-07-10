#include "_usart.h"
#include "define.h"
#include "AS5043.h"
#include "reader.h"
#include "motor.h"
#ifndef __STM32_COMMAND
#define __STM32_COMMAND

//Команды USART                             -----------------------------
#define GET_MODUL_NAME					0x01				// получить имя модуля
#define GET_SENSOR_DATA					0x02				// получить данные датчика угла поворота
#define GET_TURNSTILE_DEFAULT		0x03				// получить уставки турникета
#define GET_TURNSTILE_STATE			0x04				// получить состояние турникета
#define GET_WIEGAND							0x05				// получить данные с считывателя
#define GET_DSM									0x06        // получить данные DSM

//-----------------------------------------------------------------------

typedef struct{
	unsigned char header;
	unsigned char command;
	unsigned char data1;
	unsigned char data2;
	unsigned char data3;
	unsigned char data4;
	unsigned char data5;
	unsigned char crc8;
}Command_InitTypeDef;

void Command_Processor(Sensor_InitTypeDef* sensor, EEPROM_InitTypeDef *eeprom, Turnstile_InitTypeDef *ts,dsm_InitTypeDef *dsm);


#endif
