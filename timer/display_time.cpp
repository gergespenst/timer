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
	UpdateTime(); 
	DisplayHours(g_date.hour);
	DisplayMinute(g_date.minute);
	
	AddTask(UpdateTime,30000,30000);
	
}

void DisplayHours( int8_t hour )
{
	SetDigit(0,hour/10,ELEMON);
	SetDigit(1,hour%10,ELEMON);
}

void DisplayMinute( int8_t min )
{
	SetDigit(3,min/10,ELEMON);
	SetDigit(4,min%10,ELEMON);
}


void BlinkTime()
{

}


void StartBlinkMinute()
{
	SetDigit(3,g_date.minute/10,ELEMBLINK);
	SetDigit(4,g_date.minute%10,ELEMBLINK);
	
}


void StartBlinkHours()
{
	SetDigit(0,g_date.hour/10,ELEMBLINK);
	SetDigit(1,g_date.hour%10,ELEMBLINK);
	
}



void StartShowTime()
{
	
}

void StopShowTime()
{
SetLLineElements(ELEM0,ELEMOFF);
}

void UpdateTime()
{//Эта функция должна грузить время из RTC
	
	(g_date.minute > 60)?( g_date.minute = 0):( g_date.minute++);

}

void DisplayClock()
{
	
	
}
