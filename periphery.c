#include "periphery.h"
unsigned char acs_code=0, fire_pin=0;
unsigned short acs_timer,fire_timer;
//==================================================================
void FSM_ACS(void)
{		unsigned char acs_input;
		static unsigned char _acs_input=0, acs_state=0;
		acs_input= (GPIO_ReadInputDataBit(CONTR_ACCESS2)<<1)|
							(GPIO_ReadInputDataBit(CONTR_ACCESS1));
		acs_input= (~acs_input)&0x03;
		switch(acs_state)
		{
			case 0:		// проверить сигнал СКУД?
							if(acs_input!=_acs_input)
							{
								_acs_input=acs_input;
								acs_timer = GetSysTime();
								acs_state=1;
							}
							break;
			case 1:		// выждать время дребезга контактов
						 if((GetSysTime()-acs_timer)>3)
						 {
							acs_state=2; 
						 }
						 break;
			case 2:
						 if(acs_input==_acs_input)
						 {
							 acs_state=2;										// 
							 acs_code=acs_input;
						 }
						 else
						 {
							acs_state=0; 										// если был дребезг вернуться в исходное состояние
							acs_code=acs_input;
						 }
						 break;			
		}
}
//=================================================================================
unsigned char GetACSCode(void)
{
		return(acs_code);
}
//=================================================================================
void FSM_Fire_Alarm(void)
{	unsigned char fire_input;
	static unsigned char _fire_input=0, fire_state=0;
	fire_input= GPIO_ReadInputDataBit(FIRE_ALARM);
	//fire_input= (~fire_input)&0x01;
	switch(fire_state)
		{
			case 0:		// проверить пожарный шлейф
							if(fire_input>0)
							{
								_fire_input=fire_input;
								fire_timer = GetSysTime();
								fire_state=1;
							}
							break;
			case 1:		// выждать время дребезга контактов
						 if((GetSysTime()-fire_timer)>3)
						 {
							fire_state=2; 
						 }
						 break;
			case 2:
						 if(fire_input==_fire_input)
						 {
							 fire_state=2;										// 
							 fire_pin=fire_input;
						 }
						 else
						 {
							fire_state=0; 										// если был дребезг вернуться в исходное состояние
							 fire_pin=fire_input;
						 }
						 break;
						
		}
}
//====================================================================================
unsigned char GetFireState(void)
{
		return(fire_pin);
}
