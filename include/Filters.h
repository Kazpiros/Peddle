#include <stdio.h>
#include <stdlib.h>

#define IIR_ORDER 2
#define INT16_MAX 32767
#define INT16_MIN -32768

// b coefficients: b0, b1, b2
// (e.g., 0.25, 0.5, 0.25 in Q15, fixed point)
static const int16_t b[10][IIR_ORDER + 1]; 
static const int16_t a[10][IIR_ORDER];
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


static int a[10][2] = {
    {-64213, 31470},
    {-63543, 30833},
    {-62534, 29897},
    {-61017, 28540},
    {-58738, 26611},
    {-55329, 23948},
    {-50262, 20434},
    {-42811, 16101},
    {-32015, 11327},
    {-16568, 7114}
};

static int b[10][3] = {
    {6, 13, 6},
    {14, 29, 14},
    {32, 65, 32},
    {73, 146, 73},
    {160, 320, 160},
    {347, 694, 347},
    {735, 1470, 735},
    {1514, 3029, 1514},
    {3020, 6040, 3020},
    {5828, 11657, 5828}
};
