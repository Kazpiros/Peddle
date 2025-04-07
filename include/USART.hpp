#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"

void USART_init(int BAUD);

void USART_Transmit(unsigned int data);

unsigned char USART_Receive( void );

void USART_Flush( void );

/* important SPI info - (SS)'
    In slave mode:
@ pin "SS" held low, MISO becomes OUTPUT, rest inputs -> SPI activated
@ pin "SS" held high -> will not receieve data (acts as a reset pin too)

    In master mode:
@ pin "SS" set 'output' -> no effect
@ pin "SS" set 'input' - HOLD HIGH to enable

SS high is "act as master", SS low "act as slave"
*/

// SPSR is status reg, dont forget
// MasterSPI_Init -> MOSI
void MSPI_Init(void);

void MSPI_Transmit(char cData);

// SlaveSPI_init -> MISO
void SSPI_init(void);

char SSPI_recieve(void);

#endif