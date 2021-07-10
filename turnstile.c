#include "turnstile.h"

//=====================================================================================================
short dFi(short fi1, short fi2)
{
	if((fi2-fi1)>512)
	{
		return((fi2-fi1)-1024);
	}
	if((fi2-fi1)<-512)
	{
		return((fi2-fi1)+1024);
	}	
	return(fi2-fi1);
}
//=====================================================================================================
/*
  Ф-я (конечный автомат) режима турникета
  В зависимости от нажатой кнопки пульта, входа СКУД , пожарного входа, устанавливается 
  требуемый режим турникета turnstile_struct->turnstile_mode
*/
void FSM_turnstile(Turnstile_InitTypeDef *ts, EEPROM_InitTypeDef *eeprom)
{
	static unsigned short time_permit;
	//*************
		if(GetFireState())
		{
			ts->fire=1;
			return;
		}
		switch(GetACSCode())
		{
			case ACS_OPEN1:
										ts->turnstile_mode= ts->turnstile_mode | ((1<<ACS_PASS1));
										// сбросить бит ACS_PASS2
										ts->turnstile_mode= ts->turnstile_mode & (~((1<<ACS_PASS2)));
										break;
			case ACS_OPEN2:
										ts->turnstile_mode= ts->turnstile_mode | ((1<<ACS_PASS2));
										// сбросить бит ACS_PASS2
										ts->turnstile_mode= ts->turnstile_mode & (~((1<<ACS_PASS1)));
										break;
			case ACS_OPEN1_OPEN2:
										ts->turnstile_mode= ts->turnstile_mode | ((1<<ACS_PASS1) |
																														(1<<ACS_PASS2));
			default:
										// сбросить биты ACS_PASS1, ACS_PASS2 
										ts->turnstile_mode= ts->turnstile_mode & (~((1<<ACS_PASS1)	 |
																														 (1<<ACS_PASS2)));
		}
	//***************************
	if(GetMessage(MSG_KEY_PRESSED))		// если была нажата клавиша
	{
		switch(GetKeyCode())
		{
			case KEY_OPEN1:	// нажата кнопка "Направление 1"
										if(!(ts->turnstile_mode & ((1<<CONST_PASS1))))
										{
										// инвертировать бит SINGLE_PASS1
										ts->turnstile_mode= ts->turnstile_mode ^ (1<<SINGLE_PASS1);											
										}
										// сбросить биты SINGLE_PASS2, CONST_PASS1, ACS_PASS1
										ts->turnstile_mode= ts->turnstile_mode & (~((1<<SINGLE_PASS2) |
																														 (1<<CONST_PASS1)	 |
																														 (1<<ACS_PASS1)));	
										ts->fire=0;
										time_permit=GetSysTime();
										break;
		  case KEY_CONST_OPEN1:	// нажата кнопка "Направление 1 + Доп"
										// инвертировать бит CONST_PASS1
										ts->turnstile_mode= ts->turnstile_mode ^ (1<<CONST_PASS1);										
										break;
			case KEY_OPEN2:	// нажата кнопка "Направление 2"
										if(!(ts->turnstile_mode & ((1<<CONST_PASS2))))
										{
										// инвертировать бит SINGLE_PASS1
										ts->turnstile_mode= ts->turnstile_mode ^ (1<<SINGLE_PASS2);	
										}
										// сбросить биты SINGLE_PASS1, CONST_PASS2, ACS_PASS2
										ts->turnstile_mode= ts->turnstile_mode & (~((1<<SINGLE_PASS1) |
																														 (1<<CONST_PASS2)	 |
																														 (1<<ACS_PASS2)));
										ts->fire=0;
										time_permit=GetSysTime();
										break;
		  case KEY_CONST_OPEN2:	// нажата кнопка "Направление 2 + Доп"
										// инвертировать бит CONST_PASS2
										ts->turnstile_mode= ts->turnstile_mode ^ (1<<CONST_PASS2);										
										break;			
		} //switch(GetKeyCode())
		
	} //if(GetMessage(MSG_KEY_PRESSED))
	
	if(GetMessage(MSG_FACT_PASS))  // если пересекли угол факта прохода
	{
		// сбросить биты SINGLE_PASS1, SINGLE_PASS2
		ts->turnstile_mode= ts->turnstile_mode &	(~((1<<SINGLE_PASS1) |
																								(1<<SINGLE_PASS2)));	
	}
	// сбросить однократный проход по истечению времени
	if((unsigned short)(GetSysTime()-time_permit)>eeprom->time_permit)
	{
		ts->turnstile_mode= ts->turnstile_mode &	(~((1<<SINGLE_PASS1) |
																								(1<<SINGLE_PASS2)));		
	}
	
}
//====================================================================================================
void FSMlocation(Sensor_InitTypeDef* sensor, EEPROM_InitTypeDef *eeprom, Turnstile_InitTypeDef *ts)
{ static short fi_zero_current, sensor_data_old=0, sensor_data_old1=0;
	short dfi;
	static char fact_pass_zone=0;
	//static unsigned char location=0;
	ts->location &=0x80;
	//set zero
	if((dFi(eeprom->fi_zero1,sensor->sensor_data)<eeprom->eps_zero) && (dFi(eeprom->fi_zero1,sensor->sensor_data)>-eeprom->eps_zero))
	{
		fi_zero_current=eeprom->fi_zero1;
		
		fact_pass_zone=0;
		return;
	}
	if((dFi(eeprom->fi_zero2,sensor->sensor_data)<eeprom->eps_zero) && (dFi(eeprom->fi_zero2,sensor->sensor_data)>-eeprom->eps_zero))
	{
		fi_zero_current=eeprom->fi_zero2;
		fact_pass_zone=0;
		return;
	}	
	if((dFi(eeprom->fi_zero3,sensor->sensor_data)<eeprom->eps_zero) && (dFi(eeprom->fi_zero3,sensor->sensor_data)>-eeprom->eps_zero))
	{
		fi_zero_current=eeprom->fi_zero3;
		
		fact_pass_zone=0;
		return;
	}
	//ts->rrr=sensor_data_old1;
	
	
	
	dfi= dFi(sensor_data_old1,sensor->sensor_data);
	if(dfi>30)
	{ts->rrr=sensor->sensor_status;}

  sensor_data_old1=sensor->sensor_data;	
	//direction
	dfi= dFi(sensor_data_old,sensor->sensor_data);

	if(dfi<-vFI_DIRECTION){ts->location |= 0x80; sensor_data_old= sensor->sensor_data;}
	if(dfi>vFI_DIRECTION) {ts->location &= 0x7F; sensor_data_old= sensor->sensor_data;}
	
	//location
	dfi= dFi(fi_zero_current,sensor->sensor_data);
	if(dfi<0){ts->location |= 0x40; }
	if(dfi>eeprom->fi_fp || dfi<-eeprom->fi_fp) 
		{
			if(!fact_pass_zone) 
			{
				SendMessage(MSG_FACT_PASS);
				fact_pass_zone=1;
			}
		}  // послать сообщение о пересечении факта прохода
	if(dfi>eeprom->fi_zone3 || dfi<-eeprom->fi_zone3) {ts->location |=0x08; return;}
	if(dfi>eeprom->fi_zone2 || dfi<-eeprom->fi_zone2) {ts->location |=0x04; return;}
	if(dfi>eeprom->fi_zone1 || dfi<-eeprom->fi_zone1) {ts->location |=0x02; return;}
	if(dfi>eeprom->eps_zero || dfi<-eeprom->eps_zero) {ts->location |=0x01; return;}
	
		
	ts->location =0x00;
}
//=====================================================================================================
/*
   Ф-я (конечный автомат) организации прохода
	 В зависимости от режима турникета (turnstile_struct->turnstile_mode) определяется
	 разрешен ли проход в заданном направлении (turnstile_struct->blocking)  */
void FSM_permit(Turnstile_InitTypeDef *ts)
{ 
	// блокировка турникета в "направлении 1"
	if((ts->turnstile_mode &((1<<SINGLE_PASS1)|(1<<ACS_PASS1)|(1<<CONST_PASS1))))
	{
		ts->blocking |=PASS1;
		if((ts->turnstile_mode &(1<<CONST_PASS1)) && (ts->turnstile_mode & ((1<<ACS_PASS2)|(1<<SINGLE_PASS2))))
		{
			ts->blocking=ts->blocking & (~(1<<(PASS1-1)));
			//ts->blocking &=0xFD; //0xFE;
		}
	}else{
		ts->blocking=ts->blocking & (~(1<<(PASS1-1)));
		//ts->blocking &=0xFD; //0xFE;
	}
	// блокировка турникета в "направлении 2"
	if((ts->turnstile_mode &((1<<SINGLE_PASS2)|(1<<ACS_PASS2)|(1<<CONST_PASS2))))
	{
		ts->blocking |=PASS2;
		if((ts->turnstile_mode &(1<<CONST_PASS2)) && (ts->turnstile_mode & ((1<<ACS_PASS1)|(1<<SINGLE_PASS1))))
		{
			ts->blocking=ts->blocking & (~(1<<(PASS2-1)));
			//ts->blocking &=0xFE;// 0xFD;
		}
	}else{
		ts->blocking=ts->blocking & (~(1<<(PASS2-1)));
		//ts->blocking &=0xFE; //0xFD;
	}

}
//=======================================================================================================
/*
	Ф-я управления индикацией турникета
	
!!! комбинация переменных blink и led_condition задаёт режим моргания 
1. режим включается/выключается (красный сторона 1)
		blink = 00000011    led_condition = 00000001 
2. режим переключение цвета (красный-зеленый сторона1)
    blink = 00000001    led_condition = 00000011 
*/
void FSM_indicators(Turnstile_InitTypeDef *ts)
{
	static unsigned char blink=0, led_condition=0, led_state=PERMANENT;
	unsigned char led_out=0;
	static unsigned short time_blink;
	switch(led_state)
	{
		case PERMANENT:		// состояние постоянного горения светодиодов
			if(ts->fire)				// если пожар
			{
				led_state = PERMANENT;
				led_out = GREEN1 | GREEN2;
			}else	if(ts->alarm || ts->calibr)		//если тревога или калибровка начать моргать
						{
							led_state = BLINK;
						}else  if(ts->blocking) // если турникет разблокирован
											{
												led_state = PERMANENT;
												if((ts->blocking & PASS1))
													led_out |= GREEN1;
												else
													led_out |= RED1;
												if((ts->blocking & PASS2))
													led_out |= GREEN2;
												else
													led_out |= RED2;
											}else{
											led_out = RED1 | RED2;
											}
					break;
		case BLINK:	// состояние мигания (режим мигания задается переменной blink)
				if((ts->alarm) && (led_condition !=(RED1 | RED2)))
				{
						blink = RED1 | RED2;	// начальное условие
						led_condition = RED1 | RED2;	//моргать двумя красными
						
				}else if(ts->calibr)
								{
								if(((ts->calibr & 0x81)==0x81) && (led_condition !=(RED2 | GREEN1)))
								{
									blink = RED2 | GREEN1;	// начальное условие
									led_condition = RED2 | GREEN1; //моргать зеленым1 и красным2 диодами	
								}
								if(((ts->calibr & 0x81)==0x01) && (led_condition !=(RED1 | RED2)))
								{
									blink = RED1 | RED2;	// начальное условие
									led_condition = RED1 | RED2;	//моргать двумя красными
								}
								
				}else 
				{
					led_state = PERMANENT;
				}	
				led_out = led_condition & blink;
				break;
	}//switch(led_state)
// изменить состояние переменной blink
	if((unsigned short)(GetSysTime()-time_blink)>vTIME_BLINK)
	{
					blink = blink ^ 0x0F;		// инвертировать биты 0,1,2,3
		time_blink=GetSysTime();
	}
// вывести состояние светодиодов на плату
	if((led_out & RED1))	
	 GPIO_WriteBit(DIOD_RED1, Bit_SET);	
	else 
		GPIO_WriteBit(DIOD_RED1, Bit_RESET);
	
	if((led_out & GREEN1))	
	 GPIO_WriteBit(DIOD_GREEN1, Bit_SET);	
	else 
		GPIO_WriteBit(DIOD_GREEN1, Bit_RESET);
	
	if((led_out & RED2))	
	 GPIO_WriteBit(DIOD_RED2, Bit_SET);	
	else 
		GPIO_WriteBit(DIOD_RED2, Bit_RESET);
	
	if((led_out & GREEN2))	
	 GPIO_WriteBit(DIOD_GREEN2, Bit_SET);	
	else 
		GPIO_WriteBit(DIOD_GREEN2, Bit_RESET);
}
//=============================================================================================
/* ф-я (внутренняя) определяет разрешено ли движение штанги турникета
   0- движение разрешено;
	 1- дижение запрещено.
*/	
unsigned char Motion_Allow(Turnstile_InitTypeDef *ts)
{	unsigned char motion_allow=0;
	switch(ts->blocking)
								{
									case 0:	// если нет разрешения на проход
										if((ts->location & 0x03))    // если сдвинули штангу с нулевого положения - заблокировать
												motion_allow=1;
												break;
									case PASS1: // если разрешен проход в "направлении 1"
										if(((ts->location & 0xC0)==0xC0) && (ts->location & 0x07)) // заблокировать если толкают не в направлении1
												motion_allow=1;
												break;
									case PASS2: // если разрешен проход в "направлении 2"
										if(((ts->location & 0xC0)==0x00) && (ts->location & 0x07)) // заблокировать если толкают не в направлении2
												motion_allow=1;
												break;
								}//switch(ts->blocking)	
	return(motion_allow);
}
//=============================================================================================
/*
Ф-я управления электромагнитом

*/
void FSM_blocking(Turnstile_InitTypeDef *ts, EEPROM_InitTypeDef *eeprom)
{
	static unsigned char blocking_state=UNBLOCKED;
	static unsigned short time_blocking;
	switch(blocking_state)
	{
		case UNBLOCKED:  // состояние разблокирован
					if(ts->fire)
					{
						blocking_state=UNBLOCKED;
						break;
					}
					if(ts->alarm)
					{
						blocking_state=BLOCKED;
					}else	if(ts->calibr)
								{ 
									blocking_state=UNBLOCKED;
									break;
									}else if(Motion_Allow(ts))	// если движение штанги неправильное - заблокировать
											{
												blocking_state=BLOCKED;
												time_blocking=GetSysTime();
											}

				break;
		case BLOCKED:
					if(ts->fire)
					{
						blocking_state=UNBLOCKED;
						break;
					}else if(ts->alarm)
								{
									blocking_state=BLOCKED;
									break;						
								}else if(ts->calibr)
											{
												blocking_state=UNBLOCKED;
												break;
											}
				// разблокировать по истечению времени
				if((unsigned short)(GetSysTime()-time_blocking)>eeprom->time_blocking)
				{
					blocking_state=UNBLOCKED;		// разблокировать
				}
			 
			 break;
	}//switch(blocking_state)
	
	if(blocking_state==BLOCKED)	
	 GPIO_WriteBit(MAGNET_BLOCKING, Bit_SET);	
	else 
	 GPIO_WriteBit(MAGNET_BLOCKING, Bit_RESET);	
}
//=================================================================================================
void FSM_speed(Sensor_InitTypeDef* sensor, Turnstile_InitTypeDef *ts)
{ static unsigned short sensor_data_old;
	static unsigned short time_speed=0;
		if((unsigned short)(GetSysTime()-time_speed)>1)
	{
		ts->speed= sensor->sensor_data - sensor_data_old;
		if(ts->speed <0)
			ts->speed= -ts->speed;
		if(ts->speed<3)
			ts->speed=0;
		else
		sensor_data_old= sensor->sensor_data;
		time_speed=GetSysTime();
	}
}
//===============================================================================================
/*
Ф-я калибровки.

*/
void FSM_calibr(Sensor_InitTypeDef* sensor, EEPROM_InitTypeDef *eeprom, Turnstile_InitTypeDef *ts)
{static unsigned char calibr_state=0, n=0;
	static unsigned short time_wait_speed=0, time_wait_flash_write=0;
	switch(calibr_state)
	{
		case 0:
			if(GetMessage(MSG_KEY_PRESSED))			// если нажата кнопка калибровки
					{
						if(GetKeyCode()==KEY_CALIBR)
						calibr_state=1;
						n=0;
					}
					break;
		case 1:															// ожидаем начала вращения штанг
			    ts->calibr=0x81;
					if(ts->speed)	
						calibr_state=2;
					break;
		case 2:														 // если штанги остановились запустить таймер ожидания нулевой скорости
					if(!ts->speed)
					{
						calibr_state=3;
						time_wait_speed = GetSysTime();
					}
					break;
	  case 3:														// проверить убедиться что действительно остановились
					if((unsigned short)(GetSysTime()-time_wait_speed)>200)	
					{
						if(ts->speed)
						{
							calibr_state=2;
							break;
						}
						ts->calibr=0x01;
						time_wait_flash_write= GetSysTime();	
						//******************
						if(n==0)
							eeprom->fi_zero1 = sensor->sensor_data;
						if(n==1)
							eeprom->fi_zero2 = sensor->sensor_data;
						if(n==2)
							eeprom->fi_zero3 = sensor->sensor_data;
						//*****************
						n++;
						calibr_state=4;
					}
					break;
		case 4:												// выждать 2 сек и запустить цикл заново
					if((unsigned short)(GetSysTime()-time_wait_flash_write)>200)
					{
						if(n<3)
							calibr_state=1;
						else
						calibr_state=5;						
					}
					break;
		case 5:							// произвести запись во flash
					Flash_Unlock();
					Flash_Erase_Page(0x0801FC00);
					Flash_Write((unsigned char*)eeprom,0x0801FC00,20);
					ts->calibr=0;
					calibr_state=0;
					break;
	}//switch(calibr_state)
}
//========================================================================================
void FSM_Fact_Pass(Turnstile_InitTypeDef *ts)
{static unsigned short time_fact_pass=0;
	
	if(GetMessage(MSG_FACT_PASS))
	{
		if((ts->location & 0x40)!=0)
			GPIO_WriteBit(FACT_PASS1, Bit_SET);
		else
			GPIO_WriteBit(FACT_PASS2, Bit_SET);
		time_fact_pass= GetSysTime();
	}
	if((unsigned short)(GetSysTime()-time_fact_pass)>vTIME_FACT_PASS)
				{
					GPIO_WriteBit(FACT_PASS1, Bit_RESET);
					GPIO_WriteBit(FACT_PASS2, Bit_RESET);
				}
}
//===========================================================================================
/* Ф-я управления зуммером    */
void FSM_Sound(Turnstile_InitTypeDef *ts)
{
	static unsigned char sound_state=0, mute=0, blink=0,_blink=0, button_const_cnt=0;
	unsigned char sound_out;
	static unsigned short time_sound=0;
	switch(sound_state)
	{
		case ZUM_MUTE:		// 
			if(ts->alarm || ts->calibr)		// если тревога или калибровка включить прерывистый звук
			{
				sound_state=ZUM_ALARM;
				}else if(ts->warning)						// предупреждающее сообщение
							{
								sound_state=ZUM_WARNING;
							}else if(mute)								// если звук отключен пультом
										{
											sound_state=ZUM_MUTE;							
										}else if(Motion_Allow(ts))
													{
														sound_state=ZUM_PERMANENT;										
													}
						sound_out=0;
			break;
		case ZUM_PERMANENT:
			if(ts->alarm || ts->calibr)		// если тревога или калибровка включить прерывистый звук
				{
					sound_state=ZUM_ALARM;
				}else if(!Motion_Allow(ts))
							{
								sound_state=ZUM_MUTE;
							}
				sound_out=1;
			break;
		case ZUM_ALARM:
			if((!ts->alarm) && (!ts->calibr))
				sound_state=ZUM_MUTE;
			sound_out=blink;
			break;
		case ZUM_WARNING:
			if(ts->alarm || ts->calibr)		// если тревога или калибровка включить прерывистый звук
				{
					sound_state=ZUM_ALARM;
				}else 
				{
					sound_out=blink;
					if(blink !=_blink)
					{ _blink=blink;
						if(blink)
							ts->warning=ts->warning-1;
					}
					if(ts->warning==0)
					  sound_state=ZUM_MUTE;
				}
			break;
	}// switch(sound_state)
	
	// изменить состояние переменной blink
	if((unsigned short)(GetSysTime()-time_sound)>vTIME_BLINK)
	{
		blink = blink ^ 0x0F;		// инвертировать биты 0,1,2,3

		time_sound=GetSysTime();
	}
	// отключить звук при тройно нажатии на кнопку "Доп."
	if(GetMessage(MSG_KEY_PRESSED))
	{
		if(GetKeyCode()==KEY_CONST)
			button_const_cnt++;
		else
			button_const_cnt=0;
		if(button_const_cnt==3)
		{
			button_const_cnt=0;
			mute = mute ^ 0x0F;
		}
	}
	// вывести на плату
	if(sound_out)	
	 GPIO_WriteBit(BEEP_ON	, Bit_SET);	
	else 
	 GPIO_WriteBit(BEEP_ON	, Bit_RESET);	
}
//=========================================================================================

void FSM_Sensor_Damage(Sensor_InitTypeDef* sensor, Turnstile_InitTypeDef *ts)
{	static unsigned char warning=0;
	if((sensor->sensor_status & 0x10))
		ts->alarm=1;
	else
		ts->alarm=0;
	if((sensor->sensor_status & 0x0A) && !warning)
	{
		ts->warning= WARNING_SENSOR_MAGNET;
		warning=1;
	}
}
