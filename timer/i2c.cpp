/*
 * i2c.c
 *
 * Created: 19.09.2014 13:47:26
 *  Author: USER
 */ 
#include "i2c.h"
//»Ќ»÷»јЋ»«ј÷»я ѕќ–“ј ƒЋя IIC

void IICInit(){
	IICPORT &= ~_BV(IICSDAPin);
	IICPORT &= ~_BV(IICSDLPin);
	IICDDR |= _BV(IICSDAPin);
	IICDDR |= _BV(IICSDLPin);
	IICDDR &= ~_BV(IICSDAPin);
	IICDDR &= ~_BV(IICSDLPin);
	//IICPORT = 0x00;
	//IICDDR = 0x00;
}

//‘”Ќ ÷»я ‘ќ–ћ»–ќ¬јЌ»я —ќ—“ќяЌ»я START ѕќ »Ќ“≈–‘≈…—” IIC
void IICStart (void)
{
	IICDDR &= ~(IICData | IICClc);
	_delay_us (IICPause);
	IICDDR |= IICData;                            //подт€жка data к нулю
	_delay_us (IICPause);
	IICDDR |= IICClc;                            //подт€жка clc к нулю
	_delay_us (IICPause);
}



//‘”Ќ ÷»я ‘ќ–ћ»–ќ¬јЌ»я —ќ—“ќяЌ»я STOP ѕќ »Ќ“≈–‘≈…—” IIC
void IICStop (void)
{
	IICDDR |= (IICData | IICClc);
	_delay_us (IICPause);
	IICDDR &= ~IICClc;                            //подт€жка data к нулю
	_delay_us (IICPause);
	IICDDR &= ~IICData;                            //подт€жка clc к нулю
	_delay_us (IICPause);
}


//‘”Ќ ÷»я «јѕ»—» Ѕј…“ј ѕќ »Ќ“≈–‘≈…—” IIC
//ј–√”ћ≈Ќ“ - байт данных дл€ вывода по IIC
//«Ќј„≈Ќ»≈ - флаг подтверждени€ 0 (есть подтверждение) или 1 (нет подтверждени€)
char IICByteWrite (char byte)
{
	char i;
	for (i = 0; i<8; i++)
	byte = IICBit (byte);
	byte = 0x80;
	return IICBit (byte);                        //возвращаем бит подтверждени€ (0-есть бит подтверждени€, 1-нет)
}


//‘”Ќ ÷»я „“≈Ќ»я Ѕј…“ј ѕќ »Ќ“≈–‘≈…—” IIC
//ј–√”ћ≈Ќ“ - значение бита подтверждени€ (0 - есть подтверждение, 1 - нет подтверждени€)
//«Ќј„≈Ќ»≈ - прин€тый байт
char IICByteRead(char on_bit)
{char i;
	char byte = 0xFF;
	for ( i = 0; i<8; i++)
	byte = IICBit (byte);
	on_bit <<= 7;
	IICBit (on_bit);                                //передать значение бита подтверждени€
	return byte;                                //возвращаем прин€тый байт
}


//‘”Ќ ÷»я «јѕ»—»\„“≈Ќ»я Ѕ»“ј ѕќ »Ќ“≈–‘≈…—” IIC
//выводитс€ значение старшего бита аргумента
//прин€тый бит возвращаетс€ в младшем бите предварительно сдвинутого влево аргумента
char IICBit (char byte)
{char temp = 0;
	if (byte & 0x80)                        //выводимый бит
	IICDDR &= ~IICData;                 //вывод 1 на линию данных
	else
	IICDDR |= IICData;                    //вывод 0 на линию данных
	_delay_us (IICPause);
	IICDDR &= ~IICClc;                        //старт строб импульса
		
		while (! (IICPIN & IICClc)){//ожидание формировани€ строба
			temp++;				//пока ожидаем увеличиваем счетчик
			if (temp == 0)			//если прождали 255 пауз то прекращаем ждать
			break;
			_delay_us(IICPause);
		}
	_delay_us (IICPause);
	
	if (IICPIN & IICData)                    //принимаем бит
	temp = 1;
	else
	temp = 0;
	IICDDR |= IICClc;                        //завершить строб
	byte <<= 1;
	byte |= temp;
	return byte;
}
