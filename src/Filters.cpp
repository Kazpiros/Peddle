#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "circular_buffer.hpp"
#include "Filters.hpp"

int16_t iir_DF2(circular_buf* filter_buf, int16_t input, int16_t level)
{
    write_cbuf(filter_buf, input);

    int32_t acc = (((int32_t)input * b[level][0]) >> 15) + state[0];
    int16_t y = SATURATE(acc);

    int32_t s0_new = state[1] 
                     + (((int32_t)input * b[level][1]) >> 15)
                     - (((int32_t)y * a[level][0]) >> 15);
    state[0] = SATURATE(s0_new);

    int32_t s1_new = (((int32_t)input * b[level][2]) >> 15)
                     - (((int32_t)y * a[level][1]) >> 15);
    state[1] = SATURATE(s1_new);

    return y;
}

int16_t iir_DF1(circular_buf* filter_buf, int16_t input, int16_t level)
{
    write_cbuf(filter_buf, input);
    
    int y = (b[level][0] * read_cbuf(filter_buf, 0)
           + b[level][1] * read_cbuf(filter_buf, 1)
           + b[level][2] * read_cbuf(filter_buf, 2)
           - a[level][0] * read_cbuf(filter_buf, 1)
           - a[level][1] * read_cbuf(filter_buf, 2)) >> 15;
    y = SATURATE(y);
    return (int16_t)y;
}

int16_t envelope_detector(circular_buf* envelope_buf, int input)
{
    //ALPHA = exp(-2pi fc/fs)
    input = abs(input);
    write_cbuf(envelope_buf, input);
    int y = (ALPHA * read_cbuf(envelope_buf, 1) 
            + (ALPHAL)*input);
    return y;
}

void fft(void)
{

}