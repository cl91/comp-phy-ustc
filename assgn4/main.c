#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* rand() normalized to [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* if you don't understand this go fuck yourself */
double radius(double xi)
{
	return sqrt(xi * (2-xi));
}

/* see comments for radius() */
#define CONST_PI	(4.0*atan(1.0))
double varphi(double xi)
{
	return 2 * CONST_PI * xi;
}

/* if you don't understand this you are an ass */
#define NSTEPS	(1<<14)
int main(void)
{
	int i;
	double r, p;

	srand(time(NULL));
	for (i = 0; i < NSTEPS; i++) {
		r = radius(rand_norm());
		p = varphi(rand_norm());
		printf("%.12f %.12f\n", r*cos(p), r*sin(p));
	}
	return 0;
}
