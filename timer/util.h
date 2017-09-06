/*
 * util.h
 *
 * Created: 05.09.2017 13:24:20
 *  Author: USER
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#define _NBV(x)		(0 << (x))
#define PIN(x)		(*(&x - 2)) // Address Of Data Direction Register Of Port x
#define DDR(x)		(*(&x - 1)) // Address Of Input Register Of Port x
#define PORT(x)		(x)



#endif /* UTIL_H_ */