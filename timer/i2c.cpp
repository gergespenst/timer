/*
 * i2c.c
 *
 * Created: 19.09.2014 13:47:26
 *  Author: USER
 */ 
#include "i2c.h"
//������������� ����� ��� IIC

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

//������� ������������ ��������� START �� ���������� IIC
void IICStart (void)
{
	IICDDR &= ~(IICData | IICClc);
	_delay_us (IICPause);
	IICDDR |= IICData;                            //�������� data � ����
	_delay_us (IICPause);
	IICDDR |= IICClc;                            //�������� clc � ����
	_delay_us (IICPause);
}



//������� ������������ ��������� STOP �� ���������� IIC
void IICStop (void)
{
	IICDDR |= (IICData | IICClc);
	_delay_us (IICPause);
	IICDDR &= ~IICClc;                            //�������� data � ����
	_delay_us (IICPause);
	IICDDR &= ~IICData;                            //�������� clc � ����
	_delay_us (IICPause);
}


//������� ������ ����� �� ���������� IIC
//�������� - ���� ������ ��� ������ �� IIC
//�������� - ���� ������������� 0 (���� �������������) ��� 1 (��� �������������)
char IICByteWrite (char byte)
{
	char i;
	for (i = 0; i<8; i++)
	byte = IICBit (byte);
	byte = 0x80;
	return IICBit (byte);                        //���������� ��� ������������� (0-���� ��� �������������, 1-���)
}


//������� ������ ����� �� ���������� IIC
//�������� - �������� ���� ������������� (0 - ���� �������������, 1 - ��� �������������)
//�������� - �������� ����
char IICByteRead(char on_bit)
{char i;
	char byte = 0xFF;
	for ( i = 0; i<8; i++)
	byte = IICBit (byte);
	on_bit <<= 7;
	IICBit (on_bit);                                //�������� �������� ���� �������������
	return byte;                                //���������� �������� ����
}


//������� ������\������ ���� �� ���������� IIC
//��������� �������� �������� ���� ���������
//�������� ��� ������������ � ������� ���� �������������� ���������� ����� ���������
char IICBit (char byte)
{char temp = 0;
	if (byte & 0x80)                        //��������� ���
	IICDDR &= ~IICData;                 //����� 1 �� ����� ������
	else
	IICDDR |= IICData;                    //����� 0 �� ����� ������
	_delay_us (IICPause);
	IICDDR &= ~IICClc;                        //����� ����� ��������
		
		while (! (IICPIN & IICClc)){//�������� ������������ ������
			temp++;				//���� ������� ����������� �������
			if (temp == 0)			//���� �������� 255 ���� �� ���������� �����
			break;
			_delay_us(IICPause);
		}
	_delay_us (IICPause);
	
	if (IICPIN & IICData)                    //��������� ���
	temp = 1;
	else
	temp = 0;
	IICDDR |= IICClc;                        //��������� �����
	byte <<= 1;
	byte |= temp;
	return byte;
}
