#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#undef NDEBUG
#include <assert.h>

#define INIT_X		(0.789)

#define CONST_PI	(4.0*atan(1.0))
double iter_fcn(double lambda, double x)
{
	return lambda * sin(CONST_PI * x);
}

void print_iter_seq(FILE *fout, double lambda,
		    int nskip, int nsteps)
{
	double x = INIT_X;
	int i;

	/* skip first nskip values in the sequence */
	for (i = 0; i < nskip; i++)
		x = iter_fcn(lambda, x);

	/* print next nsteps values of the iteration sequence */
	for (i = 0; i < nsteps; i++) {
		x = iter_fcn(lambda, x);
		fprintf(fout, "%.12f %.12f\n", lambda, x);
	}
}

/* compute the lyapunov exp. \nu = \lim_{n\to\infty} (dx_n/dx_0) / n */
double get_lyapunov(double lambda, double x, double delta, int nsteps)
{
	int i;
	double y = x + delta;

	assert(delta > 0);
	for (i = 0; i < nsteps; i++) {
		x = iter_fcn(lambda, x);
		y = iter_fcn(lambda, y);
	}
	return (log(fabs(y-x) / delta)) / nsteps;
}

int main(void)
{
	FILE *fout = fopen("state", "w");
	double lambda;

	if (fout == NULL) {
		perror("fopen() state");
		abort();
	}

	for (lambda = 0.0; lambda < 5; lambda += 0.01) {
		print_iter_seq(fout, lambda, 512, 128);
	}
	fclose(fout);

	fout = fopen("lyapunov", "w");
	if (fout == NULL) {
		perror("fopen() lyapunov");
		abort();
	}
	for (lambda = 0.0; lambda < 5; lambda += 0.01) {
		fprintf(fout, "%.12f %.12f\n", lambda,
			get_lyapunov(lambda, INIT_X, 0.01, 1024));
	}
	fclose(fout);

	return 0;
}
