/*
 * i2c.h
 *
 * Created: 12.12.2013 12:02:52
 *  Author: 123
 * ���� ��������� ����� �� ���������� I2C � �������
 */ 


#ifndef I2C_H_
#define I2C_H_

#ifndef uchar
	typedef unsigned char uchar;
#endif

#include "main.h"
//#######################################################################################################################
//#
//# ������� ������������ ���������� I2C (����������� �������)
//#
//#######################################################################################################################

//��������� ����� IIC
//#define IICDDR                DDRC					/*���� ���������� I2C*/
//#define IICPORT               PORTC					/*���� ������ ������ �� ����� I2C*/
//#define IICPIN                PINC					/*���� ����� ������ � ����� I2C*/
//#define IICDataPin			  4
//#define IICCcPip              5

#define IICData               _BV(IICSDAPin)                /*����� DATA*/
#define IICClc                _BV(IICSDLPin)                /*����� CLC*/
#define IICPause				5					/*������������ ��������� � ���*/

void IICInit(void);
void IICStart (void);
void IICStop (void);
char IICByteWrite (char byte);
char IICByteRead (char on_bit);
char IICBit (char byte);

#endif /* I2C_H_ */