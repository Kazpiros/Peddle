#include <stdio.h>
#include <stdlib.h>
#define IIR_ORDER 3

#include "coefficients.h"
#define INT8_MAX 127
#define INT8_MIN -128

// b coefficients: b0, b1, b2
// (e.g., 0.25, 0.5, 0.25 in Q7, fixed point)
 
static int16_t state[IIR_ORDER] = { 0 };

static inline int8_t saturate_int8(int16_t x) {
    // If bits 15 down to 7 are not all the same, saturation is needed.
    if ((x >> 7) != (x >> 15)) {
        x = (x < 0) ? INT8_MIN : INT8_MAX;
    }
    return (int8_t)x;
}
// currently 2 pole, with no special features
int8_t iir_filter_process(int8_t x) {

    int8_t iir_filter_process(int8_t x) {
    int16_t acc = (((int16_t)x * b_buf[0]) >> 7) + state[0];  // y[n] = state0 + b0*x[n]
    int8_t y = saturate_int8(acc);

    // Update states for 3-pole filter
    state[0] = state[1] + (((int16_t)x * b_buf[1]) >> 7) - (((int16_t)y * a_buf[0]) >> 7);
    state[1] = state[2] + (((int16_t)x * b_buf[2]) >> 7) - (((int16_t)y * a_buf[1]) >> 7);
    state[2] = (((int16_t)x * b_buf[3]) >> 7) - (((int16_t)y * a_buf[2]) >> 7); 

    return y;
}


    return y;
}
