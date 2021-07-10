#include "keyboard.h"
#include "define.h"
unsigned char key_code, _key_code=0, key_state=0;
unsigned short key_timer;
//===================================================================================
/*
Ф-я (автомат состояния) обработчик нажатой клавиши.
ф-я посылает сообщение [MSG] заносит скан-код нажатой клавиши.
------------------------------------------------------------------------------*/
void KeyboardFSM(void)
{
		key_code= (GPIO_ReadInputDataBit(BUTTON_CALIBR)<<3)|
							(GPIO_ReadInputDataBit(BUTTON_DIR2)<<2)|
							(GPIO_ReadInputDataBit(BUTTON_CONST)<<1)|
							(GPIO_ReadInputDataBit(BUTTON_DIR1));
		key_code= (~key_code)&0x0F;
		switch(key_state)
		{
			case 0:		// проверить нажата ли кнопка?
							if(key_code>0)
							{
								_key_code=key_code;
								key_timer = GetSysTime();
								key_state=1;
							}
							break;
			case 1:		// выждать время дребезга контактов
						 if((GetSysTime()-key_timer)>3)
						 {
							key_state=2; 
						 }
						 break;
			case 2:
						 if(key_code==_key_code)
						 {
							 SendMessage(MSG_KEY_PRESSED);	// послать сообщение о нажатии клавиши.
							 key_state=3;										// если нажата клавиша
						 }
						 else
						 {
							key_state=0; 										// если был дребезг вернуться в исходное состояние
						 }
						 break;
			case 3:
						 if(key_code==_key_code)
						 {
							 key_state=3;										// если нажата клавиша
						 }
						 else
						 {
							key_state=0; 										// если был дребезг вернуться в исходное состояние
						 }
						
		}
}
//=========================================================================================
/*
Ф-я возвращает код нажатой клавиши
----------------------------------------------------------------------------------------*/
unsigned char GetKeyCode(void)
{
		return(key_code);
}


