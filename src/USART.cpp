#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <stdlib.h>

#include "USART.hpp"

//8bit 2stop uart, USING USART 0? -- 20.7
void USART_init(int BAUD)
{
	uint16_t ubrr = (uint16_t)(F_CPU / (8 * BAUD) - 1);

	UCSR0A  |= (1 << U2X0);
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) (ubrr);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); //tx, rx enable
	UCSR0C = (1 << USBS0) | (3 << UCSZ00); // 8bit data, 2 stops
	// ucszXX changes bit depth, 9 data bits is covered in atmega package
}

void USART_Transmit(unsigned int data)
{
	// wait to empty tx buffer
	while( !((UCSR0A) & (1<<UDRE0)) ); 
	UDR0 = data;
}

unsigned char USART_Receive(void)
{
	// wait to fill rx buffer
	while ( !(UCSR0A & (1<<RXC0)) );
	
	return UDR0;
}

void USART_Flush( void )
{
	unsigned char dummy;
	while( UCSR0A & (1<<RXC0) )
	{
		dummy = UDR0;
	}
}

void USART_MSPI_Init(void) // gotta enable MISO too!
{
	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
	PORTB |= (1 << PB2); // deselect slave
	// Enable SPI, SCK = F_CPU/128
	SPCR = (1 << SPE) | (1 << MSTR)
		 | (1 << SPR0) | (1 << SPR1);
	
}

inline void USART_MSPI_Transmit(char cData)
{
	PORTB &= ~(1 << PB2); // SS low - select slave
	SPDR = cData; // start tx
	while(!(SPSR & (1 << SPIF)));
	PORTB |= (1 << PB2); // SS high - deselect slave
}

void USART_SSPI_init(void)
{
	// set pin SS (DDB2 input)
	DDRB |= (1 << DDB4);
	//here for good practice
	DDRB &= ~((1 << DDB2) | (1 << DDB3) | (1 << DDB5));
	// Enable SPI
	SPCR = (1 << SPE);
}

inline char USART_MSPI_receive(void)
{
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}

inline char USART_SSPI_receive(void)
{
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}