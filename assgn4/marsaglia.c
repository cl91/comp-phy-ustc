#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* rand() normalized to [-1,1] */
double rand_norm(void)
{
	return 2.0 * (double) (rand() - RAND_MAX/2) / (double) RAND_MAX;
}

/* implements the marsaglia algorithm for sampling an uniform
   distribution over a sphere */
void rand_marsaglia(double *x, double *y)
{
	double u, v, r_squared;

start_all_over:
	u = rand_norm();
	v = rand_norm();
	r_squared = u*u + v*v;
	if (r_squared > 1)
		goto start_all_over;
	*x = 2*u * sqrt(1-r_squared);
	*y = 2*v * sqrt(1-r_squared);
}

/* see funny comments in main.c */
#define NSTEPS	(1<<14)
int main(void)
{
	double x, y;
	int i;

	srand(time(NULL));
	for (i = 0; i < NSTEPS; i++) {
		rand_marsaglia(&x, &y);
		printf("%.12f %.12f\n", x, y);
	}

	return 0;
}
