/*
 * PWM.c
 *
 * Created: 23.01.2021 14:28:26
 * Author : MRem
 */ 
#define F_CPU 4800000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t adc;
uint8_t ModeAuto = 0x00;

void CheckButtton(void); //������� ������������ �� ��������� ������� ������
void AutoMode (void); //������� ����� �������������� ����������� �� ���������� ����

int main(void)
{
	DDRB = 0b00001001; //��������� ������
	PORTB |= (1<<PB1);
	
	ADMUX = 0b00100001;	//��������� ���
	ADCSRA = 0b11101010;
	ADCSRB = 0x00;
	
	TCCR0A = 0b11000011; //��������� ��������
	TCCR0B = 0b00000001;
    
	sei(); //���������� ����������
    while (1) 
    {
		CheckButtton();
		AutoMode();		
    }
}

ISR(ADC_vect)
{
	adc = ADCH;	
}

void CheckButtton (void)
{
	if (!(PINB & (1 << PB1)))
	{
		if (ModeAuto == 0x00)
		{
			_delay_ms(40);
			if (PINB & (1 << PB1))
			{
				ModeAuto = 0xff;
			}	
		return;	
		}
		if (ModeAuto == 0xff)
		{
			_delay_ms(40);
			if (PINB & (1 << PB1))
			{
				ModeAuto = 0x00;
			}
		}
	}
	
}

void AutoMode (void)
{
	if (ModeAuto == 0xff)	//��������� ��������������� ������
	{
		PORTB |= (1<<PB3);
		OCR0A = adc;
	}
	else					//��������� 100% ��������
	{
		PORTB &= ~(1<<PB3);	
		OCR0A = 0;
	}
	
}