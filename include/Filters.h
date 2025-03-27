#ifndef FILTERS_H
#define FILTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "circular_buffer.h"

#define IIR_ORDER 2
#define INT16_MAX 32767
#define INT16_MIN -32768
#define SATURATE(x) ((x > INT16_MAX) ? INT16_MAX : (x < INT16_MIN) ? INT16_MIN : (int16_t)(x))

static int32_t state[IIR_ORDER] = { 0 };

// REMOVE THESE LINES:
// static const int16_t b[10][IIR_ORDER + 1]; 
// static const int16_t a[10][IIR_ORDER];

// Keep these definitions:
static const int16_t a[10][2] = {
    {-29226, 13189},
    {-27840, 12106},
    {-25927, 10743},
    {-23303, 9097},
    {-19736, 7224},
    {-14936, 5284},
    {-8523, 3601},
    {112, 2811},
    {12256, 4461},
    {31063, 14764}
};

static const int16_t b[10][3] = {
    {87, 174, 87},
    {162, 325, 162},
    {300, 600, 300},
    {544, 1089, 544},
    {968, 1936, 968},
    {1683, 3366, 1683},
    {2866, 5731, 2866},
    {4827, 9654, 4827},
    {8275, 16551, 8275},
    {15553, 31106, 15553}
};

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

#endif
