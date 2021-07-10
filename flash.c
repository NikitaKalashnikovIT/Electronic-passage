#include "flash.h"

//==================================================================================
/* Разблокировать Flash память    */
void Flash_Unlock(void) 
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}
//==================================================================================
/*
	Функция стирает одну страницу. В качестве адреса можно использовать любой
	принадлежащий диапазону адресов той странице которую нужно очистить.
*/
void Flash_Erase_Page(uint32_t address) 
{
	FLASH->CR|= FLASH_CR_PER; //Устанавливаем бит стирания одной страницы
	FLASH->AR = address; // Задаем её адрес
	FLASH->CR|= FLASH_CR_STRT; // Запускаем стирание
	while(!(FLASH->SR & FLASH_SR_EOP))	;  //Ждем пока страница сотрется.
	FLASH->CR&= ~FLASH_CR_PER; //Сбрасываем бит обратно
}
//==================================================================================
/*
 data - указатель на записываемые данные
 address - адрес во flash
 count - количество записываемых байт, должно быть кратно 2
*/
void Flash_Write(unsigned char* data, unsigned int address, unsigned int count) {
	unsigned int i;

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PG;					 //Разрешаем программирование флеша

	for (i = 0; i < count; i += 2) {
		*(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
		while (!(FLASH->SR & FLASH_SR_EOP)); //Ожидаем готовности флеша к записи
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR &= ~(FLASH_CR_PG);
}
//===========================================================================================
/*
 data - указатель на читаемые данные
 address - адрес во flash
 count - количество записываемых байт, должно быть кратно 2
*/
void Flash_Read(unsigned char* data, unsigned int address, unsigned int count) {
	unsigned int i;
	for (i = 0; i < count; i++) {
		data[i]=*(__IO uint8_t*)(address + i);

	}
}
