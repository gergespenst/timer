/*
 * led_7_seg_drv.cpp
 *
 * Created: 05.09.2017 13:23:32
 *  Author: USER
 */ 
#include "led_7_seg_drv.h"

struct SEGDISP {
	uint8_t segments[5];		//физические сегменты
	uint8_t segmentsMask[5];	//маска физических егментов
	uint8_t digit[2];			//цифры (по пол байта на цифру) [1 0] [3 2]
	uint8_t digitMask[2];		//маска для мигания частей цифр (по ол байта на цифру)
	uint8_t HLine;				//Верхняя линия
	uint8_t HLineMask;			//маска верхней линии
	uint8_t LLine;				//нижняя линия
	uint8_t LLineMask;			//маска нижней линии
	uint8_t dots:3;				//точки - 4 бита на точки, но их всего две
	uint8_t dotsMask:3;			//маска дял точек
	uint8_t blinkState:2;
} g_indicator;
		


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
	0x00,//d//вместо d отображаем пустоту для экономии (OFFSYM)
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
#define OFFSYM 0x0D
#define GET_BIT(x,nbit)		(((x)&_BV(nbit)) == _BV(nbit))
#define CONVERT_DIGIT(x) ((GET_BIT(segDigit[(x)],0)<<ANODEA)|(GET_BIT(segDigit[(x)],1)<<ANODEB)|(GET_BIT(segDigit[(x)],2)<<ANODEC)|\
						 (GET_BIT(segDigit[(x)],3)<<ANODED)|(GET_BIT(segDigit[(x)],4)<<ANODEE)|(GET_BIT(segDigit[(x)],5)<<ANODEF)|\
						 (GET_BIT(segDigit[(x)],6)<<ANODEG)|(GET_BIT(segDigit[(x)],7)<<ANODEP))
#define CONVERT_MASK(x)  (((x & 0x0F) == 0x0F)?(0xFF):(0x00))

void Init7Seg()
{
	DDR(CATODE_PORT) |= _BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5);
	PORT(CATODE_PORT) |= _BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5);
	
	DDR(ANODE_PORT) |= _BV(ANODEA)|_BV(ANODEB)|_BV(ANODEC)|_BV(ANODED)|_BV(ANODEE)|_BV(ANODEF)|_BV(ANODEG)|_BV(ANODEP);
	PORT(ANODE_PORT) &= ~(_BV(ANODEA)|_BV(ANODEB)|_BV(ANODEC)|_BV(ANODED)|_BV(ANODEE)|_BV(ANODEF)|_BV(ANODEG)|_BV(ANODEP));
		
		SetDigit(0,0,ELEMON);SetDigit(1,1,ELEMON);SetDigit(2,2,ELEMON);SetDigit(3,3,ELEMON);
}

void SetDigit( int8_t digit, int8_t value){};
	
void SetDigit( int8_t digit, int8_t value, int8_t state)
{
	if ((digit >= 0) && (digit <=3))
	{
		g_indicator.digit[digit/2] = (g_indicator.digit[digit/2] & (0xF0 >> 4*(digit%2))) | (value & 0x0F)<<4*(digit%2);
	switch(state)	{
		case ELEMOFF:{						//обнуляем в зависимости от номера цифры старший или младший полубайт и помещаем туда сдвинутую цифру
				g_indicator.digit[digit/2] = (g_indicator.digit[digit/2] & (0xF0 >> 4*(digit%2))) | (OFFSYM <<4*(digit%2));
		}break;																				// TODO: Для экономии считаем что OFFSYM - выключение цифры
		case ELEMON:{	
				g_indicator.digitMask[digit/2] = (g_indicator.digitMask[digit/2] & (0xF0 >>4*(digit%2)));
		}break;
		case ELEMBLINK:{	
				g_indicator.digitMask[digit/2] =  (g_indicator.digitMask[digit/2] & (0xF0 >>4*(digit%2))) | (0x0F <<4*(digit%2));
		}break;
		default:break;
		}
	}
	UpdateSegments();
	
}

void SetDots(int8_t dots,int8_t state){
	
	g_indicator.dots |= dots;
	switch(state)	{
		case ELEMOFF:{	
			g_indicator.dots &= ~dots;
			}break;
		case ELEMON:{	
			g_indicator.dotsMask &= ~dots;
			}break;
		case ELEMBLINK:{
			g_indicator.dotsMask |= dots;
			}break;
		default:break;
	}
UpdateSegments();
	
}

void UpdateSegments(){
	
	g_indicator.segments[0] =	(CONVERT_DIGIT( (g_indicator.digit[0] & 0x0F)     ) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLine,0));
	g_indicator.segments[1] =	(CONVERT_DIGIT( (g_indicator.digit[0] & 0xF0) >> 4) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLine,1));
	
	g_indicator.segments[2] =	(g_indicator.HLine & ~(_BV(ANODEP) | ELEM_DOT | (ELEM_DOT<<1))) | (_BV(ANODEP)*GET_BIT(g_indicator.LLine,2))
																								| g_indicator.dots << 6;
	g_indicator.segments[3] =	(CONVERT_DIGIT( (g_indicator.digit[1] & 0x0F)     ) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLine,3));
	g_indicator.segments[4] =	(CONVERT_DIGIT( (g_indicator.digit[1] & 0xF0) >> 4) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLine,4));
	
	g_indicator.segmentsMask[0] = (CONVERT_MASK( g_indicator.digitMask[0] & 0x0F       ) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLineMask,0));
	g_indicator.segmentsMask[1] = (CONVERT_MASK( (g_indicator.digitMask[0] & 0xF0) >> 4) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLineMask,1));
 	g_indicator.segmentsMask[2] = ( g_indicator.HLineMask &   ~(_BV(ANODEP) | ELEM_DOT | (ELEM_DOT<<1))) | (_BV(ANODEP)*GET_BIT(g_indicator.LLineMask,2))
																									 | g_indicator.dotsMask << 6;
	g_indicator.segmentsMask[3] = (CONVERT_MASK(  g_indicator.digitMask[1] & 0x0F      ) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLineMask,3));
	g_indicator.segmentsMask[4] = (CONVERT_MASK( (g_indicator.digitMask[1] & 0xF0) >> 4) & ~_BV(ANODEP)) | (_BV(ANODEP)*GET_BIT(g_indicator.LLineMask,4));
	
}

void SetSegmentRaw( int8_t segment,int8_t rawVal )
{
	g_indicator.segments[segment] = rawVal;
}

void DisplayAllSeg()
{
	static int8_t digit = 0;
	
	PORT(CATODE_PORT) = (PORT(CATODE_PORT) | (_BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5)) ) & ~_BV(digit );
	
	if(g_indicator.blinkState )
		 PORT(ANODE_PORT) = (g_indicator.segments[digit])  ;
	else 	PORT(ANODE_PORT) = (g_indicator.segments[digit]) & ~(g_indicator.segmentsMask[digit]) ;
	(digit < 5)?(digit++):(digit = 0);
}


void BlinkAllSeg(){
	(g_indicator.blinkState)?(g_indicator.blinkState = 0):(g_indicator.blinkState = 1);
}


void SetHLineElements(int8_t elements,int8_t state){
	g_indicator.HLine |= elements;
		switch(state)	{
			case ELEMOFF:{
				g_indicator.HLine &= ~elements;
			}break;
			case ELEMON:{
				g_indicator.HLineMask &= ~elements;
			}break;
			case ELEMBLINK:{
				g_indicator.HLineMask |= elements;
			}break;
			default:break;
		}
	UpdateSegments();
}
void SetLLineElements(int8_t elements,int8_t state){
	g_indicator.LLine |= elements;
	switch(state)	{
		case ELEMOFF:{
			g_indicator.LLine &= ~elements;
		}break;
		case ELEMON:{
			g_indicator.LLineMask &= ~elements;
		}break;
		case ELEMBLINK:{
			g_indicator.LLineMask |= elements;
		}break;
		default:break;
	}
	UpdateSegments();
}

