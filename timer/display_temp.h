/* 
* display_clock.h
*
* Created: 18.09.2017 13:29:44
* Author: USER
*/


#ifndef __DISPLAY_TEMP_H__
#define __DISPLAY_TEMP_H__

#include "util.h"
#include "led_7_seg_drv.h"
#include "OneWire.h"
#include "task_query.h"
#include "keyboard.h"
#include <avr/eeprom.h>

#define MAX_NUM_OF_THERMO 1
#define UPDATE_TEMP_PERIOD 10000 //������ ���������� ����������� �� ������ ������� 3 �

#define PARASITE_LINE	   PIND7
#define PARASITE_PORT      PORTD
#define PARASITE_DDR       DDR(PARASITE_PORT)

#define THRESHOLD_TEMP 27 //����������� ������������ ���������� �� ������������ TODO: ��������� ����� ���� ������ � ������

typedef struct
{
	uint64_t rom;//����� �������
	int8_t cur_temp; // ����� ������� �� -128 �� + 128

	int8_t hi_temp_tr;//������� ������� ������������ ( ����� ������� �� -128 �� + 128) TODO: ���� �������, ������ ��������!
	int8_t lo_temp_tr;//������ ������� ������������
	uint8_t exist:1;//���� �������
	uint8_t edit_hi:1;//���� ������ �������������� ������� �������
	
} T_THERMO_SENSOR;

void InitTemp();
void UpdateDispTemp(uint8_t nterm);
uint8_t IsNormalTemp();//������� ���������� 1 ���� ����������� � ����� � 0 ���� ����������� ���� ��������
uint8_t TempLongPress(uint8_t alarm, uint8_t key);
uint8_t TempPress(uint8_t alarm,uint8_t key);

void UpdateTemp();
#endif //__DISPLAY_TEMP_H__
