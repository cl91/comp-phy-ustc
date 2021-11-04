#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* uniform distribution over [0,a] */
double rand_uni(double a)
{
	return a * (double) rand() / (double) RAND_MAX;
}

/* compute \int^1_0\sqrt{x+\sqrt{x}} using Monte-Carlo integration
 * nsteps --- number of sampling points
 */
double integral_1(int nsteps)
{
	int i;
	double sum;
	double rand_nr;

	sum = 0.0;
	for (i = 0; i < nsteps; i++) {
		rand_nr = rand_uni(1.0);
		sum += sqrt(rand_nr + sqrt(rand_nr));
	}
	return sum / (double) nsteps;
}

/* compute \int^{7/10}_0 dx \int^{4/5}_0 dy \int^{9/10}_0 dz
 * \int^1_0 du \int^{11/10}_0 dv (6-x^2-y^2-z^2-u^2-v^2)
 */
double integral_2(int nsteps)
{
	int i;
	double sum;
	double x, y, z, u, v;

	sum = 0.0;
	for (i = 0; i < nsteps; i++) {
		x = rand_uni(0.7);
		y = rand_uni(0.8);
		z = rand_uni(0.9);
		u = rand_uni(1.0);
		v = rand_uni(1.1);
		sum += 6.0 - x*x - y*y - z*z - u*u - v*v;
	}
	return sum * 0.7 * 0.8 * 0.9 * 1.1 / (double) nsteps;
}


#define NSTEPS	(1<<22)
int main(void)
{
	FILE *fout;

	fout = fopen("result_1", "w");
	if (fout == NULL) {
		perror("fopen() result_1");
		abort();
	}
	fprintf(fout, "%.12f", integral_1(NSTEPS));
	fclose(fout);

	fout = fopen("result_2", "w");
	if (fout == NULL) {
		perror("fopen() result_2");
		abort();
	}
	fprintf(fout, "%.12f", integral_2(NSTEPS));
	fclose(fout);

	return 0;
}
