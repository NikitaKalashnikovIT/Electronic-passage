// трипод Т83М1

#include "stm32f10x_conf.h"
#include "_Timers.h"
#include "AS5043.h"
#include "define.h"
#include "default.h"
#include "keyboard.h"
#include "messages.h"
#include "turnstile.h"
#include "flash.h"
#include "periphery.h"
#include "_usart.h"
#include "Command_Processor.h"
#include "reader.h"
#include "stm32f10x.h"
//#include "motor.h"
//

unsigned short last_time_blue, last_time_green, last_time_ports; 
int state=0, state1=0;
char a='A';
//****************************

GPIO_InitTypeDef GPIO_InitStructure;
Sensor_InitTypeDef Sensor1_InitStructure;
Turnstile_InitTypeDef Turnstile_InitStruct;
EEPROM_InitTypeDef EEPROM_InitStruct;
dsm_InitTypeDef dsm_InitStruct;
Turnstile_InitTypeDef Turnstile_InitStruct;
int main()
{


		// включаем порты PB3, PB4
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE ;
	

	//конфигурация портов ввода-вывода	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	*/
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	//////////////////////////////////////////////////////////////

	init_sensor(&GPIO_InitStructure);
	InitMessages();
	usart1_init(&GPIO_InitStructure);
	InitTimers();
	//-//init_reader();
	init_motor();
///////////////////////////////////////////////////////////////////
  GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
	

	last_time_green=GetSysTime();



	Turnstile_InitStruct.alarm=0;
	Turnstile_InitStruct.calibr=0x00;
	Turnstile_InitStruct.fire=0;
	Turnstile_InitStruct.warning=0;
	
	//****************************************************
	Flash_Unlock();
	Flash_Read((unsigned char*)&EEPROM_InitStruct, 0x0801FC00, 20);
	if(EEPROM_InitStruct.CRC_eeprom==0xFF)
	{
	EEPROM_InitStruct.addr_unit=0x02;
	EEPROM_InitStruct.time_permit=vTIME_PERMIT;
	EEPROM_InitStruct.time_blocking=vTIME_BLOCKING;
	EEPROM_InitStruct.fi_fp=220;
	EEPROM_InitStruct.fi_zero1=1001;//541;
	EEPROM_InitStruct.fi_zero2=660;//880;
	EEPROM_InitStruct.fi_zero3=319;//199;
	EEPROM_InitStruct.fi_zone1=80;
	EEPROM_InitStruct.fi_zone2=171;
	EEPROM_InitStruct.fi_zone3=310;
	EEPROM_InitStruct.eps_zero=4;
	EEPROM_InitStruct.CRC_eeprom=0xAA;
	Turnstile_InitStruct.warning=WARNING_NO_CALIBR;
	Flash_Erase_Page(0x0801FC00); 
	Flash_Write((unsigned char*)&EEPROM_InitStruct,0x0801FC00,20);		
	}
	//
	EEPROM_InitStruct.fi_zero1=799;//793;//1001;//541;
	EEPROM_InitStruct.fi_zero2=458;//451;//660;//880;
	EEPROM_InitStruct.fi_zero3=118;//110;//319;//199;
  while(1)
  {
		read_sensor(&Sensor1_InitStructure);
		KeyboardFSM();
		FSM_ACS();
		FSM_Fire_Alarm();
		FSM_turnstile(&Turnstile_InitStruct, &EEPROM_InitStruct);
		FSMlocation(&Sensor1_InitStructure, &EEPROM_InitStruct, &Turnstile_InitStruct);
		FSM_permit(&Turnstile_InitStruct);
		FSM_indicators(&Turnstile_InitStruct);
		FSM_blocking(&Turnstile_InitStruct, &EEPROM_InitStruct);
		FSM_speed(&Sensor1_InitStructure, &Turnstile_InitStruct);
		FSM_calibr(&Sensor1_InitStructure, &EEPROM_InitStruct,&Turnstile_InitStruct);
		FSM_Fact_Pass(&Turnstile_InitStruct);
		//FSM_Sound(&Turnstile_InitStruct);
		FSM_Sensor_Damage(&Sensor1_InitStructure, &Turnstile_InitStruct);
		////////////////
		FSM_location_dsm(&Sensor1_InitStructure,&dsm_InitStruct,&EEPROM_InitStruct);
		working_with_zones(&Sensor1_InitStructure,&dsm_InitStruct,&EEPROM_InitStruct,&Turnstile_InitStruct);
		////////////////
		Command_Processor(&Sensor1_InitStructure, &EEPROM_InitStruct, &Turnstile_InitStruct,&dsm_InitStruct );
		// task 2
		ProcessMessages();
  }
}
