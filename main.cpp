#include <avr/interrupt.h>
#include <avr/iom328p.h>
//#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <stdio.h>
#include <stdlib.h>

#include "USART.h"
#include "ADC_START.h"
#include "PWM.h"
#include "Filters.h"
#include "timers.h"
#include "circular_buffer.h"

#define F_CPU (int)16e6
#define SAMPLE_RATE 25600U

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

volatile int16_t level = 0;
volatile int16_t envelope = 0;

circular_buf filter_buf_instance = { 
    .buffer = {0},      
    .readIndex = 0,     
    .writeIndex = 0     
};

circular_buf* filter_buf = &filter_buf_instance;

typedef enum{
	DEFAULT_CONVERSION,
	ENVELOPE_CONVERSION
} ADC_Switch_t;
volatile ADC_Switch_t adc_switch = DEFAULT_CONVERSION;

int main(void)
{
	SREG = (1 << 7); // global enable
	//PRR = 0xff; // all power reduction measures off
	USART_init(115200);
	PWM_init();
	ADC_init();
	ADC_start_conversion(0);
	//cbuf_init(filter_buf);

	timA_8_init(SAMPLE_RATE); // sample rate
	timB_8_init(256); // 128th note * 2 (record envelope)

    TCNT0 = 0; // start sample rate timer for debug
	while(1)
	{
		switch (adc_switch)
		{
		case DEFAULT_CONVERSION:
			level = ADC_get_conversion(0);
			level = iir_DF1(filter_buf, level, envelope);
			PWM_update(level);
			USART_Transmit(TCNT0); 	 // debug
			break;

		case ENVELOPE_CONVERSION:
			envelope = ADC_get_conversion(1);
			level = iir_DF1(filter_buf, level, envelope);
			PWM_update(level);
			//USART_Transmit(TCNT0); // debug
			PORTB ^= (1<<DDB2); 	 // debug
			adc_switch = DEFAULT_CONVERSION;
			break;

		default:
			break;
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
    TCNT0 = 0; // reset timer
	PORTB ^= (1<<DDB0); // signals out
}

ISR(TIMER2_COMPA_vect) 
{
	TCNT2 = 0; // reset timer
	PORTB ^= (1<<DDB2); // signals out
	adc_switch = ENVELOPE_CONVERSION;
}
