
#include "AS5043.h"
//==========================================================================
/*

*/
void delay(unsigned long p)     //  задержка
{
while(p>0){p--;}
}

//===========================================================================
/*
*: Инициализация выводов GPIO для работы с датчиком

---------------------------------------------------------------------------*/
void init_sensor(GPIO_InitTypeDef* GPIO_InitStruct)
{
	GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct->GPIO_Pin = GPIO_Pin_15;				//DATA
  GPIO_Init(GPIOB, GPIO_InitStruct);
	
	GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct->GPIO_Pin = GPIO_Pin_14;				// CLK
  GPIO_Init(GPIOB, GPIO_InitStruct);
	
	GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct->GPIO_Pin = GPIO_Pin_13;				// CS
  GPIO_Init(GPIOB, GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	delay(10000);		// задержка для инициализации датчика
	
}
//===============================================================================

//==============================================================================
/*
*: Чтение данных с датчика AS5043
аргумент1: указатель на структуру описания пакета Sensor_InitTypeDef
--------------------------------------------------------------------------------*/
void read_sensor(Sensor_InitTypeDef* sensor)
{ unsigned short as5043, temp, x,y;
	static unsigned short read_delay=0;
	
	if(read_delay>1)

	{

	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
	for(int i=0; i<16; i++)
	{
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
		if(GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_15))// 
		{
		as5043 |=0x0001;
		}
		else{
			as5043 &=0xFFFE;
		}
		as5043=as5043<<1;
		delay(200);
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
		delay(200);
		
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
	//sensor->sensor_status=as5043&0x003F;
	/////////////
	sensor->fff = 0;
	/*
	y=as5043;
	//------------------
	  temp = y;
		y = y >> 1;
		x = y;
		for (int i = 0; i < 15; i++)
		{
			y = y >> 1;
			x = x ^ y;

		}
		x = x & 0x0001;
		x = x;
		temp=temp&0x0001;
		if((x^temp)==0)
		 sensor->sensor_data=(as5043>>6)&0x03FF;
		
	//--------------------
	*/
	if((as5043&0x6) == 0)
	//////////////////
	{
	sensor->sensor_data=(as5043>>6)&0x03FF;
	
	}
	sensor->sensor_status|=as5043&0x003F;
	//////////////////////////////////////
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)//DI0
	{
	sensor->sensor_status =0;
	sensor->ddd =0;
	}
	
	
	/*
	sensor->sensor_status|=as5043&0x003F;
	*/
	//////////////////////////////////////
	read_delay=0;
	}//if(read_delay>1000)

	else
	{
		read_delay++;
	}

}




