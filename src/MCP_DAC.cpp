#include <stdio.h>
#include <stdlib.h>

#include "MCP_DAC48CXDX.h"
#include "SPI.hpp"
#include "MCP_DAC.hpp"

MCP_DAC::MCP_DAC(uint8_t channel) : _channel(channel)
{
    //_channel = channel; // only one channel for now...
    DAC_init();
}
void MCP_DAC::DAC_init() 
{
    MSPI_Init();
    DDRD |= (1 << DAC_CS_PIN); // Set CS as output
    select_dac();
    DAC_write(0b01,VREF_REG_ADDR);
    DAC_write(0b00,PD_REG_ADDR);
    DAC_write(0x00,STAT_GAIN_REG_ADDR);
    deselect_dac();
}

void MCP_DAC::DAC_write(uint16_t value)
{
    value &= (0x0FFF);
    
    uint8_t data_high = (value >> 8) & 0x0F;
    uint8_t data_low = (value) & 0xFF;

    select_dac();
    MSPI_Transmit(CMDR_BYTE_WRITE);
    MSPI_Transmit(data_high);
    MSPI_Transmit(data_low);
    deselect_dac();
}

void MCP_DAC::DAC_write(uint16_t value, uint8_t address)
{
    value &= (0x0FFF);
    uint8_t cmd_byte = (address << 3);
    uint8_t data_high = (value >> 8) & 0xFF;
    uint8_t data_low = (value) & 0xFF;

    select_dac();
    MSPI_Transmit(CMDR_BYTE_WRITE);
    MSPI_Transmit(data_high);
    MSPI_Transmit(data_low);
    deselect_dac();
}

void MCP_DAC::DAC_write_continuous(const uint16_t *buffer, uint16_t length)
{
    select_dac();
    
    for (uint16_t i = 0; i < length; i++) {
        uint16_t value = buffer[i] & 0x0FFF;
        uint8_t data_high = (value >> 8) & 0x0F;
        uint8_t data_low = value & 0xFF;
        
        MSPI_Transmit(CMDR_BYTE_WRITE); // Command byte
        MSPI_Transmit(data_high);
        MSPI_Transmit(data_low);
    }

    deselect_dac();
}

int MCP_DAC::DAC_read_addr()
{
    select_dac();
    MSPI_Transmit(CMDR_BYTE_READ);
    uint8_t cmd_byte = MSPI_receive();
    deselect_dac();
    if((0x01 & cmd_byte) != 1)
        return -1;
    return (cmd_byte & (MEM_MASK)) >> 3;
}