/* test_x_k_c_l.c
 * Implements $\langle x^k \rangle$ and $C(l)$ tests for
 * random number generator rand_schrage() and randomz()
 * Chang Liu, PB09203226
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include "schrage.h"
#include "randomz.h"

double test_x_k(int k, int nsteps, double (*prand)(double))
{
	int i, j;
	double x, x_k, accum_x_k;
	uint32_t z;

	/* setting seed */
	z = time(NULL);
	x = prand( (double) z / (double) UINT32_MAX );

	accum_x_k = 0.0;
	for (i = 0; i < nsteps; i++) {
		x = prand(x);	/* generate random number */
		x_k = 1.0;	/* compute $x_i^k$ */
		for (j = 0; j < k; j++) {
			x_k *= x;
		}
		accum_x_k += x_k; /* compute $\sum_i x_i^k$ */
	}

	return accum_x_k / nsteps; /* returns $\langle x^k \rangle$ */
}

double test_c_l(int l, int nsteps, double (*prand)(double))
{
	double x, x_l;		/* x --- $x_n$, x_l --- $x_{n+l}$ */
	double accum_x_nl;	/* $\sum_n x_{n} x_{n+l}$ */
	double accum_x_n;	/* $\sum_n x_n */
	double accum_x_n2;	/* $\sum_n x^2_n */
	uint32_t z;
	int i;

	/* setting seed */
	z = time(NULL);
	x = prand( (double) z / (double) UINT32_MAX );

	/* calculate $x_{n+l}$ */
	x_l = x;
	for (i = 0; i < l; i++) {
		x_l = prand(x_l);
	}

	accum_x_nl = 0.0;
	accum_x_n = 0.0;
	accum_x_n2 = 0.0;
	for (i = 0; i < nsteps; i++) {
		accum_x_nl += x * x_l;
		accum_x_n += x;
		accum_x_n2 += x*x;
		x = prand(x);
		x_l = prand(x_l);
	}

	return (accum_x_nl - accum_x_n * accum_x_n / nsteps)
		/ (accum_x_n2 - accum_x_n * accum_x_n / nsteps);
}

#define MAX_K	4
#define MAX_L	4
int main(void)
{
	int k, l;
	int nsteps;
	double test_res;

	for (k = 1; k<= MAX_K; k++) {
		for (nsteps = 10; nsteps <= 10000000; nsteps *= 10) {
			test_res = test_x_k(k, nsteps, rand_schrage);
			printf("schrage |<x^%d> - 1/%d| = %.12f, nsteps = %d\n",
			       k, k+1, fabs(test_res - 1.0/(k+1.0)), nsteps);
		}
		putchar('\n');
	}

	for (k = 1; k<= MAX_K; k++) {
		for (nsteps = 10; nsteps <= 10000000; nsteps *= 10) {
			test_res = test_x_k(k, nsteps, randomz);
			printf("randomz |<x^%d> - 1/%d| = %.12f, nsteps = %d\n",
			       k, k+1, fabs(test_res - 1.0/(k+1.0)), nsteps);
		}
		putchar('\n');
	}

	for (l = 1; l<= MAX_L; l++) {
		for (nsteps = 10; nsteps <= 10000000; nsteps *= 10) {
			test_res = test_c_l(l, nsteps, rand_schrage);
			printf("schrage |C(%d)| = %.12f, nsteps = %d\n",
			       l, fabs(test_res), nsteps);
		}
		putchar('\n');
	}

	for (l = 1; l<= MAX_L; l++) {
		for (nsteps = 10; nsteps <= 10000000; nsteps *= 10) {
			test_res = test_c_l(l, nsteps, randomz);
			printf("randomz |C(%d)| = %.12f, nsteps = %d\n",
			       l, fabs(test_res), nsteps);
		}
		putchar('\n');
	}

	return 0;
}
