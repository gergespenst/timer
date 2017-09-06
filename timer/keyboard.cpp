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
	int8_t temp_key = ~( ((PIN(KEYPORT) & _BV(PINKEY0)) >> PINKEY0) |
				 ((PIN(KEYPORT) & _BV(PINKEY1)) >> PINKEY1)<<1 |
				 ((PIN(KEYPORT) & _BV(PINKEY2)) >> PINKEY2)<<2 ) & ((KEY0)|(KEY1)|(KEY2));
	if (temp_key == pressed_key)
	{		
		if ((press_counter > LONG_PRESS) & (pressed_key != NOP))
		{
			(*long_press_func)(pressed_key);
			pressed_key = NOP;
			press_counter = 0;
		}else
			press_counter++;
	}else
	{
		press_counter = 0;
		pressed_key = temp_key;
		if (pressed_key != NOP)
		{
			(*press_func)(pressed_key);
		}
		
	}
}
