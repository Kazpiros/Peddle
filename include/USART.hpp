#ifndef USART_HPP
#define USART_HPP

#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU (int)16e6

void USART_init(int BAUD);

void USART_Transmit(unsigned int data);

unsigned char USART_Receive( void );

void USART_Flush( void );

#endif