#ifndef MCPADC_HPP
#define MCPADC_HPP

#include <stdio.h>
#include <stdlib.h>

#include "MCP3461_ADC.h"
#include "SPI.hpp"
#include "circular_buffer.hpp"

// writes to all registers of ADC
void MCP_ADC_write(uint8_t start_addr, uint8_t device_address);

// Reads only data register
inline uint_fast16_t MCP_ADC_static_read(void);

// Reads all ADC registers
void MCP_ADC_reg_map_read(void);

void trigger_HVC(void);

#endif