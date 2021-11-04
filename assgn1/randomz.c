/* randomz.c - random number generator */

#include <math.h>

#define CONST_M		((double) 100000001)
#define CONST_A		((double) 329)

/* returns next random number */
double randomz(double x)
{
	return fmod(CONST_A * x * 1e+8, CONST_M) * 1e-8;
}
