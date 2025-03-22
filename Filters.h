#include <stdio.h>
#include <stdlib.h>
#include "circular_buffer.h"
#include "filter_coefs.h"

#define IIR_ORDER 2
#define INT16_MAX 32767
#define INT16_MIN -32768


static int32_t state[IIR_ORDER] = { 0 };

int saturate_int16(int x){
    return (x > INT16_MAX) ? INT16_MAX : (x < INT16_MIN) ? INT16_MIN : (int16_t)x;
}

int16_t iir_DF2(int16_t input, int8_t level) {

    // Output calculation (already saturated)
    int32_t acc = (((int32_t)input * b[level][0]) >> 15) + state[0];
    int16_t y = saturate_int16(acc);

    // Update state[0] with saturation
    int32_t s0_new = state[1] + (((int32_t)input * b[level][1]) >> 15) - (((int32_t)y * a[level][0]) >> 15);
    state[0] = saturate_int16(s0_new);

    // Update state[1] with saturation
    int32_t s1_new = (((int32_t)input * b[level][2]) >> 15) - (((int32_t)y * a[level][1]) >> 15);
    state[1] = saturate_int16(s1_new);

    return y;
}

int iir_CDE(circular_buf* input, circular_buf* output, int8_t level) {
    int temp = ((int32_t)b[level][0] * read_cbuf(input, 0) + 
               (int32_t)b[level][1] * read_cbuf(input, 1) + 
               (int32_t)b[level][2] * read_cbuf(input, 2) -
               (int32_t)a[level][0] * read_cbuf(output, 1) - 
               (int32_t)a[level][1] * read_cbuf(output, 2)) >> 15;
    
    // Saturate before writing to buffer
    int16_t saturated_value = saturate_int16(temp);
    write_cbuf(output, (uint16_t)saturated_value);
    
    return saturated_value;
}


