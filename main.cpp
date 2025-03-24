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


#define F_CPU (int)16e6
#define SAMPLE_RATE 25600U

unsigned int ADC_VAL = 0;
char ADC_VAL_Char[5];

volatile int level = 0;
volatile int envelope = 0;

typedef enum{
	DEFAULT_CONVERSION,
	ENVELOPE_CONVERSION
} ADC_Switch_t;
volatile ADC_Switch_t adc_switch = DEFAULT_CONVERSION;

int main(void)
{
	SREG = (1 << 7); // global enable
	//PRR = 0xff; // all power reduction measures off
	USART_init(9600);
	PWM_init();
	ADC_init();
	ADC_start_conversion(0);

	timA_8_init(SAMPLE_RATE); // sample rate
	timB_8_init(256); // 128th note * 2 (record envelope)

    TCNT0 = 0; // start sample rate timer for debug
	int i = 0;
	while(1)
	{
		switch (adc_switch)
		{
		case DEFAULT_CONVERSION:
			level = ADC_get_conversion(0);
			level = iir_DF2(level, envelope);
			PWM_update(level);
			USART_Transmit(TCNT0); // debug
			break;

		case ENVELOPE_CONVERSION:
			envelope = ADC_get_conversion(1);
			level = iir_DF2(level, envelope);
			PWM_update(level);
			USART_Transmit(TCNT0); // debug
			adc_switch = DEFAULT_CONVERSION;
			break;

		default:
			break;
		} adc_switch:
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

