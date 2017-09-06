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

void Init7Seg();
void SetDigit(int8_t digit, int8_t value);
void DisplayAllDigits();
void SetBlinkDigitPart(int8_t digit,int8_t part);
void BlinkDigitPart(int8_t digit);
void SetHLine(int8_t elements);
void SetLLine(int8_t elements);

#endif /* LED_7_SEG_DRV_H_ */