#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

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


inline void write_cbuf(circular_buf* cbuf, int16_t val);

inline int16_t read_cbuf(circular_buf* cbuf, unsigned Xn);

void cbuf_init(circular_buf* cbuf);

#endif