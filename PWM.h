#define F_CPU (int)16e6

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t total_ticks = 0;

ISR(TIMER0_OVF_vect)
{
	total_ticks += 256;
}

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

unsigned int TIM16_ReadTCNT1(void)
{
	unsigned char sreg;
	unsigned int i;
	sreg = SREG; // interrupt flag
	cli(); //disable interrupts
	i = TCNT1;
	SREG = sreg;
	return i;
}

void TIM16_WriteTCNT1(unsigned int i)
{
	unsigned char sreg;
	sreg = SREG; // interrupt flag
	cli(); //disable interrupts
	TCNT1 = i;
	SREG = sreg;
}
//start 8 bit tim0
void start_timer(void)
{
	cli();
	total_ticks = 0;
	TCNT0 = 0; //reset count
	sei();
	
	TCCR0A = 0;
	TCCR0B = (1 << CS01); // 8x prescaler, worst case scenario 8 cc error.
	TIMSK0 |= (1 << TOIE0);
	// 8* 256 = 65k cc of time
}
uint16_t stop_timer(void)
{
	uint8_t ticks;
	TIMSK0 &= ~(1 << TOIE0);
	TCCR0B = 0;
	
	cli();
	ticks = (total_ticks + TCNT1) * 8; 
	sei();
	return ticks;
}

// go to 93 page for pwm stuff
