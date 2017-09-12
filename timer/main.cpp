/*
 * timer.cpp
 *
 * Created: 05.09.2017 9:18:19
 * Author : USER
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "led_7_seg_drv.h"
#include "task_query.h"
#include "display_time.h"
#include "util.h"
#include "keyboard.h"
#include "i2c.h"
#include "ds1307.h"

#define TIMER_CONST (0xFF - F_CPU/1024000L )

#define BLINK_PIN 0
#define BLINK_PORT PORTB


#define INIT_BLINK_DIODE()				DDR(BLINK_PORT) |= _BV(BLINK_PIN)
#define CHANGE_BLINK_STATE()			PORT(BLINK_PORT) ^= _BV(BLINK_PIN)

void StartDispatcherTimer(){//запуск таймера для диспетчера задач
	TCCR0 =  _BV(CS00) | _NBV(CS01) | _BV(CS02);
	TIMSK = _BV(TOIE0);
	TCNT0 = TIMER_CONST;
}

ISR(TIMER0_OVF_vect){//при переполнении вызываем шаг по времени
	TimerProcess();
	TCNT0 = TIMER_CONST;
}


void TestBlink(){
	CHANGE_BLINK_STATE();
}
int8_t test = 1;

void TestInc(){
DisplayAllSeg();

}

void Test(){
	;
	CHANGE_BLINK_STATE();
}

#define NMODES 5
int8_t g_mode = 0;



void PressFunc(int8_t key){
	uint8_t retval = 1;
	switch (g_mode){
		case 0:retval = ClockPress(key);break;
		default: retval = 0;break;
	}
	

 	if(!retval) {	
		switch (key){
			case KEY0:{
				(g_mode > 0)?(g_mode--):(g_mode = NMODES -1);
				SetHLineElements(ALLELEM,ELEMOFF);
				SetHLineElements(key,ELEMON);
			}break;
			case KEY1:break;
			case KEY2:{
				(g_mode < NMODES - 1)?(g_mode++):(g_mode = 0);
				SetHLineElements(ALLELEM,ELEMOFF);
				SetHLineElements(key,ELEMON);
			}break;
		}
	 }

	SetLLineElements(ALLELEM,ELEMOFF);
	SetLLineElements(_BV(g_mode),ELEMON);
	
	};
	
void LongPressFunc(int8_t key){
	uint8_t retval = 1;
	switch (g_mode){
		case 0:retval = ClockLongPress(key);break;
		default: retval = 0;break;
	}
	

	if(!retval) {
		switch (key){
			case KEY0:{
				
			}break;
			case KEY1:break;
			case KEY2:{
			}break;
		}
	SetLLineElements(ELEM1,ELEMON);
	}
	
	CHANGE_BLINK_STATE();
};
	
void DisplayMode(){
	switch(g_mode){
	case 0: DisplayClock();break;
	default:break;
	}
}
	
	
int main(void)
{
    /* INIT SECTION*/
	INIT_BLINK_DIODE();
	Init7Seg();
	InitClock();
	InitKeyboard(PressFunc,LongPressFunc);
	IICInit();
	
	
	/*END: INIT SECTION*/
	sei();
	StartDispatcherTimer();

	//AddTask(TestBlink,0,2000);
	/*Fill task query*/
	AddTask(DisplayAllSeg,0,4);
	AddTask(DisplayMode,0,50);
	AddTask(ScanKeyboard,0,200);
	AddTask(BlinkAllSeg,500,500);
	AddTask(UpdateTime,2000,2000);
	/*END: Fill */
	
	StartShowTime();

	
    while (1) Dispatcher(); 
	
    return 0;
}

