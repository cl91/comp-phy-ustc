#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define CONST_PI	(atan(1.0)*4.0)

/* uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* sample from the Lorentz distribution */
double lorentz(void)
{
	return tan(CONST_PI * (rand_norm()-0.5));
}

/* g(x) = \frac{p(x){LF(x)}, where L = \max p(x)/F(x),
 * p(x) = Gauss dist, F(x) = Lorentz dist
 */
#define CONST_L	(sqrt(2*CONST_PI/exp(1.0)))
double g(double x)
{
	return 0.5 * exp(0.5*(1-x*x)) * (1+x*x);
}

/* records number of total sampling instances and accepted ones */
static int Nr_accepted = 0;
static int Nr_total_instance = 0;
/* sample from the gauss distribution p(x) = \frac{e^{-x^2/2}}{\sqrt{2\pi}} */
double gauss(void)
{
	double xi, eta;

	do {
		Nr_total_instance++;
		xi = rand_norm();
		eta = lorentz();
	} while (xi > g(eta));
	Nr_accepted++;

	return eta;
}

#define NSTEPS	(1<<17)
#define NBINS	(1<<12)
#define RANGE	(10.0)
/* print the normalized frequency count of a distribution */
void count_freq(FILE *fout, double (*pdist)(void))
{
	int i;
	int freq[NBINS];
	double rand_nr;

	/* initialize random number generator and freq[] */
	srand(time(NULL));
	for (i = 0; i < NBINS; i++)
		freq[i] = 0.0;

	/* count the frequency of pdist() */
	for (i = 0; i < NSTEPS; i++) {
		rand_nr = pdist();
		if ((rand_nr < -RANGE) || (rand_nr > RANGE))
			continue; /* eliminate points outside [-RANGE,RANGE] */
		freq[(int)(rand_nr/(2*RANGE/NBINS)) + NBINS/2]++;
	}

	/* print the normalized frequency count */
	for (i = 0; i < NBINS; i++) {
		fprintf(fout, "%.12f %.12f\n", RANGE*(2.0*i/NBINS-1.0),
			(double)(freq[i]*NBINS) / (2.0*RANGE*NSTEPS));
	}
}

int main(void)
{
	FILE *fout = fopen("freq_lorentz", "w");
	double efficiency;	/* acceptance rate of acc-rej sampling */

	if (fout == NULL) {
		perror("fopen() lorentz");
		abort();
	}
	count_freq(fout, lorentz);
	fclose(fout);
	fout = fopen("freq_gauss", "w");
	if (fout == NULL) {
		perror("fopen() gauss");
		abort();
	}
	count_freq(fout, gauss);
	fclose(fout);

	/* compute efficiency (acceptance rate) of acc-rej sampling */
	fout = fopen("efficiency_gauss", "w");
	if (fout == NULL) {
		perror("fopen() efficiency_gauss");
		abort();
	}
	efficiency = (double) Nr_accepted / (double) Nr_total_instance;
	fprintf(fout, "%.12f\n", efficiency);
	fclose(fout);

	return 0;
}
