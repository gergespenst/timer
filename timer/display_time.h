/* 
* display_time.h
* В этом файле описывается отображение и установка времени
* Created: 05.09.2017 15:47:27
* Author: USER
*/


#ifndef __DISPLAY_TIME_H__
#define __DISPLAY_TIME_H__

#include "led_7_seg_drv.h"
#include "task_query.h"

void InitClock();
void SetHour(int8_t hour);
void SetMinute(int8_t min);
void BlinkTime();
void StartBlinkMinute();
void StartBlinkHours();
void StartBlinkDots();


#endif //__DISPLAY_TIME_H__
