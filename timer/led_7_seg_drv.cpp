/*
 * led_7_seg_drv.cpp
 *
 * Created: 05.09.2017 13:23:32
 *  Author: USER
 */ 
#include "led_7_seg_drv.h"
int8_t blink_mask[5] ={0x00,0x00,0x00,0,0},
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

}

void SetDigit( int8_t digit, int8_t value )
{

PORT(CATODE_PORT) = ~_BV(digit - 1);
PORT(ANODE_PORT) = CONVERT_DIGIT(value) & ~(current_blink_mask[digit - 1]) ;
}

void DrawDigits( int8_t dig1,int8_t dig2,int8_t dig3,int8_t dig4,int8_t dig5 )
{
	static int8_t digit = 0;
	digit++;
	switch (digit){
	case 1:SetDigit(1,dig1);break;
	case 2:SetDigit(2,dig2); break;
	case 3:SetDigit(3,dig3); break;
	case 4:SetDigit(4,dig4); break;
	case 5:SetDigit(5,dig5); break;
	default: digit = 0; break;
	}
// 	SetDigit(1,dig1);
// 	_delay_ms(4);
// 	SetDigit(2,dig2);
// 	_delay_ms(4);
// 	SetDigit(3,dig3);
// 	_delay_ms(4);
// 	SetDigit(4,dig4);
// 	_delay_ms(4);
// 	SetDigit(5,dig5);
}

void SetBlinkDigitPart( int8_t digit,int8_t part )
{
	blink_mask[digit - 1] = part;
}

void BlinkDigitPart( int8_t digit )
{
	if (current_blink_mask[digit - 1] == 0x00)
	{
		current_blink_mask[digit - 1] = blink_mask[digit - 1];
	}else
		current_blink_mask[digit - 1] = 0x00;
}
