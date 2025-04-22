#ifndef SPI_HPP
#define SPI_HPP

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"

void MSPI_Init(void);

inline void MSPI_Transmit(char cData);

inline uint16_t MSPI_receive(void);

inline void MSPI_CS_on();

inline void MSPI_CS_off();

#endif