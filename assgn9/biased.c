#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "count_freq.h"

/* generate uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* parameters for the \Delta r prob. distribution, to conform funcs in
 * unbiased.c, global variables were used
 */
double Param_omega = 0.01;
double Param_a = 0.3;

/* sample \Delta r with prob. distribution
 * p(\Delta r = -1) = 0.5 + a\sin(\omega t)
 * p(\Delta r = 1) = 0.5 - a\sin(\omega t)
 */
int sample_biased_delta_r(int t)
{
	return (rand_norm() > (0.5 + Param_a*sin(Param_omega*t))) ? 1 : -1;
}

/* sample random variable r with r = \sum_i \Delta r_i */
int sample_biased_r(int nsteps)
{
	int i, dist;

	dist = 0;
	for (i = 0; i < nsteps; i++)
		dist += sample_biased_delta_r(i);
	return dist;
}

/* if you cannot understand this macro consulte
 * {\sl C: A Reference Manual}, Samuel P. Harbison III, Guy L. Steele Jr.
 * Fifth Edition, pp. 44, Table 3-1, and Chapter 3 in general
 */
#define NSTEPS	(1<<7)
#define test_random_walk(omega)	do {					\
		int i, j;						\
		int dist;		/* random variable r */		\
		double sum, sum_x2, mean, rms;				\
		FILE *fout, *fout_rms;					\
									\
		Param_omega = omega;					\
		fout = fopen("res_b_" #omega, "w");			\
		if (fout == NULL) {					\
			perror("fopen() res_b_" #omega);		\
			abort();					\
		}							\
		srand(time(NULL));					\
		dist = 0;						\
		for (i = 0; i < (1<<12); i++) {				\
			fprintf(fout, "%d %d\n", i, dist);		\
			dist += sample_biased_delta_r(i);		\
		}							\
		fclose(fout);						\
									\
		fout = fopen("res_b_f_" #omega, "w");			\
		if (fout == NULL) {					\
			perror("fopen() res_b_f_" #omega);		\
			abort();					\
		}							\
		count_freq(fout, sample_biased_r, -200.0, 200.0);	\
		fclose(fout);						\
									\
		fout = fopen("res_b_mean_" #omega, "w");		\
		fout_rms = fopen("res_b_rms_" #omega, "w");		\
		if (fout == NULL) {					\
			perror("fopen() res_b_mean_" #omega);		\
			abort();					\
		}							\
		if (fout_rms == NULL) {					\
			perror("fopen() res_b_rms_" #omega);		\
			abort();					\
		}							\
		for (i = 0; i < (1<<10); i++) {				\
			sum = 0.0;					\
			sum_x2 = 0.0;					\
			for (j = 0; j < NSTEPS; j++) {			\
				dist = sample_biased_r(i);		\
				sum += dist;				\
				sum_x2 += dist * dist;			\
			}						\
			mean = sum / NSTEPS;				\
			rms = sqrt(sum_x2 / NSTEPS - mean * mean);	\
			fprintf(fout, "%d %.12f\n", i, mean);		\
			fprintf(fout_rms, "%d %.12f\n", i, rms);	\
		}							\
		fclose(fout);						\
		fclose(fout_rms);					\
	} while (0)

int main(void)
{
test_random_walk(0.01);
test_random_walk(0.1);
return 0;
}
