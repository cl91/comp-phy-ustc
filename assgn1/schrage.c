/* schrage.c
 * Implements Lehmer linear congruent random number generator
 * Chang Liu, PB09206426
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CONST_A		(16807)
#define CONST_M		((1U<<31) - 1)

/* returns I_{n+1} for the given I_{n}, where I_{n+1} = (a*I_{n}) % m */
double rand_schrage(double x)
{
	if (x < 0 || x > 1) {
		fprintf(stderr, "rand_schrage: input must be within [0,1]\n");
		abort();
	}
	int32_t z = x * CONST_M;
	int32_t q = CONST_M / CONST_A;
	int32_t r = CONST_M % CONST_A;
	int32_t retval = CONST_A * (z % q) - r * (z / q);
	if (retval < 0)
		retval += CONST_M;
	return (double) retval / (double) CONST_M;
}
