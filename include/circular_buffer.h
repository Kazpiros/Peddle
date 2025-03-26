#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 16U
#define BUFFER_SIZE_MASK (BUFFER_SIZE-1U)

typedef struct circular_buf{
    uint16_t buffer[BUFFER_SIZE];
    int readIndex;
    int writeIndex;
}circular_buf;

//circular_buf buffer = { .writeIndex = 0 }; // malloc bad

inline void write_cbuf(circular_buf* cbuf, int val)
{
    //masking done as an alternative to "if at end, go to start"
    cbuf->buffer[(++cbuf->writeIndex) & BUFFER_SIZE_MASK] = val;
}

// Xn is reads value at index most recently written, counting down
inline int read_cbuf(circular_buf* cbuf, unsigned Xn)
{
    return cbuf->buffer[(cbuf->writeIndex - Xn) & BUFFER_SIZE_MASK];
}

//can and will be just pasted into final product, removal of function.
void cbuf_init(circular_buf* cbuf)
{
    cbuf->writeIndex = 0;
    cbuf->readIndex = 0;
}

#endif
