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



void StartDispatcherTimer(){//запуск таймера для диспетчера задач
	TCCR0 =  _BV(CS00) | _NBV(CS01) | _BV(CS02);
	TIMSK = _BV(TOIE0);
	TCNT0 = TIMER_CONST;
}

ISR(TIMER0_OVF_vect){//при переполнении вызываем шаг по времени
	TimerProcess();
	TCNT0 = TIMER_CONST;
}



int8_t test = 1;

void TestInc(){
DisplayAllSeg();

}



#define NMODES 5
#define CLOCKMODE  0x00
#define TIMER1MODE 0x01
int8_t g_mode = 0;

//функция которая сбрасывает экран к отображению часов при отсутствии нажатия кнопок 
#define TIMETORES 10000
void ResetAfterWait(){
	ResetToClock();
	g_mode = 0;
}

void PressFunc(int8_t key){
	AddTask(ResetAfterWait,TIMETORES,0);
	uint8_t retval = 1;
	switch (g_mode){
		case CLOCKMODE: retval = ClockPress(key);break;
			case 1:
			case 2:
			case 3:
			case 4: retval = AlarmPress(g_mode - 1,key);break;
		default: retval = 0;break;
	}

 	if(!retval) {	
		switch (key){
			case KEY0:{
				(g_mode > CLOCKMODE)?(g_mode--):(g_mode = NMODES -1);
			}break;
			case KEY1:break;
			case KEY2:{
				(g_mode < NMODES - 1)?(g_mode++):(g_mode = CLOCKMODE);
			}break;
		}
	 }
};
	
void LongPressFunc(int8_t key){
	AddTask(ResetAfterWait,TIMETORES,0);
	uint8_t retval = 1;
	switch (g_mode){
		case CLOCKMODE:retval = ClockLongPress(key);break;
			case 1:
			case 2:
			case 3:
			case 4:retval = AlarmLongPress(g_mode - 1,key);break;
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
	}

};
	
void DisplayMode(){
	switch(g_mode){
	case 0: DisplayClock();break;
	case 1:
	case 2:
	case 3:
	case 4: DisplayAlarm(g_mode - 1);break;
	default:break;
	}
}
	
	
int main(void)
{
    /* INIT SECTION*/
	Init7Seg();
	InitClock();
	InitAlarm();
	InitKeyboard(PressFunc,LongPressFunc);
	IICInit();
	
	
	/*END: INIT SECTION*/
	sei();
	StartDispatcherTimer();

	
	/*Fill task query*/
	AddTask(DisplayAllSeg,0,4);
	AddTask(DisplayMode,0,50);
	AddTask(ScanKeyboard,0,200);
	AddTask(BlinkAllSeg,500,500);
	
	/*END: Fill */
	
	StartShowTime();

	
    while (1) Dispatcher(); 
	
    return 0;
}

