#ifndef MCP48CXDX_H
#define MCP48CXDX_H
/*
    Device communication starts with command byte,
    then followed by databytes on SDO or SDI depending on
    READ or WRITE
*/

#include <avr/io.h>

/* CMD byte is 5 MSB bits for memory addr followed by 2 command bits*/
/* Data bits are from LSB 0-11 */
/* MSP DAC requires 24 bits, MSB byte is commander, LSB and middle are dataword*/
#define MEM_MASK (0x1F << 3)
#define CMD_MASK (0x06 << 1)

#define DAC_CS_PIN (DDD3)
#define DAC_CS_PORT (PORTD3)

#define CMDR_BYTE_WRITE (0x00) // write
#define CMDR_BYTE_READ (0x06)

#endif