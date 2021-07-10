#include "stm32f10x_conf.h"

#ifndef __STM32_DEF
#define __STM32_DEF
//extern GPIO_InitTypeDef GPIO_InitStructure;
//------------------------------------------------------------------------------
#define DI0									GPIOA, GPIO_Pin_8
#define DI1									GPIOA, GPIO_Pin_11
#define DI2									GPIOA, GPIO_Pin_12
#define DI3									GPIOA, GPIO_Pin_15
#define DI4									GPIOB, GPIO_Pin_6
#define DI5									GPIOB, GPIO_Pin_7
#define DI6									GPIOB, GPIO_Pin_8
#define DI7									GPIOB, GPIO_Pin_9

#define DO0									GPIOA, GPIO_Pin_0
#define DO1									GPIOA, GPIO_Pin_1
#define DO2									GPIOA, GPIO_Pin_4
#define DO3									GPIOA, GPIO_Pin_5
#define DO4									GPIOA, GPIO_Pin_6
#define DO5									GPIOA, GPIO_Pin_7
#define DO6									GPIOB, GPIO_Pin_0
#define DO7									GPIOB, GPIO_Pin_1
//
#define vTIME_PERMIT				400									//время прохода одного человека
#define vFI_DIRECTION				4//4
#define vTIME_BLINK					60									// периодичность мерцания светодиодов
#define vTIME_BLOCKING			40									// 
#define vTIME_FACT_PASS			60
// исполнительные механизмы и датчики турникета
#define BUTTON_DIR1					DI0						// кнопка "Направление 1"
#define BUTTON_DIR2					DI2					  // кнопка "Направление 2"
#define BUTTON_CONST				DI1					  // кнопка "Доп."
#define CONTR_ACCESS1				DI4
#define CONTR_ACCESS2				DI3
#define FIRE_ALARM					DI6

#define MAGNET_BLOCKING			DO0
#define DIOD_RED1						DO2
#define DIOD_GREEN1					DO3
#define DIOD_RED2						DO4
#define DIOD_GREEN2					DO5
//
#define BEEP_ON							GPIOC, GPIO_Pin_13
#define FACT_PASS1					GPIOB, GPIO_Pin_3
#define FACT_PASS2					GPIOB, GPIO_Pin_4
#define BUTTON_CALIBR				GPIOD, GPIO_Pin_2
//определение режимов турникета
#define SINGLE_PASS1    0																// однократный проход в направлении 1
#define ACS_PASS1       1																// проход от СКУД в направлении 1
#define CONST_PASS1     2																// групповой проход в направлении 1
#define SINGLE_PASS2    4																// однократный проход в направлении 2
#define ACS_PASS2       5																// проход от СКУД в направлении 2
#define CONST_PASS2     6																// групповой проход в направлении 2
// константы блокировки турникета 
#define PASS1					1
#define PASS2					2
// предупреждающие сообщения
#define WARNING_SENSOR_MAGNET		4					// 3 звуковых сигнала если неправильное магнитное поле датчика
#define WARNING_NO_CALIBR				6					// турникет не откалиброван
// константы задающие цвет индикации
#define RED1								1
#define GREEN1							2
#define RED2								4
#define GREEN2							8
// состояние индикации
#define PERMANENT						1											// состояние постоянного свечения красных(зеленых) диодов
#define BLINK 							2											// состояние мигания

// состояние блокирующего магнита
#define UNBLOCKED 					1											// 
#define BLOCKED							2											// 
// состояние зумера
#define ZUM_MUTE						0											// 
#define ZUM_PERMANENT				1											// 
#define ZUM_ALARM						2											//
#define ZUM_WARNING					3											//

#define vDI15			(GPIOA->IDR & GPIO_Pin_15)
////////
#define DSM_STEP						GPIOC, GPIO_Pin_11							//изменение лог уровня выхода DSM каждые 2 шага   
////////

//--------------------------------------------------------------------------------------
typedef struct{
	unsigned char alarm;
	unsigned char warning;
	unsigned char fire;
	unsigned char turnstile_mode;
	unsigned char blocking;
	unsigned char calibr;
	short speed;
	short location;
	short rrr;
}Turnstile_InitTypeDef;

typedef struct{
	unsigned char addr_unit;
	unsigned char time_blocking;	
	unsigned short time_permit;
	unsigned char eps_zero;
	unsigned char fi_fp;
	unsigned short fi_zero1;
	unsigned short fi_zero2;
	unsigned short fi_zero3;
	unsigned short fi_zone1;
	unsigned short fi_zone2;
	unsigned short fi_zone3;
	unsigned char CRC_eeprom;
}EEPROM_InitTypeDef;
#endif
