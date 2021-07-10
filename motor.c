#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "turnstile.h"
#include "define.h"
#include "AS5043.h"
#include "motor.h"
/* Ф-я инициализации входов/выходов необходимых для работы с DSM */
void init_motor(void)
{
//ВЫХОДА
GPIO_InitTypeDef PORTC_init_struct;
PORTC_init_struct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
PORTC_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
PORTC_init_struct.GPIO_Mode = GPIO_Mode_Out_OD;//выхода с открытым стоком;
GPIO_Init(GPIOC,&PORTC_init_struct);
//ВХОДА
PORTC_init_struct.GPIO_Pin = GPIO_Pin_11;
PORTC_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
PORTC_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//PORTC_init_struct.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(GPIOC,&PORTC_init_struct);
}
//=====================================================================================================

short dFi0(short fi01, short fi02)
{
	short dFi;
	dFi=fi02-fi01;
	if(dFi<0){dFi=-(dFi);}
	if((dFi)>512){return(-((dFi)-1024));}
  return(dFi);
}
//=====================================================================================================
//=====================================================================================================
//КОНТРОЛЬ ОТСТАВАНИЯ РОТОРА ОТ МАГ.ПОЛЯ СТАТОРА
unsigned char Control_step(unsigned short sensor,char precision)//если функция = 1,то отстование зафиксированно
//sensor-значение датчика в данный момент,precision-точность определеня лежит в пределах 0-7
{
	static unsigned char verify_FG =0;//1-старт счетчика,0=стоп
	static short start_point =0;//начальная точка отсчета угла
	static short result =0;//значение подтвержнея отстования  ротора от маг.поля статора (1-есть,0-нет)
	static short condition =0;//точность определия отстования
	
	if (GPIO_ReadInputDataBit(BUTTON_DIR1)== 0)
	{
		verify_FG =0;
		start_point =sensor;
		result =0;
		condition = GPIO_ReadInputDataBit(BUTTON_DIR1);
	}
		if ((GPIO_ReadInputDataBit(DSM_STEP)) != condition)//каждые 2-ва шага проходить проверку
		{
			if(verify_FG ==1)
			{
					if((dFi0(start_point, sensor))>precision)
					{
					verify_FG =0;
					result=0;
					condition = GPIO_ReadInputDataBit(DSM_STEP);
					}
					else
					result=1;
			}
			else
			{
			start_point = sensor;
			condition = GPIO_ReadInputDataBit(DSM_STEP);
			verify_FG = 1;
			}
		}
	return result;
}
//=====================================================================================================

//=====================================================================================================
//поиск нулевых зон и зон dsm
void FSM_location_dsm(Sensor_InitTypeDef* sensor,dsm_InitTypeDef *dsm,EEPROM_InitTypeDef *eeprom)
{
//dsm->xxx =(GPIO_ReadInputDataBit(DSM_STEP));
	static short fi_zero_current0;//текущая нулевая зона
	short dfi0,verify_location;//dfi0-положение штанг относительно текущей нулевой точки 
	//verify_location-предпологаемая зона dsm
	static short fact_pass_zone0=0;
	short *min_angle,z1,z2,z3;//мин угол от штанг до одной из нулевой зоны
	static short damper = 0;//демпфер необходимый для точного перехода на границе двух зон dsm (работает при движении штанг по направления прохода) 
	static short count=0,zone_count;//count-количество опросов необходимое для подтверждения одной из зонm
	//zone_count-зона dsm на предыдущем опросе
	//////////////////////////////
	//static short fi_zero_current01;
	//dsm->xxx =fi_zero_current0;/////////////////////////////////////////////////
	/*
	static short yty = 0;
	if((dFi0(yty,sensor->sensor_data))>5)
	{
	//ts->rrr=dFi0;
	dsm->xxx=dFi0(yty,sensor->sensor_data);
	}
	yty = sensor->sensor_data;
	
		if ((GPIO_ReadInputDataBit(BUTTON_DIR1)) == 0  )
	{dsm->xxx = 0;}
	*/
	//////////////////////////////
	
	//*******определение нулевой зоны
		if((dFi0(eeprom->fi_zero1,sensor->sensor_data)<eeprom->eps_zero))
	{
		fi_zero_current0=eeprom->fi_zero1;
		fact_pass_zone0=0;
	}
	if((dFi0(eeprom->fi_zero2,sensor->sensor_data)<eeprom->eps_zero))
	{
		fi_zero_current0=eeprom->fi_zero2;
		fact_pass_zone0=0;
	}	
	if((dFi0(eeprom->fi_zero3,sensor->sensor_data)<eeprom->eps_zero))
	{
		fi_zero_current0=eeprom->fi_zero3;
		fact_pass_zone0=0;
	}
	//******************************************************************
	//*******При неопределений нулевой зоны(проскоке),определение нулевой зоны
	dfi0= dFi0(fi_zero_current0,sensor->sensor_data);
	//if(dfi0>(341 + eeprom->eps_zero))
	if(dfi0>341)
		{
		z1 = dFi0(eeprom->fi_zero1,sensor->sensor_data);
		z2 = dFi0(eeprom->fi_zero2,sensor->sensor_data);
		z3 = dFi0(eeprom->fi_zero3,sensor->sensor_data);
		min_angle = (z1 > z2) ? &z2 : &z1;
		min_angle = (*min_angle > z3) ? &z3 : min_angle;
		z1 = eeprom->fi_zero1;
		z2 = eeprom->fi_zero2;
		z3 = eeprom->fi_zero3;
		fi_zero_current0 = *min_angle;
		}
	//******************************************************************
	//*******определение зоны dsm
	if(dfi0>(dsm_zone7-damper)) {verify_location = 7;goto loop;}
	if(dfi0>(dsm_zone6-damper)) {verify_location = 6;goto loop;}
	if(dfi0>(dsm_zone5-damper)) {verify_location = 5;goto loop;}
	if(dfi0>(dsm_zone4-damper)) {verify_location = 4;goto loop;}
	if(dfi0>(dsm_zone3-damper)) {verify_location = 3;goto loop;}
	if(dfi0>(dsm_zone2-damper)) {verify_location = 2;goto loop;}
	if(dfi0>(dsm_zone1-damper)) {verify_location = 1;goto loop;}
	if(dfi0>(eeprom->eps_zero-damper)){verify_location = 0;goto loop;}
	loop:
		if (verify_location == zone_count)
		{
			count++;
			if(count == 10)
			{
			damper = 0;
			count = 0;
			dsm->location_dsm = verify_location;
			}
		} 
		else
		{
			if(zone_count > verify_location){damper = 2;}
			zone_count = verify_location;
			count=0;
		}
		//******************************************************************
			return;
}
//=====================================================================================================

//=====================================================================================================
//работа c зонами dsm
void working_with_zones(Sensor_InitTypeDef* sensor,dsm_InitTypeDef *dsm,EEPROM_InitTypeDef *eeprom,Turnstile_InitTypeDef *Turnstile)
{
////////////
	GPIO_WriteBit(DO6, Bit_SET);
	switch (dsm->location_dsm)
		{
		case  0:
			dsm->new_speed = 0;
				dsm->delay_rotor = (Control_step(sensor->sensor_data,4));
			break;
		case  1:
			dsm->delay_rotor = (Control_step(sensor->sensor_data,4));
		dsm->new_speed = 9;
		/*
		if(dsm->delay_rotor)
		Control_step(sensor->sensor_data,1);
			*/
			/*
			//dsm->new_speed = 9;
			if((dsm->delay_rotor == 1)||(slow_step == 1))
			{
			dsm->new_speed = 5;
			slow_step = 1;
			}
			*/
			break;
		case  2:
			dsm->delay_rotor = (Control_step(sensor->sensor_data,4));
			dsm->new_speed = 6;
			/*
			if(dsm->delay_rotor)
			Control_step(sensor->sensor_data,1);
			*/
			/*
			if((dsm->delay_rotor == 1)||(slow_step == 1))
			{
			dsm->new_speed = 9;
			}
			*/

			/*
			if((dsm->delay_rotor == 1)||(slow_step == 1))
			{
			dsm->new_speed = 5;
			slow_step = 1;
			}
			*/
			break;
		case  3:
			dsm->delay_rotor = (Control_step(sensor->sensor_data,4));
			dsm->new_speed = 3;
			/*
			if((dsm->delay_rotor == 1)||(slow_step == 1))
			{
			dsm->new_speed = 5;
			slow_step = 1;
			}
			*/
			break;
		case  4:
		dsm->delay_rotor = (Control_step(sensor->sensor_data,7));
		dsm->new_speed = 5;//delete
			/*
			if((dsm->delay_rotor == 1)||(slow_step == 1))
			{
			dsm->new_speed = 5;
			slow_step = 1;
			}
			*/
			break;
		case  5:
		dsm->delay_rotor = (Control_step(sensor->sensor_data,7));
			dsm->new_speed = 4;
			/*
			if((dsm->delay_rotor == 1)||(slow_step == 1))
			{
			dsm->new_speed = 5;
			slow_step = 1;
			}
			*/

		  break;
		case  6:
		dsm->delay_rotor = (Control_step(sensor->sensor_data,4));
			dsm->new_speed = 0;
			dsm->delay_rotor = 0;
			break;
		case  7:
			dsm->new_speed = 0;
			dsm->delay_rotor = 0;
			break;
		}

		if(dsm->delay_rotor == 1){dsm->new_speed = 0;}

		/////////
		/*
			if (GPIO_ReadInputDataBit(BUTTON_DIR2)== 0)
		{
		dsm->new_speed = 10;
		}
		else
		dsm->new_speed = 0;
		*/
		/////////
		  //dsm->new_speed |= 16;
		//*******определение направления штанг и движение в противоположную сторону разрешонному  	
		/*
		if((64& Turnstile->location) !=0 )
			dsm->new_speed |= 16;//против часовой
			else
			dsm->new_speed =(dsm->new_speed & 239);//по часовой
		*/
		/*
		if(((64 & Turnstile->location)>>6)!=((128& Turnstile->location)>>7))
		dsm->new_speed = 0;
		if(((64 & Turnstile->location)>>6)!=((128& Turnstile->location)>>7))
		dsm->xxx =1;
		else
		dsm->xxx =0;
		*/
		//********************************************************************	
    //*******вывод скорости и направления на GPIOC 
	  dsm->PORTC =(0xf83f)&(GPIO_ReadOutputData(GPIOC));
	  dsm->new_speed = (dsm->new_speed << 6)|(dsm->PORTC);
	  GPIO_Write(GPIOC, dsm->new_speed);

	

}