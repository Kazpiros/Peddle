#ifndef PWM_HPP
#define PWM_HPP

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

void PWM_init(void);

void PWM_update(unsigned int top_val);

#endif