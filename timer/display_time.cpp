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
	uint8_t digit[2];
	uint8_t dot:2;
	uint8_t edit_digit:2;
	uint8_t disp_mode:2;
	} g_clock;

enum CLOKMODE {TIME_MOD,DATE_MOD,YEAR_MOD};

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
	g_clock.edit_digit = 0;
	//
	UpdateTime(); 
	AddTask(UpdateTime,30000,30000);
	
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
{
	if (g_clock.edit_digit == 0)
	{


	
//Вместо этого должен быть запрос времени от RTC
		if (g_clock.date.minute == 59)
		{
			(g_clock.date.hour > 23)?(g_clock.date.hour = 0):(g_clock.date.hour++);
		}
		(g_clock.date.minute >= 59)?( g_clock.date.minute = 0):( g_clock.date.minute++);
//////////////////////////////////////////////////////////////////////////
	}
		
}

void DisplayClock()
{
	switch(g_clock.disp_mode){
		default:
		case TIME_MOD :{
			g_clock.digit[0] = g_clock.date.hour;
			g_clock.digit[1] = g_clock.date.minute;
			g_clock.dot = DOTH|DOTL;
			g_clock.disp_mode = TIME_MOD;
		}break;
		case DATE_MOD:{
			g_clock.digit[0] = g_clock.date.date;
			g_clock.digit[1] = g_clock.date.month;
			g_clock.dot = DOTL;
		}break;
		case YEAR_MOD:{
			g_clock.digit[0] = g_clock.date.numday;
			g_clock.digit[1] = g_clock.date.year;
			g_clock.dot = DOTH;
		}break;

	}
			
	if (g_clock.digit[0]/10 == 0)
	{
		SetDigit(0,0,ELEMOFF);
	}else
		SetDigit(0,g_clock.digit[0]/10, ELEMON<<(g_clock.edit_digit>>1));
	
	SetDigit(1,g_clock.digit[0]%10, ELEMON<<(g_clock.edit_digit>>1));
	SetDigit(2,g_clock.digit[1]/10, ELEMON<<(g_clock.edit_digit & 0x01) );
	SetDigit(3,g_clock.digit[1]%10, ELEMON<<(g_clock.edit_digit & 0x01) );
	
	SetDots(DOTL|DOTH,ELEMOFF);
	SetDots(g_clock.dot,ELEMBLINK>>(g_clock.edit_digit != 0x00));
	SetLLineElements(ELEM0,ELEMON);
	
}

uint8_t ClockLongPress( uint8_t key )
{

	switch (key){
		case KEY0:break;
		case KEY1:{
					if (g_clock.edit_digit)
					{
						g_clock.edit_digit = 0;
				
					}else{
						g_clock.disp_mode = TIME_MOD;
						g_clock.edit_digit = 0x02;
				
					}
				return 1;
			}break;
		case KEY2:break;
	}
	return 0;

}

uint8_t PressModeKey(){
	if ( g_clock.edit_digit)//если мигают хоть какието цифры то
	{
		if (g_clock.edit_digit & 0x01)// если мигают правые разряды
		{

			g_clock.disp_mode++;//значит надо переходить на отображение следующей части времени
			SetHLineElements(g_clock.disp_mode,ELEMON);
			if (g_clock.disp_mode > YEAR_MOD)// если перешагнули с отображения года на отображение часов
			{							// то значит закончилди редактирование часов
				g_clock.disp_mode = TIME_MOD;
				g_clock.edit_digit = 0;
				SetTimeToRTC();//записываем установленное время в RTC
				return 1;
			}
		}

		g_clock.edit_digit = ~g_clock.edit_digit;
		return 1;
	}else//если цифры не мигают то меняем режим отображения и возвращаем результат "обработано"
	{
		g_clock.disp_mode++ ;
		return 1;
	}
}

void ChangeDigit(uint8_t sign){//0 - '+', 1 - '-'
	switch(g_clock.disp_mode){
		default:
		case TIME_MOD:{
			
			
			}break;
		case DATE_MOD:{
			
			}break;
		case YEAR_MOD:{
			
			}break;
	}
}


uint8_t ClockPress( uint8_t key )
{
	
	switch (key){
		case KEY0:{//
				if (g_clock.edit_digit){//если мигают цифры то уменьшаем число
					
				}
				else{//иначе гасим часы и отдаем обработку обработчику более высокого уровня
					StopShowTime();
					return 0;
				}
		}
		break;
		
		case KEY1:{//нажатие кнопки MODE
				return PressModeKey();
			}break;
			
		case KEY2:{
					if (g_clock.edit_digit){
						
					}
					else{//иначе гасим часы и отдаем обработку обработчику более высокого уровня
						StopShowTime();
						return 0;
					}
			}break;;
	}
	
	
	return 0;
}

void SetTimeToRTC()
{
 //Заглушка для установки времени в RTC
}
