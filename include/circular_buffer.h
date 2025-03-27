#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 16U
#define BUFFER_SIZE_MASK (BUFFER_SIZE-1U)

typedef struct circular_buf{
    int16_t buffer[BUFFER_SIZE]; 
    int readIndex;
    int writeIndex;
} circular_buf;

inline void write_cbuf(circular_buf* cbuf, int16_t val)
{
    cbuf->buffer[(++cbuf->writeIndex) & BUFFER_SIZE_MASK] = val;
}

inline int16_t read_cbuf(circular_buf* cbuf, unsigned Xn)
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

#endif
