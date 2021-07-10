#include "define.h"

#ifndef __STM32_FLASH
#define __STM32_FLASH

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

void Flash_Unlock(void);
/* Функции блокирующие  */
void Flash_Erase_Page(uint32_t address);
void Flash_Write(unsigned char* data, unsigned int address, unsigned int count);
void Flash_Read(unsigned char* data, unsigned int address, unsigned int count);
#endif
