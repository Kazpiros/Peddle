#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"
//https://electronics.stackexchange.com/questions/644461/how-to-send-16-bit-data-to-mcp4802-from-atmega328p-arduino-nano
//REDO SPI 
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
void USART_MSPI_Init(void);

inline void USART_MSPI_Transmit(char cData);

// SlaveSPI_init -> MISO
void USART_SSPI_init(void);

inline char USART_SSPI_recieve(void);

inline char USART_MSPI_recieve(void);

#endif