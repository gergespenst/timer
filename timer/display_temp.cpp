/* 
* display_clock.cpp
*
* Created: 18.09.2017 13:29:44
* Author: USER
*/


#include "display_temp.h"

#define INIT_PARASITE() 		PARASITE_PORT |= (1 << PARASITE_LINE);PARASITE_DDR |= (1 << PARASITE_LINE); // �����
#define PARASITE_ON()   PARASITE_PORT &= ~(1 << PARASITE_LINE)
#define  PARASITE_OFF() PARASITE_PORT |= (1 << PARASITE_LINE);

T_THERMO_SENSOR g_thermo_sens[MAX_NUM_OF_THERMO];
uint8_t g_finded_thermo_sens;
uint64_t g_rom;
void InitTemp(){
	INIT_PARASITE() ;
	oneWireInit(ONE_WIRE_DQ);
	g_finded_thermo_sens = MAX_NUM_OF_THERMO;//������ ����� �������� � ��������
	uint64_t  roms[MAX_NUM_OF_THERMO];
	searchRom(roms, g_finded_thermo_sens);//���� ���������� �� ����
		g_rom = roms[0];
	for(uint8_t i = 0; i < g_finded_thermo_sens; i++){//�������� ������ � ���������� ��������� � ������ ����� ����������� 
		g_thermo_sens[i].rom = roms[i];
		g_thermo_sens[i].exist = 1;
	}
	
}


uint8_t temperatureL;
uint8_t temperatureH;

void UpdateDispTemp( uint8_t nterm )
{
	SetLLineElements(ALLELEM,ELEMOFF);
	SetHLineElements(ALLELEM,ELEMOFF);
	SetHLineElements(ELEM3  ,ELEMON);
		
		
		SetDigit(0,g_finded_thermo_sens,ELEMON);
		if (g_thermo_sens->cur_temp & 0x80)//TODO: ���� ����������� ��������� �� ��������� 0 ����� �����
		{
			SetDigit(1,0,ELEMON);
		} else
 			SetDigit(1,0,ELEMOFF);
// 		SetDigit(2,(temperatureL & 0xF0 )>>4,ELEMON);
// 		SetDigit(3,temperatureL & 0x0F,ELEMON);
		
		SetDigit(2,g_thermo_sens->cur_temp /10,ELEMON);
		SetDigit(3,g_thermo_sens->cur_temp %10,ELEMON);
		//		SetDigit(2,(g_thermo_sens->rom & 0xF0)>>4,ELEMON);
		//		SetDigit(3,g_thermo_sens->rom & 0x0F,ELEMON);

}

enum STATE {SELECT_DEVICE,WAIT_CONVERSION};

void UpdateTemp(){	static uint8_t device = 0;static uint8_t state = SELECT_DEVICE;T_THERMO_SENSOR* currentSensor = g_thermo_sens;PARASITE_OFF();uint8_t ret = reset();
 if(ret) {//���� �� ���� ������ �� ������� �� ������� �� ��������� TODO: ���� ����� ���������� ������ ��� ��������?
 	return;
 	}switch(state){	case SELECT_DEVICE:{		currentSensor = &g_thermo_sens[device];//�������� ���������� ������				setDevice(currentSensor->rom);//�������� ������� ������ ����������		writeByte(CMD_CONVERTTEMP);//��������� ��������������		PARASITE_ON();		AddTask(UpdateTemp,750,UPDATE_TEMP_PERIOD);//������ ������ �� �������� ��������������		state = WAIT_CONVERSION;		}break;	case WAIT_CONVERSION:{		//���� �������������� ��������� �� ������ ������		PARASITE_OFF();		setDevice(currentSensor->rom);//�������� ������� ������ ����������		writeByte(CMD_RSCRATCHPAD);//������ ������		temperatureL = readByte();
		temperatureH = readByte();		if( (currentSensor->rom & 0x0000FF) == 0x10) //DS18S20+		{			currentSensor->cur_temp = (temperatureH & 0x80)	 + (temperatureL >> 1);		}		if( (currentSensor->rom & 0x0000FF) == 0x28) {//DS18B20			currentSensor->cur_temp = (temperatureH << 4) + (temperatureL >> 4);//������������� �����������					}				device++;//��������� � ���������� �������		if (device > g_finded_thermo_sens - 1) device = 0;//��������� �� ����� �� �������		AddTask(UpdateTemp,3000,UPDATE_TEMP_PERIOD);// ����������� ������ ���������� ������ �������		state = SELECT_DEVICE;		}break;		default:;	}

}
