#ifndef MCP_DAC_HPP
#define MCP_DAC_HPP

#include <stdio.h>
#include <stdlib.h>

#include "MCP_DAC48CXDX.h"
#include "SPI.hpp"

class MCP_DAC {
    public:
    explicit MCP_DAC(uint8_t channel);

    void DAC_init(); 

    void DAC_write(uint16_t value);

    void DAC_write_continuous(const uint16_t *buffer, uint16_t length);

    int DAC_read_addr();

    private:
    uint8_t _channel;
    inline void select_dac() { PORTD &= ~(1 << DAC_CS_PIN); }
    inline void deselect_dac() { PORTD |= (1 << DAC_CS_PIN); }
};
#endif