#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "SPI.hpp"
#include "config.hpp"
//https://electronics.stackexchange.com/questions/644461/how-to-send-16-bit-data-to-mcp4802-from-atmega328p-arduino-nano
//REDO SPI 

void MSPI_Init(void) // gotta enable MISO too!
{
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2); // : MOSI, SCK, SS output
	PORTB |= (1 << PB2); // deselect all slaves
	// Enable SPI, MASTER, SCK = F_CPU/16
	SPCR = (1 << SPIE) | (1 << SPE) | (1 << MSTR)
		 | (0 << SPR0) | (1 << DORD)| (1 << CPOL)  // lsb first out dord = 1
         | (0 << CPHA);
    
}

inline void MSPI_Transmit(char cData)
{
	SPDR = cData; // start tx
	while(!(SPSR & (1 << SPIF)));
	//PORTB |= (1 << PB2); // SS high - deselect slave
}

inline uint16_t MSPI_receive(void)
{
	while(!(SPSR & (1 << SPIF)));
	return SPDR; // reading the SPDR and SPSR clears flags & ready for re-read
}

inline void MSPI_CS_on()
{
    PORTB &= ~(1 << PB2); // active low, select slave/CS
}

inline void MSPI_CS_off()
{
    PORTB |= (1 << PB2); // active low, select slave/CS
}