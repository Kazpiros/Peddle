#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "circular_buffer.hpp"


void write_cbuf(circular_buf* cbuf, int16_t val)
{
    cbuf->buffer[(++cbuf->writeIndex) & BUFFER_SIZE_MASK] = val;
}

int16_t read_cbuf(circular_buf* cbuf, unsigned Xn)
{
    return cbuf->buffer[(cbuf->writeIndex - Xn) & BUFFER_SIZE_MASK];
}

void cbuf_init(circular_buf* cbuf)
{
    for (int i = 0; i < BUFFER_SIZE; i++) {
        cbuf->buffer[i] = 0;
    }
    cbuf->writeIndex = 0;
    cbuf->readIndex = 0;
}

//make class to override cbuf_init to also specify a size
