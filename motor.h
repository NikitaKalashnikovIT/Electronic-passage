#include "stm32f10x_gpio.h"
#include "AS5043.h"

#include "define.h"
#include "messages.h"
#include "keyboard.h"
#include "_Timers.h"
#include "flash.h"
#include "periphery.h"

//dsm-зоны

#define dsm_zone1					8//8//12//70
#define dsm_zone2					30//12//16//115
#define dsm_zone3					130//50//160
#define dsm_zone4					310//310//130//205
#define dsm_zone5					320//310//250
#define dsm_zone6					344//320//295
#define dsm_zone7					345//343//330

typedef struct{
	uint16_t PORTC ;//состояние выходов порта
	uint16_t new_speed ;//0-3 ий бит значение скорости + 4-ый бит направления вращения
	short location_dsm;//зоны скоростей
	short delay_rotor;
	short xxx;
}dsm_InitTypeDef;

	
void init_motor(void);
void FSM_location_dsm(Sensor_InitTypeDef* sensor,dsm_InitTypeDef *dsm,EEPROM_InitTypeDef *eeprom);
void working_with_zones(Sensor_InitTypeDef* sensor,dsm_InitTypeDef *dsm,EEPROM_InitTypeDef *eeprom,Turnstile_InitTypeDef *Turnstile);

