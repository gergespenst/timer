/* 
* keyboard.h
*
* Created: 06.09.2017 12:46:20
* Author: USER
*/


#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include "util.h"
#include "led_7_seg_drv.h"
#ifndef FPTR
	typedef void (*FPTR)(int8_t);
#endif

#define KEYPORT PORTA
#define PINKEY0	5
#define PINKEY1	6
#define PINKEY2	7
#define NOP 0
#define KEY0	1
#define KEY1	2
#define KEY2	4
#define LONG_PRESS 10 //Длительность долгого нажатия в периодах запуска процедуры сканирвоания кнопок

void InitKeyboard(FPTR press,FPTR long_press);//инициализация порта и передача обработчиков нажатия кнопок
void ScanKeyboard();//ПРоцедура сканирования кнопок, запускать в основном цикле или как задачу.


#endif //__KEYBOARD_H__
