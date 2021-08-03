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

void CheckButtton(void); //Функция отслеживания та обработки нажатия кнопки
void AutoMode (void); //Функция смены автоматический регулировки на постоянный свет

int main(void)
{
	DDRB = 0b00001001; //Настройка портов
	PORTB |= (1<<PB1);
	
	ADMUX = 0b00100001;	//Настройка АЦП
	ADCSRA = 0b11101010;
	ADCSRB = 0x00;
	
	TCCR0A = 0b11000011; //Настройка таймеров
	TCCR0B = 0b00000001;
    
	sei(); //Разрешение прерываний
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
	if (ModeAuto == 0xff)	//Включение автоматического режима
	{
		PORTB |= (1<<PB3);
		OCR0A = adc;
	}
	else					//Включение 100% свечения
	{
		PORTB &= ~(1<<PB3);	
		OCR0A = 0;
	}
	
}