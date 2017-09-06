/*
 * led_7_seg_drv.cpp
 *
 * Created: 05.09.2017 13:23:32
 *  Author: USER
 */ 
#include "led_7_seg_drv.h"
struct ELEMENT {
	int8_t mask;
	int8_t digit;
	bool digitOn;
	};
ELEMENT g_elements[5];
int8_t	blink_mask[5] ={0x00,0x00,0x00,0,0},
		current_blink_mask[5] = {0,0,0,0,0};
		


int8_t segDigit[] = {
	0x3F,//0
	0x06,//1
	0x5B,//2
	0x4F,//3
	0x66,//4
	0x6D,//5
	0x7D,//6
	0x07,//7
	0x7F,//8
	0x6F,//9
	0x77,//A
	0x7C,//b
	0x39,//C
	0x5E,//d
	0x79,//E
	0x71,//F
	0x54,//n
	0x5C,//o
	0x73,//P
	0x50,//r
	0x78,//t
	0x1C,//u
	0x00	
};

#define GET_BIT(x,nbit)		(((x)&_BV(nbit)) == _BV(nbit))
#define CONVERT_DIGIT(x) ((GET_BIT(segDigit[(x)],0)<<ANODEA)|(GET_BIT(segDigit[(x)],1)<<ANODEB)|(GET_BIT(segDigit[(x)],2)<<ANODEC)|\
						 (GET_BIT(segDigit[(x)],3)<<ANODED)|(GET_BIT(segDigit[(x)],4)<<ANODEE)|(GET_BIT(segDigit[(x)],5)<<ANODEF)|\
						 (GET_BIT(segDigit[(x)],6)<<ANODEG)|(GET_BIT(segDigit[(x)],7)<<ANODEP))

void Init7Seg()
{
	DDR(CATODE_PORT) |= _BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5);
	PORT(CATODE_PORT) |= _BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5);
	
	DDR(ANODE_PORT) |= _BV(ANODEA)|_BV(ANODEB)|_BV(ANODEC)|_BV(ANODED)|_BV(ANODEE)|_BV(ANODEF)|_BV(ANODEG)|_BV(ANODEP);
	PORT(ANODE_PORT) &= ~(_BV(ANODEA)|_BV(ANODEB)|_BV(ANODEC)|_BV(ANODED)|_BV(ANODEE)|_BV(ANODEF)|_BV(ANODEG)|_BV(ANODEP));
		
		SetDigit(0,1);SetDigit(1,16);SetDigit(3,1);SetDigit(4,20);//Выводим корявенькое InIt
}

void SetDigit( int8_t digit, int8_t value )
{
g_elements[digit].digit = (g_elements[digit].digit & _BV(ANODEP)) | CONVERT_DIGIT(value);
}

void DisplayAllDigits()
{
	static int8_t digit = 0;
	
	PORT(CATODE_PORT) = (PORT(CATODE_PORT) | (_BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5)) ) & ~_BV(digit );
	
	if(g_elements[digit].digitOn )
		 PORT(ANODE_PORT) = (g_elements[digit].digit)  ;
	else PORT(ANODE_PORT) = (g_elements[digit].digit) & ~(g_elements[digit].mask) ;
	(digit < 5)?(digit++):(digit = 0);
}

void SetBlinkDigitPart( int8_t digit,int8_t part )
{
	g_elements[digit].mask = part;
}

void BlinkDigitPart( int8_t digit )
{
	(g_elements[digit].digitOn)?(g_elements[digit].digitOn = 0):(g_elements[digit].digitOn = 1);
}

void SetHLine( int8_t elements )
{
	g_elements[2].digit = (g_elements[2].digit & (_BV(ANODEP) |(ELEM_DOT))) | elements;
}

void SetLLine( int8_t elements )
{
	g_elements[0].digit = (g_elements[0].digit & ~_BV(ANODEP)) | ( ((elements & ELEM0) == ELEM0) << ANODEP );
	g_elements[1].digit = (g_elements[1].digit & ~_BV(ANODEP)) | ( ((elements & ELEM1) == ELEM1) << ANODEP );
	g_elements[3].digit = (g_elements[3].digit & ~_BV(ANODEP)) | ( ((elements & ELEM3) == ELEM3) << ANODEP );
	g_elements[4].digit = (g_elements[4].digit & ~_BV(ANODEP)) | ( ((elements & ELEM4) == ELEM4) << ANODEP );
 	g_elements[2].digit = (g_elements[2].digit & ~(_BV(ANODEP) |(ELEM_DOT))) | ( ((elements & ELEM2) == ELEM2) << ANODEP ) 
															   | ( ((elements & ELEM_DOT) == ELEM_DOT) << 7 );

}
