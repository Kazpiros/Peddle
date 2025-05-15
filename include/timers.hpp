#ifndef TIMERS_HPP
#define TIMERS_HPP

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"

void timA_8_init(uint16_t rate);

void timB_8_init(uint16_t rate);

uint8_t timA_8_PreInt(uint16_t rate);

uint8_t timB_8_PreInt(uint16_t rate);

void delay_us(uint16_t us);

#endif