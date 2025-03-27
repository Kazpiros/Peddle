#ifndef TIMERS_HPP
#define TIMERS_HPP

#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/interrupt.h>
#include <avr/common.h>

#include <stdio.h>
#include <stdlib.h>

#define F_CPU (int)16e6

volatile uint8_t total_ticks = 0;

void timA_8_init(uint16_t rate);

void timB_8_init(uint16_t rate);

uint8_t timA_8_PreInt(u_int16_t rate);

uint8_t timB_8_PreInt(u_int16_t rate);

#endif