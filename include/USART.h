#define F_CPU (int)16e6
#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU (int)16e6


//8bit 2stop uart, USING USART 0? -- 20.7
void USART_init(unsigned int BAUD)
{
	uint16_t ubrr = F_CPU / (8 * BAUD) - 1;

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
