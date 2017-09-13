/* 
* display_time.h
* � ���� ����� ����������� ����������� � ��������� �������
* Created: 05.09.2017 15:47:27
* Author: USER
*/


#ifndef __DISPLAY_TIME_H__
#define __DISPLAY_TIME_H__

#include "led_7_seg_drv.h"
#include "task_query.h"
#include "keyboard.h"
#include "ds1307.h"
#include <avr/eeprom.h>

void InitAlarm();
void AlarmFunc(uint8_t alarm,uint8_t state);
void AlarmsCheck();
void DisplayAlarm(uint8_t alarm);
uint8_t AlarmLongPress(uint8_t alarm, uint8_t key);
uint8_t AlarmPress(uint8_t alarm,uint8_t key);

void InitClock();
void DisplayHours(int8_t hour);
void DisplayMinute(int8_t min);
void BlinkTime();
void StartBlinkMinute();
void StartBlinkHours();
void StartShowTime();
void StopShowTime();
void UpdateTime();
void DisplayClock();

uint8_t ClockLongPress(uint8_t key);
uint8_t ClockPress(uint8_t key);
void SetTimeToRTC();



#endif //__DISPLAY_TIME_H__
