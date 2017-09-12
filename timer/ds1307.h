/* 
* ds1307.h
*
* Created: 11.09.2017 13:44:08
* Author: USER
*/


#ifndef __DS1307_H__
#define __DS1307_H__
#include "i2c.h"
#define DS1307ADDR 0b11010000
#define SECREG		0x00
#define MINREG		0x01
#define HOURREG		0x02
#define DAYREG		0x03
#define DATEREG		0x04
#define MONTHREG	0x05
#define YEARREG		0x06
#define CONTROLREG	0x07
#define STARTRAM	0x08
#define ENDRAM		0x3F
#define CLOCHALT	0x80

uint8_t DS1307Read(uint8_t addr,uint8_t * data);
uint8_t DS1307Write(uint8_t addr,uint8_t data);

#endif //__DS1307_H__
