#include <stdio.h>
#include <stdlib.h>
// b coefficients: b0, b1, b2
// (e.g., 0.25, 0.5, 0.25 in Q15, fixed point)
static const int16_t b[IIR_ORDER + 1] = { 8192, 16384, 8192 }; 

static const int16_t a[IIR_ORDER] = { 15360, -7168 }; 

static int32_t state[IIR_ORDER] = { 0 };

static inline int16_t saturate_int16(int32_t x) {
    // If bits 31 down to 15 are not all the same, saturation is needed.
    if ((x >> 15) != (x >> 31)) {
        x = (x < 0) ? INT16_MIN : INT16_MAX;
    }
    return (int16_t)x;
}
// currently 2 pole, with no special features
int16_t iir_filter_process(int16_t x) {

    int32_t acc = (((int32_t)x * b[0]) >> 15) + state[0]; //y[n] = state0 + b0*x[n]
    int16_t y = saturate_int16(acc);
    
    state[0] = state[1] + (((int32_t)x * b[1]) >> 15) - (((int32_t)y * a[0]) >> 15);
    
    state[IIR_ORDER - 1] = (((int32_t)x * b[IIR_ORDER]) >> 15) - (((int32_t)y * a[IIR_ORDER - 1]) >> 15); //state1 = b2*x[n] - a2*y[n]

    return y;
}
