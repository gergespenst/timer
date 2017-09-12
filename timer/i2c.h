/*
 * i2c.h
 *
 * Created: 12.12.2013 12:02:52
 *  Author: 123
 * ‘айл описывает обмен по интерфейсу I2C с пам€тью
 */ 


#ifndef I2C_H_
#define I2C_H_

#ifndef uchar
	typedef unsigned char uchar;
#endif

#include "main.h"
//#######################################################################################################################
//#
//# ‘”Ќ ÷»» ќЅ—Ћ”∆»¬јЌ»я »Ќ“≈–‘≈…—ј I2C (программный вариант)
//#
//#######################################################################################################################

//настройка порта IIC
//#define IICDDR                DDRC					/*порт управлени€ I2C*/
//#define IICPORT               PORTC					/*порт вывода данных на линию I2C*/
//#define IICPIN                PINC					/*порт ввода данных с линий I2C*/
//#define IICDataPin			  4
//#define IICCcPip              5

#define IICData               _BV(IICSDAPin)                /*лини€ DATA*/
#define IICClc                _BV(IICSDLPin)                /*лини€ CLC*/
#define IICPause				5					/*длительность импульсов в мкс*/

void IICInit(void);
void IICStart (void);
void IICStop (void);
char IICByteWrite (char byte);
char IICByteRead (char on_bit);
char IICBit (char byte);

#endif /* I2C_H_ */