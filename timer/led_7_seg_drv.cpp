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
static int8_t g_HLine,g_LLine,g_LLineMask;
int8_t	blink_mask[5] ={0x00,0x00,0x00,0,0},
		current_blink_mask[5] = {0,0,0,0,0};
			
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
	0x5E,//d
	0x79,//E
	0x00,//F//вместо F отображаем пустоту для экономии
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
#define CONVERT_MASK(x)  (((x & 0x0F) == 0x0F)?(0xFF):(0x00))

void Init7Seg()
{
	DDR(CATODE_PORT) |= _BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5);
	PORT(CATODE_PORT) |= _BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5);
	
	DDR(ANODE_PORT) |= _BV(ANODEA)|_BV(ANODEB)|_BV(ANODEC)|_BV(ANODED)|_BV(ANODEE)|_BV(ANODEF)|_BV(ANODEG)|_BV(ANODEP);
	PORT(ANODE_PORT) &= ~(_BV(ANODEA)|_BV(ANODEB)|_BV(ANODEC)|_BV(ANODED)|_BV(ANODEE)|_BV(ANODEF)|_BV(ANODEG)|_BV(ANODEP));
		
		SetDigit(0,2,ELEMBLINK);SetDigit(1,3,ELEMON);SetDigit(2,3,ELEMBLINK);SetDigit(3,4,ELEMON);
		SetDots(DOTH,ELEMON);SetDots(DOTL,ELEMBLINK);
		SetHLine(ELEM0|ELEM1|ELEM2|ELEM3|ELEM4,ELEMON);
		SetHLine(ELEM2,ELEMBLINK);
		SetHLine(ELEM0,ELEMOFF);
		SetLLine(ELEM0|ELEM1,ELEMON);
		SetLLine(ELEM2,ELEMBLINK);
}

void SetDigit( int8_t digit, int8_t value){};
	
void SetDigit( int8_t digit, int8_t value, int8_t state)
{
	if ((digit >= 0) && (digit <=3))
	{
		g_indicator.digit[digit/2] = (g_indicator.digit[digit/2] & (0xF0 >> 4*(digit%2))) | (value & 0x0F)<<4*(digit%2);
	switch(state)	{
		case ELEMOFF:{						//обнуляем в зависимости от номера цифры старший или младший полубайт и помещаем туда сдвинутую цифру
				g_indicator.digit[digit/2] = (g_indicator.digit[digit/2] & (0xF0 >> 4*(digit%2))) | (0x0F <<4*(digit%2));
		}break;																				// TODO: Для экономии считаем что F - выключение цифры
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

void DisplayAllDigits()
{
	static int8_t digit = 0;
	
	PORT(CATODE_PORT) = (PORT(CATODE_PORT) | (_BV(CATODE1)|_BV(CATODE2)|_BV(CATODE3)|_BV(CATODE4)|_BV(CATODE5)) ) & ~_BV(digit );
	
	if(g_indicator.blinkState )
		 PORT(ANODE_PORT) = (g_indicator.segments[digit])  ;
	else 	PORT(ANODE_PORT) = (g_indicator.segments[digit]) & ~(g_indicator.segmentsMask[digit]) ;
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

void BlinkAllSeg(){
	(g_indicator.blinkState)?(g_indicator.blinkState = 0):(g_indicator.blinkState = 1);
}

void SetHLine( int8_t elements )
{
	
	g_elements[2].digit = (g_elements[2].digit & (_BV(ANODEP) |(ELEM_DOT))) | elements;
}

void SetHLine(int8_t elements,int8_t state){
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
void SetLLine(int8_t elements,int8_t state){
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

void SetLLine( int8_t elements )
{
	g_elements[0].digit = (g_elements[0].digit & ~_BV(ANODEP)) | ( ((elements & ELEM0) == ELEM0) << ANODEP );
	g_elements[1].digit = (g_elements[1].digit & ~_BV(ANODEP)) | ( ((elements & ELEM1) == ELEM1) << ANODEP );
	g_elements[3].digit = (g_elements[3].digit & ~_BV(ANODEP)) | ( ((elements & ELEM3) == ELEM3) << ANODEP );
	g_elements[4].digit = (g_elements[4].digit & ~_BV(ANODEP)) | ( ((elements & ELEM4) == ELEM4) << ANODEP );
													//здесь сдвиг на один разряд сделан для того чтоб управлять нижней точкой(см. распиновку индикатора)
 	g_elements[2].digit = (g_elements[2].digit & ~(_BV(ANODEP) |(ELEM_DOT<<1))) | ( ((elements & ELEM2) == ELEM2) << ANODEP ) 
															   | ( ((elements & ELEM_DOT) == ELEM_DOT) << 7 );

}


void SetLLineMask(int8_t elements){
	g_elements[0].mask = (g_elements[0].mask & ~_BV(ANODEP)) | ( ((elements & ELEM0) == ELEM0) << ANODEP );
	g_elements[1].mask = (g_elements[1].mask & ~_BV(ANODEP)) | ( ((elements & ELEM1) == ELEM1) << ANODEP );
	g_elements[3].mask = (g_elements[3].mask & ~_BV(ANODEP)) | ( ((elements & ELEM3) == ELEM3) << ANODEP );
	g_elements[4].mask = (g_elements[4].mask & ~_BV(ANODEP)) | ( ((elements & ELEM4) == ELEM4) << ANODEP );
													//здесь сдвиг на один разряд сделан для того чтоб управлять нижней точкой(см. распиновку индикатора)
	g_elements[2].mask = (g_elements[2].mask & ~(_BV(ANODEP) |(ELEM_DOT<<1))) | ( ((elements & ELEM2) == ELEM2) << ANODEP )
															 | ( ((elements & ELEM_DOT) == ELEM_DOT) << 7 );

	
}

void SetLLineElements( int8_t element,int8_t state )
{
	switch(state)	{
		case ELEMOFF:{	g_LLine &= ~element;
						g_LLineMask &= ~element;}break;
		case ELEMON:{	g_LLine |= element;
						g_LLineMask &= ~element;}break;
		case ELEMBLINK:{g_HLine |= element;
						g_LLineMask |= element;
			}break;
		default:break;
	}
	SetLLine(g_LLine);
	SetLLineMask(g_LLineMask);

}

void SetHLineElements( int8_t element,int8_t state )
{
	switch(state)	{
		case ELEMOFF:{	g_HLine &= ~element;
						}break;
		case ELEMON:{	g_HLine |= element;
						}break;
		case ELEMBLINK:{ 
									
		}break;
		default:break;
	}
	SetHLine(g_HLine);

}