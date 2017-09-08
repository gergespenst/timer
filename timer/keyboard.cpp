/* 
* keyboard.cpp
*
* Created: 06.09.2017 12:46:20
* Author: USER
*/


#include "keyboard.h"


static int8_t pressed_key = NOP;
static FPTR press_func,long_press_func;

void InitKeyboard(FPTR press,FPTR long_press)
{
	DDR(KEYPORT) &= ~(_BV(PINKEY0)|_BV(PINKEY1)|_BV(PINKEY2));
	PORT(KEYPORT) |= (_BV(PINKEY0)|_BV(PINKEY1)|_BV(PINKEY2));
	press_func = press;
	long_press_func = long_press;
}

void ScanKeyboard()
{
	static int8_t press_counter;
	//Читаем нажатую клавишу из порта
	int8_t temp_key = ~( ((PIN(KEYPORT) & _BV(PINKEY0)) >> PINKEY0) |
				 ((PIN(KEYPORT) & _BV(PINKEY1)) >> PINKEY1)<<1 |
				 ((PIN(KEYPORT) & _BV(PINKEY2)) >> PINKEY2)<<2 ) & ((KEY0)|(KEY1)|(KEY2)); 
	if (temp_key == (pressed_key & 0x0F))//если нажатая клавиша совпадает с нажатой ранее
	{		
		if ((press_counter > LONG_PRESS) && ((pressed_key & 0x0F) != NOP))
		{//если длительность нажатия больше чем длинное нажатие
			(*long_press_func)(pressed_key & 0x0F);	// выполняем функцию обработчик длительного нажатия
			pressed_key |= 0x80;					// ставим флаг длительного нажатия и сбрасываем счетчик
			press_counter = 0;
		}else//увеличиваем четчик длительности нажатия
		{
			press_counter++;
		}
	}else
	{//если предыдущее и нынешнее значение клавиш не совпадают то в случае если не взведен флаг длительного нажатия выполняем обработчик
		
		if (((pressed_key & 0x0F) != NOP) && ( (pressed_key & 0xF0) == 0))
		{
			(*press_func)(pressed_key & 0x0F);
		}
		// в любом случае сбрасываем длительность нажатия и предыдущую клавишу
		press_counter = 0;
		pressed_key = temp_key;
		
	}
}
