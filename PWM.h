#define F_CPU (int)16e6

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t total_ticks = 0;

//ISR(TIMER0_COMPA_VECT) // no routines yet...
//{
//	TCNT0 = 0; //reset count
//}

void PWM_init(void)
{
	DDRB |= (1 << DDB1); // pwm output
	
	//OC0A //too?
	// phase correct, 10 bit
	TCCR1A = (1 << COM1A1);
	TCCR1B = (1 << WGM13) | (1 << CS10); // (1 << ICNC1) noise canceller (4 cycle delay)
	
}

void PWM_update(unsigned int top_val)
{
	ICR1 = top_val;
	OCR1A = top_val >> 1;// duty cycle (50%)
}

void start_sampling(int rate)
{
    
	cli();
	total_ticks = 0;
	TCNT0 = 0; //reset count
	sei();
	// CTC MODE (WGM02:0 = 2) (clear on match)
	// TCNT0 == OCR0A (top val of counter)
	TCCR0A = (1 << COM0A1); //clear ocr0a on  match
	TCCR0B = (1 << CS01) | (1 << CS00) | (1 << WGM01); // 64x prescaler, CTC en
    OCR0A = (F_CPU/rate) / 16
}// go to 93 page for pwm stuff
