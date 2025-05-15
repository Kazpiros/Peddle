#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h> // can i even use this?

#include "timers.hpp"

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
// lazy delayer, can only be max 256. ~ duration*16us = time
void delay_us(uint16_t us) {
    while (us--) {
        _delay_us(1);  // delay a single microsecond in each loop
    }
}
//Pre-mature Interrupt, returns difference between arg and TCNTn
uint8_t timA_8_PreInt(uint16_t rate)
{
    uint8_t compare = rate >> 8;
    TIFR0 |= (1 << OCF0A);
    return compare - TCNT0;
}

//Pre-mature Interrupt, returns difference between arg and TCNTn
uint8_t timB_8_PreInt(uint16_t rate)
{
    uint8_t compare = rate >> 8;
    TIFR2 |= (1 << OCF0A);
    return compare - TCNT2;
}