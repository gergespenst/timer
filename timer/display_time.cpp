/* 
* display_time.cpp
*
* Created: 05.09.2017 15:47:27
* Author: USER
*TODO: �������� ����������� � ����� ����� ����� ������� � ������ ������
*/


#include "display_time.h"
#define UPDATETIME 500
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
//////////////////////////////////////////////////////////////////////////
enum ALARMMODE {COUNTER_MOD,START_TIME_MOD,PERIOD_MOD};
typedef void (*AFPTR)(uint8_t);
static int8_t alarmDigit[2];//����� ��� ������ �� ����� ����� ��� ����

struct ALARM{
	uint8_t startHour;		//��� ������ ��������� ��������
	uint8_t startMin;		//������ ������ ��������� ��������
	uint16_t alarmPeriod;	//����������������� ����������� ��������� � �������, �� 24 ����� (1440 (0x05A0) �����)
							//������� 4 ���� - ���� ������������� ������� 0 - ��������, ����� - �������
	AFPTR alarmFunc;
	uint16_t alarmOnCounter;//������� ����������������� ����������� ���������
	uint8_t alarmEdit:2;		//���� �������������� ����������
	uint8_t alarmDispMode:2;	//����� �������������� � ����������: ��������� ����� ��� ������������������ ���������
	uint8_t alarmOn:2;
	};
	
#define NUMOFALARMS 4
#define ALARMEEPROMSTART 0x04
ALARM g_alarms[NUMOFALARMS];

#define LOADPORT PORTB
#define LOAD0_PIN 0
#define LOAD1_PIN 1
#define LOAD2_PIN 2
#define LOAD3_PIN 3





#define INIT_LOAD()	DDR(LOADPORT) |= _BV(LOAD0_PIN) | _BV(LOAD1_PIN) | _BV(LOAD2_PIN) | _BV(LOAD3_PIN);\
					LOADPORT |= (_BV(LOAD0_PIN) | _BV(LOAD1_PIN) | _BV(LOAD2_PIN) | _BV(LOAD3_PIN));


void SaveAlarmToEEPROM(uint8_t alarm){
	//TODO: ����� �������� ���� �� ���������� ����� 4, ������ ���� �������� �� ���������� ����������� ����
	eeprom_update_byte((uint8_t*)(ALARMEEPROMSTART + alarm*4 + 0),g_alarms[alarm].startHour);
	eeprom_update_byte((uint8_t*)(ALARMEEPROMSTART + alarm*4 + 1),g_alarms[alarm].startMin);
	eeprom_update_byte((uint8_t*)(ALARMEEPROMSTART + alarm*4 + 2),(uint8_t)((g_alarms[alarm].alarmPeriod & 0xFF00)>>8) );
	eeprom_update_byte((uint8_t*)(ALARMEEPROMSTART + alarm*4 + 3),(uint8_t)(g_alarms[alarm].alarmPeriod & 0x00FF));
}


void InitAlarm(){
	for (uint8_t i = 0; i < NUMOFALARMS; i++)
	{
		g_alarms[i].alarmEdit = 0;
		g_alarms[i].alarmOn = 0;
		g_alarms[i].alarmOnCounter = 0;
		
		//������ ���������� ���������� �� ������
		g_alarms[i].startHour	=	eeprom_read_byte((uint8_t*)(ALARMEEPROMSTART + i*4 + 0));
			if (g_alarms[i].startHour > 24) g_alarms[i].startHour = 0;
		g_alarms[i].startMin	=	eeprom_read_byte((uint8_t*)(ALARMEEPROMSTART + i*4 + 1));
			if (g_alarms[i].startMin > 59) g_alarms[i].startMin = 0;
		g_alarms[i].alarmPeriod	=	(eeprom_read_byte((uint8_t*)(ALARMEEPROMSTART+ i*4 + 2)) << 8)
									+ eeprom_read_byte((uint8_t*)(ALARMEEPROMSTART+ i*4 + 3));	
			if ((g_alarms[i].alarmPeriod & 0x0FFF) > 24*60) g_alarms[i].alarmPeriod = 0;
			
		//////////////////////////////////////////////////////////////////////////
		
		
	}
	//���������������� ������ ��������
	INIT_LOAD();
	AddTask(AlarmsCheck,0,UPDATETIME);
}




void UpdateAlarmDigit(uint8_t alarm){
	switch (g_alarms[alarm].alarmDispMode  ){
		case COUNTER_MOD:
				{
				if  ( (g_alarms[alarm].alarmPeriod & 0xF000) != 0x0000)
				{
					uint16_t deltaPeriod = g_alarms[alarm].alarmPeriod - g_alarms[alarm].alarmOnCounter;
					alarmDigit[0] = deltaPeriod/((uint16_t)60);
					alarmDigit[1] = deltaPeriod%((uint16_t)60);
				}else{
					alarmDigit[0] = 0x7f;
					alarmDigit[1] = 0x7f;			
					}
				}
			break;
		case START_TIME_MOD:
			alarmDigit[0] = g_alarms[alarm].startHour;
			alarmDigit[1] = g_alarms[alarm].startMin;
			break;
		case PERIOD_MOD:
			alarmDigit[0] = g_alarms[alarm].alarmPeriod/((uint16_t)60);
			alarmDigit[1] = g_alarms[alarm].alarmPeriod%((uint16_t)60);
			break;
		default:break;
	}	
}

void AlarmFunc(uint8_t alarm,uint8_t state){
		if(state == ELEMOFF){
			LOADPORT |= _BV(alarm);
		}
		else{
			LOADPORT &= ~_BV(alarm);
		}
		
}

void AlarmsCheck(){
	
	for (uint8_t i = 0; i < NUMOFALARMS; i++)
	{
		if ( 
			 ((g_clock.date.hour == g_alarms[i].startHour) && //���� ����� ������� �� � ����� ������� �� ��� ������
			 (g_clock.date.minute == g_alarms[i].startMin))
			  && (g_alarms[i].alarmOnCounter == 0) //�� � ����� ������ ����������� ��� ������������ �������
			)
		{//�������� ������� ����������� ��������
			g_alarms[i].alarmOnCounter = 1;
					
		}
		if ((g_alarms[i].alarmOnCounter != 0) && ( (g_alarms[i].alarmPeriod & 0xF000)>>12))//���� ������ ������ �� �������� � ������ �� �������� 
		{
			//TODO: �������� ��������
			AlarmFunc(i,ELEMON);			
			if (g_alarms[i].alarmOnCounter > (g_alarms[i].alarmPeriod & 0x0FFF))//���� ��������� �� ����� ��� ������ �� ��������� ���
			{
				g_alarms[i].alarmOnCounter = 0;
				//TODO: ��������� ��������
				AlarmFunc(i,ELEMOFF);			
			}else
				g_alarms[i].alarmOnCounter++;
		}
		if ( ((g_alarms[i].alarmPeriod & 0xF000)>>12) == 0)//���� ������ �������� �� ������������� ��������� ��������
		{
			AlarmFunc(i,ELEMOFF);		
		}
	}
		
}



void DisplayAlarm(uint8_t alarm){
	if (g_alarms[alarm].alarmEdit == 0) UpdateAlarmDigit(alarm);
	
		//�������� ��������� ���������� ������
	 	SetLLineElements(ALLELEM,ELEMOFF);
	 	SetLLineElements(_BV(alarm + 1),ELEMON);
		//������� ��������� ���������� 
		if (( (alarmDigit[0] != 0x7f) && (alarmDigit[1] != 0x7f) ) )
		{
			SetDigit(0,alarmDigit[0]/10, ELEMON<<(g_alarms[alarm].alarmEdit>>1));			
			SetDigit(1,alarmDigit[0]%10, ELEMON<<(g_alarms[alarm].alarmEdit>>1));
			SetDigit(2,alarmDigit[1]/10, ELEMON<<(g_alarms[alarm].alarmEdit & 0x01) );
			SetDigit(3,alarmDigit[1]%10, ELEMON<<(g_alarms[alarm].alarmEdit & 0x01) );
			SetDots(DOTL|DOTH,ELEMON);
		}else{
			SetDigit(0,0,ELEMOFF);
			SetDigit(1,0, ELEMON);
			SetDigit(2,0x0F, ELEMON);
			SetDigit(3,0x0F, ELEMON);
			SetDots(DOTL|DOTH,ELEMOFF);
		}	
		SetHLineElements(ALLELEM,ELEMOFF);
		SetHLineElements(_BV(g_alarms[alarm].alarmDispMode )>> 1,ELEMON);
		
}

void ChangeDigit(uint8_t alarm,uint8_t sign){//0 - '+', 1 - '-'
	if(sign)
		alarmDigit[2 - g_alarms[alarm].alarmEdit] = alarmDigit[2 - g_alarms[alarm].alarmEdit] - 1;
	else
		alarmDigit[2 - g_alarms[alarm].alarmEdit] = alarmDigit[2 - g_alarms[alarm].alarmEdit] + 1;
	
		if(alarmDigit[0] > 23) (alarmDigit[0] = 0);
		if(alarmDigit[0] < 0 ) (alarmDigit[0] = 23);
		if(alarmDigit[1] > 59) (alarmDigit[1] = 0);
		if(alarmDigit[1] < 0 ) (alarmDigit[1] = 59);

}

uint8_t AlarmLongPress(uint8_t alarm, uint8_t key){
	switch (key){
		case KEY0:break;
		case KEY1:{
			if (g_alarms[alarm].alarmEdit)//���� ������ ����� �� ������� ������� � �������� ��� ���������
			{
				g_alarms[alarm].alarmEdit = 0;
				
				
				}else{//��� ������ ������� �������� ������� � ��������� � �������������� �������.
				g_alarms[alarm].alarmDispMode = START_TIME_MOD;				
				g_alarms[alarm].alarmEdit =  0x02;
				//��������� ����� � �����
				alarmDigit[0] = g_alarms[alarm].startHour;
				alarmDigit[1] = g_alarms[alarm].startMin;
				
				
			}
			return 1;
		}break;
		case KEY2:break;
	}
	return 0;

}

void StopShowAlarm(uint8_t alarm){
	g_alarms[alarm].alarmDispMode = COUNTER_MOD;
	g_alarms[alarm].alarmEdit = 0;
	
}

uint8_t AlarmPress(uint8_t alarm,uint8_t key){
	switch (key){
		case KEY0:{
				if (g_alarms[alarm].alarmEdit == 0){
					StopShowAlarm(alarm);
					return 0;
				}else{
					ChangeDigit(alarm,1);
					return 1;
				}
			}break;
		case KEY1:{
			if (g_alarms[alarm].alarmEdit != 0)
			{
				if (g_alarms[alarm].alarmEdit & 0x01) // ���� ������ ������ ����� �� ��������� � ������ �����
				{
					if(g_alarms[alarm].alarmDispMode == START_TIME_MOD)//���� ���� � ������������� ������� ������ �� ��������� ����
						{
							g_alarms[alarm].startHour = alarmDigit[0];
							g_alarms[alarm].startMin = alarmDigit[1];
						}
					if(g_alarms[alarm].alarmDispMode == PERIOD_MOD)//���� ������������� ������ �� ��������� ���
					{
						g_alarms[alarm].alarmPeriod = (g_alarms[alarm].alarmPeriod & 0xF000) |
														 alarmDigit[0]*60 + alarmDigit[1];
					}
					
					g_alarms[alarm].alarmDispMode++;
					UpdateAlarmDigit(alarm);
					if (g_alarms[alarm].alarmDispMode > PERIOD_MOD)//���� ���� � ������� �� ������ ��������� ��������������
					{	//��������� ������� � ������������ � ����� ����������� 
						SaveAlarmToEEPROM(alarm);//��������� ��������� � eeprom
						g_alarms[alarm].alarmEdit  = 0; 
						g_alarms[alarm].alarmDispMode = COUNTER_MOD;
						return 1;
					}
					
				}
				(g_alarms[alarm].alarmEdit== 0x02)?(g_alarms[alarm].alarmEdit = 0x01):(g_alarms[alarm].alarmEdit = 0x02);
			}else
			{
				g_alarms[alarm].alarmPeriod ^= 0xF000;
				SaveAlarmToEEPROM(alarm);//��������� ��������� � eeprom
				
			}
			return 1;
		}break;
		case KEY2:{
				if (g_alarms[alarm].alarmEdit == 0){
					StopShowAlarm(alarm);
					return 0;
				}else{
					ChangeDigit(alarm,0);
					return 1;
				}
			}break;
	}	
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void InitClock()
{
	g_clock.edit_digit = 0;
	AddTask(UpdateTime,0,UPDATETIME);
	
}

void StartShowTime()
{	
	DisplayClock();
}

void StopShowTime()
{
	g_clock.disp_mode = TIME_MOD;
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
#if 1
	if (g_clock.date.minute == 59)
		{
			(g_clock.date.hour > 23)?(g_clock.date.hour = 0):(g_clock.date.hour++);
		}
		(g_clock.date.minute >= 59)?( g_clock.date.minute = 0):( g_clock.date.minute++);
#else
		uint8_t tempByte;
		DS1307Read(SECREG,&tempByte);
		if (tempByte & CLOCHALT) g_clock.edit_digit = 0x02;//���� ��������� ����� � RTC �� ��������� � �������������� �������
		DS1307Read(MINREG,&tempByte);
		g_clock.date.minute = ( (tempByte & 0x70) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(HOURREG,&tempByte);
		g_clock.date.hour = ( (tempByte & 0x30) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(MONTHREG,&tempByte);
		g_clock.date.month = ( (tempByte & 0xF0) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(DATEREG,&tempByte);
		g_clock.date.date = ( (tempByte & 0xF0) >> 4)*10 + (tempByte & 0x0F);
		DS1307Read(YEARREG,&tempByte);
		g_clock.date.year = ( (tempByte & 0xF0) >> 4)*10 + (tempByte & 0x0F);
			
		DS1307Read(DAYREG,&tempByte);
		g_clock.date.numday = (tempByte & 0x0F);	
#endif		
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
	
	SetLLineElements(ALLELEM,ELEMOFF);
	SetLLineElements(ELEM0,ELEMON);
	//����� ��������� ����������� � ������ ������
	
	for (uint8_t i = 0; i < NUMOFALARMS; i++)
	{//TODO: �������� ������� ��� �������� ����������
		if((g_alarms[i].alarmPeriod & 0xF000) != 0x0000) SetLLineElements(ELEM1<<i,
																		  ELEMON<<(g_alarms[i].alarmOnCounter != 0) );
	}

	SetHLineElements(ALLELEM,ELEMOFF);
	//����� ������ ���� ����� ��������� �������������
	
	
}

uint8_t ClockLongPress( uint8_t key )
{

	switch (key){
		case KEY0:break;
		case KEY1:{
					if (g_clock.edit_digit)//���� ������ ����� �� ������� ������� � �������� ��� ���������
					{
						g_clock.edit_digit = 0;
						UpdateTime();
				
					}else{//��� ������ ������� �������� ������� � ��������� � �������������� �������. ��������� ������������ �����
						g_clock.disp_mode = TIME_MOD;
						UpdateTime();
						g_clock.edit_digit = 0x02;
						
				
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
			_delay_ms(5);
			g_clock.disp_mode++;//������ ���� ���������� �� ����������� ��������� ����� �������
			
			
			if (g_clock.disp_mode > YEAR_MOD)// ���� ����������� � ����������� ���� �� ����������� �����
			{								// �� ������ ��������� �������������� �����
				g_clock.disp_mode = TIME_MOD;
				g_clock.edit_digit = 0;			
				UpdateTime();
				return 1;
			}
			UpdateDigit();
		}

		
		(g_clock.edit_digit == 0x02)?(g_clock.edit_digit = 0x01):(g_clock.edit_digit = 0x02);
		
		return 1;
	}else//���� ����� �� ������ �� ������ ����� ����������� � ���������� ��������� "����������"
	{
		g_clock.disp_mode++ ;
		UpdateTime();
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
 
 

