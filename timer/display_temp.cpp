/* 
* display_clock.cpp
*
* Created: 18.09.2017 13:29:44
* Author: USER
*/


#include "display_temp.h"

void InitTemp(){

	
}
uint8_t temperatureL;
uint8_t temperatureH;
void UpdateDispTemp( uint8_t nterm )
{
	SetLLineElements(ALLELEM,ELEMOFF);
	SetHLineElements(ALLELEM,ELEMOFF);
	SetHLineElements(ELEM3 << nterm ,ELEMON);
		
		
		SetDigit(0,temperatureH/16,ELEMON);
		SetDigit(1,temperatureH%16,ELEMON);
		SetDigit(2,temperatureL/16,ELEMON);
		SetDigit(3,temperatureL%16,ELEMON);
}

void UpdateTemp(){
// 	double retd = 0;
// 	
// 	oneWireInit(ONE_WIRE_DQ);
// 	
// 	skipRom();
// 	writeByte(CMD_CONVERTTEMP);
// 	
// 	_delay_ms(750);
// 	
// 	skipRom();
// 	writeByte(CMD_RSCRATCHPAD);
// 	
// 	temperatureL = readByte();
// 	temperatureH = readByte();
// 	
// 	retd = ((temperatureH << 8) + temperatureL) * 0.0625;
}
