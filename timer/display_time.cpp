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
			date,
			numday,
			month,
			year;
} g_date;

static struct CLOCK{
	DATE date;
	uint8_t edit_min:1;
	uint8_t edit_hour:1;
	uint8_t disp_mode:2;
	} g_clock;

void InitClock()
{
	//Update time
	g_clock.date.hour = 1;
	g_clock.date.minute = 2;
	g_clock.date.second = 3;
	g_clock.date.date = 15;
	g_clock.date.numday = 5;
	g_clock.date.month = 12;
	g_clock.date.year = 17;
	//
	UpdateTime(); 
	DisplayHours(g_clock.date.hour);
	DisplayMinute(g_clock.date.minute);
	
	AddTask(UpdateTime,30000,30000);
	
}


void DisplayHours( int8_t hour )
{
	SetDigit(0,hour/10,ELEMON<<g_clock.edit_hour);
	SetDigit(1,hour%10,ELEMON<<g_clock.edit_hour);
}

void DisplayMinute( int8_t min )
{
	SetDigit(2,min/10,ELEMON<<g_clock.edit_min);
	SetDigit(3,min%10,ELEMON<<g_clock.edit_min);
}



void StartBlinkMinute()
{
	g_clock.edit_min = 1;
}


void StartBlinkHours()
{
	g_clock.edit_hour = 1;	
}



void StartShowTime()
{
	
	DisplayClock();
}

void StopShowTime()
{
	//DeleteTask(DisplayClock);
	SetLLineElements(ELEM0,ELEMOFF);	
	SetDigit(0,0x0F,ELEMOFF);
	SetDigit(1,0x0F,ELEMOFF);
	SetDigit(2,0x0F,ELEMOFF);
	SetDigit(3,0x0F,ELEMOFF);
	SetDots(DOTH|DOTL,ELEMOFF);
}

void UpdateTime()
{//Эта функция должна грузить время из RTC
	if (g_clock.date.minute == 59)
	{
		(g_clock.date.hour > 23)?(g_clock.date.hour = 0):(g_clock.date.hour++);
	}
	(g_clock.date.minute >= 59)?( g_clock.date.minute = 0):( g_clock.date.minute++);
}

void DisplayClock()
{

	int8_t digit[2],dot;
	
	switch(g_clock.disp_mode){
		default:
		case 0 :{
			digit[0] = g_clock.date.hour;
			digit[1] = g_clock.date.minute;
			dot = DOTH|DOTL;
			g_clock.disp_mode = 0;
			}break;
		case 1:{
			digit[0] = g_clock.date.date;
			digit[1] = g_clock.date.month;
			dot = DOTL;
			}break;
		case 2:{
			digit[0] = g_clock.date.numday;
			digit[1] = g_clock.date.year;
			dot = DOTH;
			}break;

	}
	if (digit[0]/10 == 0)
	{
		SetDigit(0,0,ELEMOFF);
	}else
		SetDigit(0,digit[0]/10,ELEMON<<g_clock.edit_hour);
	
	SetDigit(1,digit[0]%10,ELEMON<<g_clock.edit_hour);
	SetDigit(2,digit[1]/10,ELEMON<<g_clock.edit_min);
	SetDigit(3,digit[1]%10,ELEMON<<g_clock.edit_min);
	
	SetDots(DOTL|DOTH,ELEMOFF);
	SetDots(dot,ELEMBLINK>>(g_clock.edit_hour|g_clock.edit_min));
	SetLLineElements(ELEM0,ELEMON);
	
}

uint8_t ClockLongPress( uint8_t key )
{

	switch (key){
		case KEY0:break;
		case KEY1:{
			StartBlinkMinute();
			return 1;
			}break;
		case KEY2:break;
	}
	return 0;

}

uint8_t ClockPress( uint8_t key )
{
	
	switch (key){
		case KEY0:if (~(g_clock.edit_hour || g_clock.edit_min))
		{
			StopShowTime();
			return 0;
		}break;
		case KEY1:{
			g_clock.disp_mode++ ;
			return 1;
			}break;
		case KEY2:if (~(g_clock.edit_hour || g_clock.edit_min))
		{
			StopShowTime();
			return 0;
		}break;
	}
	return 0;
}
