#include "_Timers.h"
#include "messages.h"
#ifndef __STM32_KEYBOARD
#define __STM32_KEYBOARD

//скан-коды клавиш
#define KEY_OPEN1					1
#define KEY_OPEN2					4
#define KEY_CONST					2
#define KEY_CONST_OPEN1		3
#define KEY_CONST_OPEN2		6
#define KEY_CALIBR				8

void KeyboardFSM(void);
unsigned char GetKeyCode(void);
#endif
