/*
 * main.cpp
 *
 * Created: 3/11/2025 6:23:00 PM
 * Author : Kazfp
 *
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU (int)16e6

/* making my own delay functions later */
//#include <util/delay.h> 
#include "USART.h"
#include "ADC.h"
#include "PWM.h"
#include "circular_buffer.h"
#include "filters.h"

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

int main(void)
{
	//PRR = 0xff; // all power reduction measures off
	USART_init(207);
	PWM_init();
	ADC_0_init();
	
	ADC_0_start_conversion(0);
	register uint16_t adc_output = 0;
	volatile int timer_temp = 0;
    while (1) 
    {
		start_timer(); //start measuring execution time (rough estimate)
	    
		adc_output = ADC_0_get_conversion(0); //read
	    	adc_output = iir_filter_process(adc_output); //filter
		PWM_update(adc_output); //output
	    
		timer_temp = stop_timer(timer_temp); //end measure
		USART_Transmit(a); // transmit debug
    }
}

