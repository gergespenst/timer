/* 
* display_clock.h
*
* Created: 18.09.2017 13:29:44
* Author: USER
*/


#ifndef __DISPLAY_TEMP_H__
#define __DISPLAY_TEMP_H__

#include "util.h"
#include "led_7_seg_drv.h"
#include "OneWire.h"
#include "task_query.h"

#define MAX_NUM_OF_THERMO 1
#define UPDATE_TEMP_PERIOD 10000 //период обновления температуры на каждом датчике 3 с

#define PARASITE_LINE	   PIND7
#define PARASITE_PORT      PORTD
#define PARASITE_DDR       DDR(PARASITE_PORT)

typedef struct
{
	uint64_t rom;//адрес датчика
	uint8_t cur_temp; // целые градусы от -128 до + 128

	uint8_t hi_temp_tr;//верхняя граница срабатывания ( целые градусы от -128 до + 128) TODO: дурь какаято, сузить диапазон!
	uint8_t lo_temp_tr;//нижняя граница срабатывания
	uint8_t exist:1;//флаг наличия
	
} T_THERMO_SENSOR;

void InitTemp();
void UpdateDispTemp(uint8_t nterm);

void UpdateTemp();
#endif //__DISPLAY_TEMP_H__
