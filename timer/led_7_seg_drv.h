/*
 * led_7_seg_drv.h
 * ������� ���������� �������� �� ���� �������������� �����������
 * [a] [b] [c] [d] [e]
 *  -   -       -   -
   | | | |  *f | | | |
    -   -       -   -
   | | | |  *g | | | |
    -   -       -   - 
   [.] [.] [.] [.] [.]
    1   2   3   4   5
� ������ ������ ������ ������� ��� � ���������
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

// #define ELEM0 0x01
// #define ELEM1 0x02
// #define ELEM2 0x04
// #define ELEM3 0x10
// #define ELEM4 0x20
// #define ALLELEM (ELEM0 | ELEM1 | ELEM2 | ELEM3 | ELEM4)
enum ELEMENTS {	ELEM0 = 0x01,
				ELEM1 = 0x02,
				ELEM2 = 0x04,
				ELEM3 = 0x08, 
				ELEM4 = 0x10,
				ALLELEM = 0x1F};
#define ELEM_DOT 0x40

#define DOTH 0x01
#define DOTL 0x02

#define ELEMOFF 0x00
#define ELEMON	0x01
#define ELEMBLINK 0x02

void Init7Seg();											
	//������������� ����������, ������������� ����� � ������� 0123
void SetDigit( int8_t digit, int8_t value, int8_t state);	
	//��������� �������� �����, digit - ����� ����� (0..3), value - �������� ����� (0..E, F - ���������), state - ���, ����, ������
void SetDots(int8_t dots,int8_t state);
	//���������� ������� ����� ��������� ����� ������, ������ ����� |, state - ����������
void UpdateSegments();
	//����������� �������� ���� ����� � ����� � ���������� ������ �������
void SetSegmentRaw(int8_t segment,int8_t rawVal);
	//������ ������ � ������� � ����� ��������� ����
void DisplayAllSeg();
	//������� ��������� ���������, ��������� ��� ������ ��� � �������� ����� ������ 4 �� ��� ����������� �����������.
void BlinkAllSeg();
	//������� ������� �������, ������ �� �������� ��� ����������� ��� blink. ��������� ��� ������ � �������� �������.
void SetHLineElements(int8_t elements,int8_t state);
	//���������� ���������� ������� ������, ����� ����� ����������� - ���������� ����� |
void SetLLineElements(int8_t elements,int8_t state);
	//���������� ���������� ������ ������, ����� ����� ����������� - ���������� ����� |


#endif /* LED_7_SEG_DRV_H_ */