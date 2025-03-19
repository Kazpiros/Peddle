/*
 * main.cpp
 *
 * Created: 3/11/2025 6:23:00 PM
 * Author : Kazfp
 *
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#define F_CPU (int)16e6

#include "USART.h"
#include "ADC_START.h"
#include "PWM.h"
#include "Filters.h"

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

int main(void)
{
	//PRR = 0xff; // all power reduction measures off
	USART_init(207);
	PWM_init();
	ADC_0_init();
	
	ADC_0_start_conversion(0);
	register int a = 0;
    while (1) 
    {
		start_timer();
		a = ADC_0_get_conversion(0);
		a = iir_filter_process(a);
		PWM_update(a);
		a = stop_timer();
		USART_Transmit(a); // debug
    }
}

