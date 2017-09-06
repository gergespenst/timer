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
DisplayAllDigits();

}

void Test(){
	SetBlinkDigitPart(1,0xFF);
	CHANGE_BLINK_STATE();
}

void BlinkDigits(){
	BlinkDigitPart(1);
}

void PressFunc(int8_t key){
	SetHLine(key);
	
	};
	
void LongPressFunc(int8_t key){
	if (KEY0 == key )
	{
		
	}
	StartBlinkHours();
	};
	
	
int main(void)
{
    /* INIT SECTION*/
	INIT_BLINK_DIODE();
	Init7Seg();
	InitClock();
	InitKeyboard(PressFunc,LongPressFunc);
	/*END: INIT SECTION*/
	sei();
	StartDispatcherTimer();

	//AddTask(TestBlink,0,2000);
	AddTask(DisplayAllDigits,0,4);
	AddTask(ScanKeyboard,0,200);

	//AddTask(BlinkDigits,0,500);
	
    while (1) Dispatcher(); 
	
    return 0;
}

