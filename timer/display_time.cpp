/* 
* display_time.cpp
*
* Created: 05.09.2017 15:47:27
* Author: USER
*/


#include "display_time.h"

static struct DATE{
	int8_t	hour,
			minute,
			second,
			numday,
			month,
			year;
} g_date;

void InitClock()
{
	//Update time
	g_date.hour = 1;
	g_date.minute = 2;
	g_date.second = 3;
	g_date.numday = 0;
	g_date.month = 12;
	g_date.year = 17;
	// 
	SetHour(g_date.hour);
	SetMinute(g_date.minute);
	
}

void SetHour( int8_t hour )
{
	SetDigit(0,hour/10);
	SetDigit(1,hour%10);
}

void SetMinute( int8_t min )
{
	SetDigit(3,min/10);
	SetDigit(4,min%10);
}


void BlinkTime()
{

}
void BlinkMinTask(){
	BlinkDigitPart(3);
	BlinkDigitPart(4);
}

void StartBlinkMinute()
{
	SetBlinkDigitPart(3,0xFF & ~_BV(ANODEP));
	SetBlinkDigitPart(4,0xFF & ~_BV(ANODEP));
	AddTask(BlinkMinTask,0,500);
}

void BlinkHoursTask(){
	BlinkDigitPart(0);
	BlinkDigitPart(1);
}

void StartBlinkHours()
{
	SetBlinkDigitPart(0,0xFF & ~_BV(ANODEP));
	SetBlinkDigitPart(1,0xFF & ~_BV(ANODEP));
	AddTask(BlinkHoursTask,0,500);
}

void BlinkDotsTask(){
	BlinkDigitPart(2);
}

void StartBlinkDots()
{
	
	SetBlinkDigitPart(2,0b11000000);
	AddTask(BlinkDotsTask,0,500);
}
