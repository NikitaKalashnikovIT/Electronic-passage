#include "stm32f10x_conf.h"
#include "define.h"

#ifndef __STM32_AS5043
#define __STM32_AS5043

typedef struct{
	unsigned short sensor_data;
	unsigned char sensor_status;
	unsigned char fff;
	unsigned char ddd;
}Sensor_InitTypeDef;


void init_sensor(GPIO_InitTypeDef* GPIO_InitStruct);						
void read_sensor(Sensor_InitTypeDef* sensor);

#endif
