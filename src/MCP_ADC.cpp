#include <stdio.h>
#include <stdlib.h>

#include "MCP3461_ADC.h"
#include "SPI.hpp"
#include "MCP_ADC.hpp"
#include "circular_buffer.hpp"

// writes to all registers of ADC
void MCP_ADC_write(uint8_t start_addr, uint8_t device_address)
{   
    if((start_addr == 0x00) || (start_addr >= 0x0E))
        return; // invalid addresses
    
    uint8_t cmd = (ADC_SPI_DEVICE_ADDR << 6) | (start_addr << 2) | (MCP_ADC_WRITE);  // start Inc. Write
    MSPI_Transmit(cmd);

    uint8_t current_addr = start_addr;
    for(int i = 0; i < 0x0D; i++)
    {
        MSPI_Transmit(incWriteLoop[current_addr]);
        current_addr++;
        // Rollover
        if(current_addr == 0x0E)
            current_addr = 0x01;
    }
}
// Reads only data register
inline uint_fast16_t MCP_ADC_static_read(void)
{
    MSPI_Transmit(MCP_ADC_SINGLE_READ);
    uint16_t temp = (uint_fast16_t) MSPI_receive();
    temp |= ((uint_fast16_t) MSPI_receive() << 7); // bang into 1 var
    return temp;
}
// Reads all ADC registers
void MCP_ADC_reg_map_read(void)
{
    MSPI_Transmit(MCP_ADC_INC_READ);
    for(int i = 0; i < 0x0F; i++)
    {
        incReadLoop[i] = MSPI_receive();
    }
}