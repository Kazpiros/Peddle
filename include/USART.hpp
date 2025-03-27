#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void USART_init(int BAUD);

void USART_Transmit(unsigned int data);

unsigned char USART_Receive( void );

void USART_Flush( void );

#endif