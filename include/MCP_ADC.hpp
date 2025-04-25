#ifndef MCPADC_HPP
#define MCPADC_HPP

#include <stdio.h>
#include <stdlib.h>

#include "MCP3461_ADC.h"
#include "SPI.hpp"
#include "circular_buffer.hpp"

class MCP_ADC
{
    public:
    // channel selection not functional yet, use the .h for better control.
    MCP_ADC(uint8_t channel);

    // writes to all registers of ADC
    void write(uint8_t start_addr, uint8_t device_address);

    // Reads only data register
    inline uint_fast16_t static_read(void);

    // Reads all ADC registers
    void reg_map_read(void);

    static void trigger_int(void);

    private:
    uint8_t _channel;
};
#endif