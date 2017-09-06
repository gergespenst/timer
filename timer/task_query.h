/* 
* task_query.h
*
* Created: 28.10.2015 21:04:35
* Author: Nik
*/


#ifndef __TASK_QUERY_H__
#define __TASK_QUERY_H__

#define MAX_TASKS 10

#include <avr/interrupt.h>

#define ENABLE_INTERRUPT() sei()
#define DISABLE_INTERRUPT() cli()

typedef void (*TPTR)(void);

extern void Idle();


volatile static struct TTask{
	TPTR task;
	unsigned int start_delay;
	unsigned int  period;
	char run;
	} TaskQueue[MAX_TASKS];
	


void AddTask(TPTR AddingTask, unsigned int  delay, unsigned int  period);
void TimerProcess();// Необходимо вызывать данную функцию по таймеру, задержка и период задаются в количестве периодов вызова данной функции
void Dispatcher();
void DeleteTask(TPTR delTask);
   
#endif //__TASK_QUERY_H__
