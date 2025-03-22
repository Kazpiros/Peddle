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
 #define SAMPLE_RATE (int)22e3
 #include "USART.h"
 #include "ADC_START.h"
 #include "PWM.h"
 #include "Filters.h"
 #include "filter_coefs.h"
 
 unsigned int ADC_VAL = 0;
 char ADC_VAL_Char[5];
 static int8_t rate = (F_CPU/SAMPLE_RATE) / 64;

 int main(void)
 {
     //PRR = 0xff; // all power reduction measures off
     USART_init(207);
     PWM_init();
     ADC_0_init();
     
     ADC_0_start_conversion(0);
     register int adc_val = 0;
     register int output = 0;

     start_sampling(rate);
     while (1) 
     {
         adc_val = ADC_0_get_conversion(0);
         output = iir_DF2(adc_val);
         PWM_update(output);
         while( (TIFR0 & (1<<OCF0A)) < rate ); 
     }
 }
 
