#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "include/circular_buffer.h"  // Use your header file
#include "include/Filters.h"

#define PI 3.14159265358979323846
#define Q15_SCALE 32767
#define NUM_POINTS 4096
#define NORM_FREQ (250.0/4096.0)

// Generate sine wave points in Q15 format.
void generate_sine_points(int16_t points[], int num_points) {
    for (int n = 0; n < num_points; n++) {
         double angle = 2 * PI * NORM_FREQ * n;
         double sine_val = sin(angle);
         points[n] = (int16_t) round(sine_val * Q15_SCALE);
    }
}

// Write an array of Q15 points to a text file.
void write_points_to_file(const char* filename, int16_t points[], int num_points) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
         perror("Failed to open file");
         return;
    }
    for (int n = 0; n < num_points; n++) {
         fprintf(fp, "%d\n", points[n]);
    }
    fclose(fp);
}

// Complex number structure for DFT.
typedef struct {
    double real;
    double imag;
} Complex;

// Compute the DFT on Q15 data (converted to float).
void dft(const int16_t in[], Complex out[], int num_points) {
    for (int k = 0; k < num_points; k++) {
         double sum_real = 0.0;
         double sum_imag = 0.0;
         for (int n = 0; n < num_points; n++) {
             double angle = 2 * PI * k * n / num_points;
             double x = (double) in[n] / Q15_SCALE;
             sum_real += x * cos(angle);
             sum_imag -= x * sin(angle);
         }
         out[k].real = sum_real;
         out[k].imag = sum_imag;
    }
}

// Write DFT output to a text file.
void write_dft_to_file(const char* filename, const Complex dft_output[], int num_points) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
         perror("Failed to open DFT output file");
         return;
    }
    for (int k = 0; k < num_points; k++) {
         double magnitude = sqrt(dft_output[k].real * dft_output[k].real + 
                                 dft_output[k].imag * dft_output[k].imag);
         fprintf(fp, "Bin %d: Real = %f, Imag = %f, Magnitude = %f\n", 
                 k, dft_output[k].real, dft_output[k].imag, magnitude);
    }
    fclose(fp);
}

int main(void) {
    // Allocate arrays for original and filtered signals.
    int16_t sine_points[NUM_POINTS];
    int16_t filtered_points[NUM_POINTS];

    // Arrays for DFT outputs.
    Complex dft_output[NUM_POINTS];
    Complex dft_filtered[NUM_POINTS];

    // Generate the sine wave.
    generate_sine_points(sine_points, NUM_POINTS);
    write_points_to_file("sine_points.txt", sine_points, NUM_POINTS);

    // Initialize the circular buffer for filtering.
    circular_buf filter_buf;
    cbuf_init(&filter_buf);

    // Process each sine sample through the filter.
    // Using level 0 (index into a[][] and b[][]).
    int g = 0;
    for (int i = 0; i < NUM_POINTS; i++) {
        if(i % 409)
            g++;
        filtered_points[i] = iir_DF1(&filter_buf, sine_points[i], g);   
    }
    write_points_to_file("filtered_points.txt", filtered_points, NUM_POINTS);

    // Compute DFTs for both original and filtered signals.
    dft(sine_points, dft_output, NUM_POINTS);
    dft(filtered_points, dft_filtered, NUM_POINTS);
    
    write_dft_to_file("dft_output.txt", dft_output, NUM_POINTS);
    write_dft_to_file("dft_filtered.txt", dft_filtered, NUM_POINTS);

    return 0;
}
