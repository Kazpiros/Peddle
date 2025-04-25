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

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

volatile int16_t level = 0;
volatile int16_t envelope = 0;

typedef enum{
	DEFAULT_CONVERSION,
	ENVELOPE_CONVERSION
} ADC_Switch_t;
volatile ADC_Switch_t adc_switch = DEFAULT_CONVERSION;

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

	circular_buf input_buf;
	cbuf_init(&input_buf); 
	
	//USART_init(115200);
	MSPI_Init(); // sck = clk/128
	PWM_init();
	ADC_init();
	ADC_start_conversion(0);

	timA_8_init(SAMPLE_RATE); // sample rate
	timB_8_init(256); // 128th note * 2 (record envelope)

    TCNT0 = 0; // start sample rate timer for debug
	while(1)
	{
		//PORTB ^= (1 << PB0); // signals out
		switch (adc_switch)
		{
		case DEFAULT_CONVERSION:
			level = ADC0.static_read();
			level = iir_DF1(&filter_buf, level, envelope);
			PWM_update(level);
			//MSPI_Transmit(TCNT0);
			DAC0.DAC_write(TCNT0);
			break;

		case ENVELOPE_CONVERSION:
			envelope = ADC_get_conversion(1);
			level = iir_DF1(&filter_buf, level, envelope);
			PWM_update(level);
			//MSPI_Transmit(0xFF); // to show that envelopes running
			DAC0.DAC_write(TCNT0);
			adc_switch = DEFAULT_CONVERSION;
			break;

		default:
			break;
		}
	}
}

// HVC Pin interrupt (PD4)
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
	adc_switch = ENVELOPE_CONVERSION;
}

// internal/external change Triggered
ISR(PCINT2_vect)
{
	
}
