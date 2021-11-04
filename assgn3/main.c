#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CONST_PI	(atan(1.0)*4.0)

/* uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* see assgn3.pdf */
double theta(double xi)
{
	return acos(1 - 2*xi);
}

/* see assgn3.pdf */
double phi(double xi)
{
	return 2 * CONST_PI * xi;
}

#define NSTEPS	(1<<11)
int main(void)
{
	int i;
	double t, p;

	srand(time(NULL));	/* init rand nr. gen. */
	for (i = 0; i < NSTEPS; i++) { /* generate dataset */
		t = theta(rand_norm());
		p = phi(rand_norm());
		printf("%.12f %.12f %.12f\n",
		       sin(t)*cos(p), sin(t)*sin(p), cos(t));
	}

	return 0;
}
