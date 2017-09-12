/*
 * main.h
 *
 * Created: 11.09.2017 12:41:36
 *  Author: USER
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#include "util.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <stdint.h>
//настройка порта IIC

#define IICPORT               PORTE				/*порт вывода данных на линию I2C*/
#define IICDDR                DDR(IICPORT)					/*порт управления I2C*/
#define IICPIN                PIN(IICPORT)					/*порт ввода данных с линий I2C*/
#define IICSDAPin			  2
#define IICSDLPin              1



#endif /* MAIN_H_ */