/*
 * ADC.cpp
 *
 * Created: 3/11/2025 6:23:00 PM
 * Author : Kazfp
 *
 *
 * helpful links "how to use adc in avr micro controllers"
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU (int)16e6

#include <util/delay.h>
#include "USART.h"



// ADC module 24.9
void ADC_init(void)
{
	// --- 24.9.1 ADC MUX selection, 
	//Vcc external ref | ADLAR-def. order | Mux0 (ADC0)
	ADMUX = (1 << MUX0); 
	
	//---  ADCSRA (also where interrupts hide)
	//ADC-EN | pre-scaler
	ADCSRA = (1 << ADEN) | (1 << ADPS2);
}

// reads into a 16 bit val
uint16_t ADC_Read(void) 
{
	ADCSRA |= (1 << 6); //start
	while( (ADCSRA & (1 << 6)) == 1 );
	//uint8_t adcl = ADCL; //read 
	//uint8_t adch = ADCH; //read  
	// these probably take a lot of time to do, check if compiler rids them
	uint16_t val = ((ADCH << 8) | ADCL) & 0x3FF; 
	return val;
}

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

int main(void)
{
	// look into debug mode, using a button to step thru code.
	PRR = 0xef; // all power reduction measures off
	USART_init(9600);
	ADC_init();
    while (1) 
    {
		 // " / 4" to reduce accuracy to fit into UART
		ADC_VAL = ADC_Read()  >> 2;
		USART_Transmit(ADC_VAL);
		_delay_ms(200);
    }
}

