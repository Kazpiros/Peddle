/*
    Implementation created by Rena Kollmann-Suhr and Kazimierz Piros
*/
#include <stdio.h>

#define FIXED_POINT_SCALE 32768. // convert float to int by 1 << 15 (2^15)
#define A_ 1.64676025806 // prod_{i=0}^{n} (sqrt{1+2^{-2i}})
static float NAN = 0.0 / 0.0;

//table of precalculated atan(2^-i) up to 16 iterations
float z_table[16] = {
					1474560, 870483, 459939, 
					233472, 117189, 58651, 
					29333, 14667, 7333, 
					3666, 1833, 916, 
					458, 229, 114, 57
};

//Rotation mode calculates sin and cosine of a vector given an angle
static void CORDIC_Rotating(int* x, int* y, int* theta) {
	register int i = 0;
	register int x1 = *x; register int y1 = *y; register int theta1 = *theta;
	register int x2; register int y2; register int next_z;

	next_z = z_table[0];
	while (i < 16) {
		int sign = (!!theta1) | (theta1 >> 31); //extract sign of theta1

		x2 = x1 - (sign * (y1 >> i));
		y2 = y1 + (sign * (x1 >> i));
		theta1 = theta1 - (sign * next_z);
		x1 = x2;
		y1 = y2;
		i++;
		next_z = z_table[i];
	}
	*x = x1;
	*y = y1;
	*theta = theta1;
}

//Vectoring mode calculates the angle of a vector given sin and cosine, which is equivalent to calculating arctan(y/x)
static void CORDIC_Vectoring(int* x, int* y, int* z) {
	register int i = 0;
	register int x1 = *x; register int y1 = *y; register int z1 = 0;
	register int x2; register int y2; register int next_z;

	next_z = z_table[0];
	while (i < 16) {
		int sign = (!!y1) | (y1 >> 31); //extract sign of theta1
		
		x2 = x1 + (sign * (y1 >> i));
		y2 = y1 - (sign * (x1 >> i));
		z1 = z1 + (sign * next_z);
		x1 = x2;
		y1 = y2;
		i++;
		next_z = z_table[i];
	}
	*x = x1;
	*y = y1;
	*z = z1;
}
