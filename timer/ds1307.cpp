/* 
* ds1307.cpp
*
* Created: 11.09.2017 13:44:08
* Author: USER
*/


#include "ds1307.h"

uint8_t DS1307Read( uint8_t addr,uint8_t * data )
{
	IICStart();
	uint8_t res = IICByteWrite(DS1307ADDR);
	if (res) return 0;
	res = IICByteWrite(addr);
	if (res) return 0;
	IICStart();
	res = IICByteWrite(DS1307ADDR | 0x01);
	if (res) return 0;
	*data = IICByteRead(1);
	//if (res) return 0;
	IICStop();
	return 1;

}

uint8_t DS1307Write( uint8_t addr,uint8_t data )
{
	uint8_t res; // Результат
	IICStart(); // СТАРТ
	res = IICByteWrite(DS1307ADDR); // адрес DS1307 + бит W
	if(res)    return 0; // ОШИБКА
	// Передача адреса необходимого регистра
	res = IICByteWrite(addr);
	if(res)    return 0; // ОШИБКА
	res = IICByteWrite(data); // Запись данных
	if(res)    return 0; // ОШИБКА
	IICStop(); // СТОП
	return 1;
}
