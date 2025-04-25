#ifndef MCP_DAC_HPP
#define MCP_DAC_HPP

#include <stdio.h>
#include <stdlib.h>

#include "MCP_DAC48CXDX.h"
#include "SPI.hpp"

// Volatile
#define DAC0_REG_ADDR 0x00
#define DAC1_REG_ADDR 0x01
#define VREF_REG_ADDR 0x08
#define PD_REG_ADDR 0x09
#define STAT_GAIN_REG_ADDR 0x0A
#define DAC0_WIPER_ADDR 0x10
#define DAC1_WIPER_ADDR 0x11

// Non-volatile & only on MCP48CMDXX (need HVC!!!!!) (SOMETHING I HAVE NOT DESIGNED FOR)
#define NVREF_REG_ADDR 0x18
#define NPD_REG_ADDR 0x19
#define NSTAT_GAIN_REG_ADDR 0x1A
#define NV_GAIN_ADDR 0x1A
#define NV_WIPERLOCK_ADDR 0x1B

class MCP_DAC {
    public:
    explicit MCP_DAC(uint8_t channel);

    void DAC_init(); 

    // Idk why this exists, maybe i need it for soemthing.
    void DAC_write(uint16_t value);

    void DAC_write(uint16_t value, uint8_t address);

    void DAC_write_continuous(const uint16_t *buffer, uint16_t length);

    int DAC_read_addr();

    private:
    uint8_t _channel;
    inline void select_dac() { PORTD &= ~(1 << DAC_CS_PIN); }
    inline void deselect_dac() { PORTD |= (1 << DAC_CS_PIN); }
};
#endif