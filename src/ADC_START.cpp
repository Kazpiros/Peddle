#include <avr/interrupt.h>
#include <avr/iom328p.h>
//#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <avr/common.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/ADC_START.hpp"

int8_t ADC_init()
{

	PRR &= ~(1 << PRADC);
	// 1.1V Vref
	ADMUX = (0x03 << REFS0) | (0 << ADLAR);//
	ADCSRA = (1 << ADEN) | (0 << ADATE) | (0 << ADIE) | (0x04 << ADPS0);
	ADCSRB = (0x00 << ADTS0)   | (0 << ACME);

	return 0;
}

void ADC_enable()
{
	ADCSRA |= (1 << ADEN);
}

void ADC_disable()
{
	ADCSRA &= ~(1 << ADEN);
}

void ADC_start_conversion(uint8_t channel)
{
	ADMUX &= ~0x0F;
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC);
}

int ADC_is_conversion_done()
{
	return ((ADCSRA & (1 << ADIF)));
}

uint16_t ADC_get_conversion_result(void)
{
	return (ADCL | ADCH << 8);
}

uint16_t ADC_get_conversion(uint8_t channel)
{
	uint16_t res;

	ADC_start_conversion(channel);
	while (!ADC_is_conversion_done());
	res = ADC_get_conversion_result();
	ADCSRA |= (1 << ADIF);
	return res;
}

