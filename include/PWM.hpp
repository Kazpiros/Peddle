#ifndef PWM_HPP
#define PWM_HPP

#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/interrupt.h>
#include <avr/common.h>

#include <stdio.h>
#include <stdlib.h>


#define F_CPU (int)16e6


void PWM_init(void);

void PWM_update(unsigned int top_val);

#endif