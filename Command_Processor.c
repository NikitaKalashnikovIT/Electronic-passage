#include "Command_Processor.h"

Command_InitTypeDef Command_Struct;
unsigned char module_name[6]={'S','T','M','3','2',0};
unsigned char wgnd[4];
unsigned int *wgd_code;

//========================================================================================
// Ф-я командного процессора турникета 
void Command_Processor(Sensor_InitTypeDef *sensor, EEPROM_InitTypeDef *eeprom, Turnstile_InitTypeDef *ts,dsm_InitTypeDef *dsm)
{
	if(get_rcv_buf()>=8)
	{
		usart1_recv((unsigned char*)&Command_Struct,get_rcv_buf());
		//sensor->sensor_data=dsm->location_dsm;
		switch(Command_Struct.command)
		{
			case GET_MODUL_NAME:
				usart1_send((unsigned char*)module_name,50/*6*/);
						break;
			case GET_SENSOR_DATA:
				usart1_send((unsigned char*)sensor,50/*4*/);
						break;
			case GET_TURNSTILE_DEFAULT:
				usart1_send((unsigned char*)eeprom,50/*20*/);
						break;
			case GET_TURNSTILE_STATE:
				usart1_send((unsigned char*)ts,50/*10*/);
						break;			
			case GET_WIEGAND:	// вернуть код карты wgnd[0]-мл.байт 
				wgd_code=(unsigned int*)wgnd;
				*wgd_code=Get_Wiegand();
				usart1_send(wgnd,50/*4*/);
						break;			
			case GET_DSM:
				usart1_send((unsigned char*)dsm,50/*10*/);
					  break;
		}// switch(Command_Struct.command)*/
		
	}// if(get_rcv_buf())
	
}
