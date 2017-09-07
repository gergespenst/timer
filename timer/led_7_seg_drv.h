/*
 * led_7_seg_drv.h
 * Функции управления дисплеем из пяти семисегментных индикаторов
 * [a] [b] [c] [d] [e]
 *  -   -       -   -
   | | | |  *f | | | |
    -   -       -   -
   | | | |  *g | | | |
    -   -       -   - 
   [.] [.] [.] [.] [.]
    1   2   3   4   5
В третий символ входит верхний ряд и двоеточие
 * Created: 05.09.2017 12:21:27
 *  Author: USER
 */ 


#ifndef LED_7_SEG_DRV_H_
#define LED_7_SEG_DRV_H_

#include "util.h"
#include <stdint.h>
#include <avr/delay.h>
#include <avr/io.h>

#define CATODE_PORT PORTA
#define CATODE1		0
#define CATODE2		1
#define CATODE3		2
#define CATODE4		3
#define CATODE5		4

#define ANODE_PORT	PORTC
#define ANODEA		0
#define ANODEB		1
#define ANODEC		2
#define ANODED		4
#define ANODEE		5
#define ANODEF		6
#define ANODEG		7
#define ANODEP		3

#define ELEM0 0x01
#define ELEM1 0x02
#define ELEM2 0x04
#define ELEM3 0x10
#define ELEM4 0x20
#define ELEM_DOT 0x40

#define DOTH 0x01
#define DOTL 0x02

#define ELEMOFF 0x00
#define ELEMON	0x01
#define ELEMBLINK 0x02

void Init7Seg();											
	//Инициализация индикатора, конфигурирует порты и выводит 0123
void SetDigit( int8_t digit, int8_t value, int8_t state);	
	//Установка значения цифры, digit - номер цифры (0..3), value - значение цифры (0..E, F - выключена), state - вкл, выкл, мигает
void SetDots(int8_t dots,int8_t state);
	//Управление точками можно управлять сразу обеими, писать через |, state - аналогично
void UpdateSegments();
	//Преобразует значения цифр точек и линий в физические выводы дисплея
void SetSegmentRaw(int8_t segment,int8_t rawVal);
	//Прямая запись в дисплей в обход установок цифр
void DisplayAllSeg();
	//Функция отрисовки сегментов, запускать как задачу или в основном цикле каждые 4 мс для немигающего отображения.
void BlinkAllSeg();
	//Функция мигания дисплея, мигают те сегменты что установлены как blink. Запускать как задачу с периодом мигания.
void SetHLineElements(int8_t elements,int8_t state);
	//Управление элементами верхней строки, можно сразу несколькими - записывать через |
void SetLLineElements(int8_t elements,int8_t state);
	//Управление элементами нижней строки, можно сразу несколькими - записывать через |


#endif /* LED_7_SEG_DRV_H_ */