#ifndef ADCSTART_HPP
#define ADCSTART_HPP

#include <avr/interrupt.h>
#include <avr/iom328p.h>
//#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <avr/common.h>
#include <stdlib.h>
#include <stdio.h>

int8_t ADC_init();

void ADC_enable();

void ADC_disable();

void ADC_start_conversion(uint8_t channel);

int ADC_is_conversion_done();

uint16_t ADC_get_conversion_result(void);

uint16_t ADC_get_conversion(uint8_t channel);

#endif

