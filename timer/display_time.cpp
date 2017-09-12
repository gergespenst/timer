/* 
* display_time.cpp
*
* Created: 05.09.2017 15:47:27
* Author: USER
*TODO: �������� ����������� � ����� ����� ����� ������� � ������ ������
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
	int8_t digit[2];
	uint8_t dot:2;
	uint8_t edit_digit:2;
	uint8_t disp_mode:2;
	
	} g_clock;

enum CLOKMODE {TIME_MOD,DATE_MOD,YEAR_MOD};

void InitClock()
{
	g_clock.edit_digit = 0;
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

void UpdateDigit(){
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
}


void UpdateTime()
{
	if (g_clock.edit_digit == 0)
	{	
//������ ����� ������ ���� ������ ������� �� RTC
// 		if (g_clock.date.minute == 59)
// 		{
// 			(g_clock.date.hour > 23)?(g_clock.date.hour = 0):(g_clock.date.hour++);
// 		}
// 		(g_clock.date.minute >= 59)?( g_clock.date.minute = 0):( g_clock.date.minute++);
		uint8_t tempByte;
		DS1307Read(SECREG,&tempByte);
		if (tempByte & CLOCHALT) g_clock.edit_digit = 0x02;//���� ��������� ����� � RTC �� ��������� � �������������� �������
		DS1307Read(MINREG,&tempByte);
		g_clock.date.minute = ( (tempByte & 0x70) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(HOURREG,&tempByte);
		g_clock.date.hour = ( (tempByte & 0x30) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(MONTHREG,&tempByte);
		g_clock.date.month = ( (tempByte & 0xF0) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(YEARREG,&tempByte);
		g_clock.date.year = ( (tempByte & 0xF0) >> 4)*10 + (tempByte & 0x0F);
								
		DS1307Read(DAYREG,&tempByte);
		g_clock.date.numday = (tempByte & 0x0F);	
		
//////////////////////////////////////////////////////////////////////////

	UpdateDigit();
	}
	
		
}

void DisplayClock()
{	
			
	if ( (g_clock.digit[0]/10 == 0) && (g_clock.disp_mode == TIME_MOD))
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
					if (g_clock.edit_digit)//���� ������ ����� �� ������� ������� � �������� ��� ���������
					{
						g_clock.edit_digit = 0;
				
					}else{//��� ������ ������� �������� ������� � ��������� � �������������� �������. ��������� ������������ �����
						g_clock.disp_mode = TIME_MOD;
						g_clock.edit_digit = 0x02;
						UpdateDigit();
				
					}
				return 1;
			}break;
		case KEY2:break;
	}
	return 0;

}

uint8_t PressModeKey(){
	
	if ( g_clock.edit_digit)//���� ������ ���� ������� ����� ��
	{
		if (g_clock.edit_digit & 0x01)// ���� ������ ������ �������
		{
			SetTimeToRTC();//���������� ������������� ����� � RTC
			g_clock.disp_mode++;//������ ���� ���������� �� ����������� ��������� ����� �������
			
			
			if (g_clock.disp_mode > YEAR_MOD)// ���� ����������� � ����������� ���� �� ����������� �����
			{							// �� ������ ���������� �������������� �����
				g_clock.disp_mode = TIME_MOD;
				g_clock.edit_digit = 0;			
				UpdateDigit();
				return 1;
			}
			UpdateDigit();
		}

		
		(g_clock.edit_digit == 0x02)?(g_clock.edit_digit = 0x01):(g_clock.edit_digit = 0x02);
		
		return 1;
	}else//���� ����� �� ������ �� ������ ����� ����������� � ���������� ��������� "����������"
	{
		g_clock.disp_mode++ ;
		UpdateDigit();
		return 1;
	}
}

void ChangeDigit(uint8_t sign){//0 - '+', 1 - '-'
	if(sign)
		g_clock.digit[2 - g_clock.edit_digit] = g_clock.digit[2 - g_clock.edit_digit] - 1;
	else
		g_clock.digit[2 - g_clock.edit_digit] = g_clock.digit[2 - g_clock.edit_digit] + 1;
		
	switch(g_clock.disp_mode){
		default:
		case TIME_MOD:{
			if(g_clock.digit[0] > 23) (g_clock.digit[0] = 0);
			if(g_clock.digit[0] < 0 ) (g_clock.digit[0] = 23);
			if(g_clock.digit[1] > 59) (g_clock.digit[1] = 0);
			if(g_clock.digit[1] < 0 ) (g_clock.digit[1] = 59);
			}break;
		case DATE_MOD:{
			if(g_clock.digit[0] > 31) (g_clock.digit[0] = 1);
			if(g_clock.digit[0] < 1 ) (g_clock.digit[0] = 31);
			if(g_clock.digit[1] > 12) (g_clock.digit[1] = 1);
			if(g_clock.digit[1] < 1 ) (g_clock.digit[1] = 12);
			}break;
		case YEAR_MOD:{
			if(g_clock.digit[0] > 7) (g_clock.digit[0] = 1);
			if(g_clock.digit[0] < 1 ) (g_clock.digit[0] = 7);
			}break;
	}
}


uint8_t ClockPress( uint8_t key )
{
	
	switch (key){
		case KEY0:{//
				if (g_clock.edit_digit){//���� ������ ����� �� ��������� �����
					ChangeDigit(1);
					return 1;
				}
				else{//����� ����� ���� � ������ ��������� ����������� ����� �������� ������
					StopShowTime();
					return 0;
				}
		}
		break;
		
		case KEY1:{//������� ������ MODE
				return PressModeKey();
			}break;
			
		case KEY2:{
					if (g_clock.edit_digit){
						ChangeDigit(0);
						return 1;
					}
					else{//����� ����� ���� � ������ ��������� ����������� ����� �������� ������
						StopShowTime();
						return 0;
					}
			}break;;
	}
	
	
	return 0;
}

void SetTimeToRTC()
{
 //�������� ��� ��������� ������� � RTC
 // � ����������� �� ������ ������������ ����� � ��������
	
		 switch(g_clock.disp_mode){
			 case TIME_MOD :{
				 g_clock.date.hour = g_clock.digit[0];	 
				 g_clock.date.minute = g_clock.digit[1];
		
					DS1307Write(MINREG, ( (g_clock.digit[1]/10)<<4 )|( (g_clock.digit[1]%10) ) );
					DS1307Write(HOURREG,( (g_clock.digit[0]/10)<<4 )|( (g_clock.digit[0]%10) ) );
					DS1307Write(SECREG,0x00);//���������� ������� � ��������� �����
			 }break;
			 case DATE_MOD:{
				 g_clock.date.date = g_clock.digit[0];
				 g_clock.date.month = g_clock.digit[1];
					DS1307Write(MONTHREG, ( (g_clock.digit[1]/10)<<4 )|( (g_clock.digit[1]%10) ) );
					DS1307Write(DATEREG,( (g_clock.digit[0]/10)<<4 )|( (g_clock.digit[0]%10) ) );
										
			 }break;
			 case YEAR_MOD:{
				g_clock.date.numday = g_clock.digit[0];
				g_clock.date.year = g_clock.digit[1];
					DS1307Write(YEARREG,( (g_clock.digit[1]/10)<<4 )|( (g_clock.digit[1]%10) ) );
					DS1307Write(DAYREG,  (g_clock.digit[0]%10) );
			 }break;
		}
	
	
 }
