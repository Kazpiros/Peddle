#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU (int)16e6

//TCCR1/2 -> 8bit
volatile uint8_t total_ticks = 0;

// Ensure rate/64 > 255
void timA_8_init(uint16_t rate) // DID I ENABLE INTERRUPTS????
{
    uint8_t compare = F_CPU / (256 * rate) - 1;
    OCR0A = compare; //register to compare to

    DDRB |= (1 << DDB0);
    TCCR0A = (1 << COM0A1); // clear OCOA
    TCCR0B = (1 << CS02); // 256x prescale
    TIMSK0 |= (1 << OCIE0A); // compare match A en
    sei();

    TCNT0 = 0;
}

/* >! NOTE: */
/* there are two comparison registers, can have up to 4 comparison ints */
void timB_8_init(uint16_t rate)
{
    uint8_t compare = F_CPU / (256 * rate) - 1;
    OCR2A = compare; //register to compare to

    DDRB |= (1 << DDB2);
    TCCR2A = (1 << COM0A1); // clear OCOA
    TCCR2B = (1 << CS02); // 256x prescale
    TIMSK2 |= (1 << OCIE0A); // compare match A en
    sei();

    TCNT2 = 0;
}

//Pre-mature Interrupt, returns difference between arg and TCNTn
uint8_t timA_8_PreInt(u_int16_t rate)
{
    PORTB ^= (1<<DDB3);
    uint8_t compare = rate >> 8;
    TIFR0 |= (1 << OCF0A);
    return compare - TCNT0;
}

//Pre-mature Interrupt, returns difference between arg and TCNTn
uint8_t timB_8_PreInt(u_int16_t rate)
{
    PORTB ^= (1<<DDB3); // for configuration
    uint8_t compare = rate >> 8;
    TIFR2 |= (1 << OCF0A);
    return compare - TCNT2;
}