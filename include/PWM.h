#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <stdio.h>
#include <stdlib.h>


#define F_CPU (int)16e6

//pwm on 16 bit stream
void PWM_init(void)
{
	DDRB |= (1 << DDB1); // pwm output
	TCCR1A = (1 << COM1A1);
	TCCR1B = (1 << WGM13) | (1 << CS10); // (1 << ICNC1) noise canceller (4 cycle delay)
	
}
void PWM_update(unsigned int top_val)
{
	ICR1 = top_val;
	OCR1A = top_val >> 1;// duty cycle (50%)
}