#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>

#include "USART.hpp"
#include "SPI.hpp"
#include "ADC_START.hpp"
#include "PWM.hpp"
#include "Filters.hpp"
#include "timers.hpp"
#include "circular_buffer.hpp"
#include "config.hpp"
#include "MCP_DAC.hpp"
#include "MCP_ADC.hpp"
#include "generators.cpp"

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

volatile int16_t level = 0;
volatile int16_t envelope = 0;

typedef enum{
	AUTO_WAH_CONVERSION,
	SINE
} ADC_Switch_t;
volatile ADC_Switch_t adc_switch = AUTO_WAH_CONVERSION;

void setup_pin_change_interrupt(void);

int main(void)
{
	SREG = (1 << 7); // global enable

	MCP_DAC DAC0(0);
	MCP_ADC ADC0(0);
	
	setup_pin_change_interrupt();
	// Keep this statically allocated.
	circular_buf filter_buf;
    cbuf_init(&filter_buf); 
	circular_buf envelope_buf;
	cbuf_init(&envelope_buf);
	circular_buf fGenBuf;
	cbuf_init(&fGenBuf);
	
	// Create a sine wave generator at 440 Hz with 80% amplitude
	SineGenerator sineGen(440, (uint16_t)(Q15_MAX * 0.8));
    

	timA_8_init(SAMPLE_RATE); // sample rate
	timB_8_init(256); // 128th note * 2 (record envelope)

    TCNT0 = 0; // start sample rate timer for debug


	while(1)
	{
		switch(adc_switch){
			case AUTO_WAH_CONVERSION: 
			{
				int16_t read_val = ADC0.static_read();
				int16_t envelope = envelope_detector(&envelope_buf,read_val);
				int16_t write_val = iir_DF1(&filter_buf, read_val, envelope);
				DAC0.DAC_write(write_val);
				break;
			}
			case SINE:
			{
				sineGen.start_output(&fGenBuf,DAC0);
				break;
		
			}
		}

	}
}

// External Pin interrupt (PD4)
void setup_pin_change_interrupt(void) {
    DDRD &= ~(1 << PD4);    
    PORTD |= (1 << PD4);    
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);
}

ISR(TIMER0_COMPA_vect)
{
    TCNT0 = 0; // reset timer
	//PORTB ^= (1 << PB0); // signals out
}

ISR(TIMER2_COMPA_vect) 
{
	TCNT2 = 0; // reset timer
	//PORTB ^= (1 << PB0); // signals out
	adc_switch = SINE;
}

// internal/external change Triggered
ISR(PCINT2_vect)
{
	
}
