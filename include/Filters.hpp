#ifndef FILTERS_HPP
#define FILTERS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "circular_buffer.hpp"

#define IIR_ORDER 2

#define SATURATE(x) ((x > INT16_MAX) ? INT16_MAX : (x < INT16_MIN) ? INT16_MIN : (int16_t)(x))
#define LERP(x,x1,x2,y2,y1) (y1 + (x-x1)/(x2-x1)*(y2-y1))

static int32_t state[IIR_ORDER] = { 0 };

//change this
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

int16_t iir_DF2(circular_buf* filter_buf, int16_t input, int16_t level);

int16_t iir_DF1(circular_buf* filter_buf, int16_t input, int16_t level);

#endif