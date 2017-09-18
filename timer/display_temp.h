/* 
* display_clock.h
*
* Created: 18.09.2017 13:29:44
* Author: USER
*/


#ifndef __DISPLAY_CLOCK_H__
#define __DISPLAY_CLOCK_H__

#include "util.h"
#include "led_7_seg_drv.h"
#include "OneWire.h"


void InitTemp();
void UpdateDispTemp(uint8_t nterm);

void UpdateTemp();
#endif //__DISPLAY_CLOCK_H__
