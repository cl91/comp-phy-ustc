#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "count_freq.h"

/* uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* sample from uniform distribution over the set {1,-1},
 * i.e. Bernoulli distribution
 */
int sample_delta_r(void)
{
	return (rand_norm() > 0.5) ? 1 : -1;
}

/* sample from R = \sum_i r_i, where r_i ~ Bernoulli dist. (see above) */
int sample_r(int nsteps)
{
	int i, dist;

	dist = 0;
	for (i = 0; i < nsteps; i++)
		dist += sample_delta_r();
	return dist;
}

#define NSTEPS			(1<<12)
int main(void)
{
	int i, j;
	int dist;
	double sum, sum_x2, mean, rms;
	FILE *fout, *fout_rms;

	/* plot r-t diagram of one random walk with NSTEPS */
	fout = fopen("res_ub", "w");
	srand(time(NULL));
	dist = 0;
	for (i = 0; i < NSTEPS; i++) {
		fprintf(fout, "%d %d\n", i, dist);
		dist += sample_delta_r();
	}
	fclose(fout);

	/* plot the frequency histogram of many random walks */
	fout = fopen("res_ub_f", "w");
	count_freq(fout, sample_r, -150.0, 150.0);
	fclose(fout);

	/* plot mean = \langle x\rangle and
	   rms = \sqrt{\langle x^2\rangle - \langle x\rangle^2}
	   versus time */
	fout = fopen("res_ub_mean", "w");
	fout_rms = fopen("res_ub_rms", "w");
	if (fout == NULL) {
		perror("fopen() res_ub_mean");
		abort();
	}
	if (fout_rms == NULL) {
		perror("fopen() res_ub_rms");
		abort();
	}
#define NSTEPS_PER_WALK	(1<<7)
	for (i = 0; i < (1<<10); i++) {
		sum = 0.0;
		sum_x2 = 0.0;
		for (j = 0; j < NSTEPS_PER_WALK; j++) {
			dist = sample_r(i);
			sum += dist;
			sum_x2 += dist * dist;
		}
		mean = sum / NSTEPS_PER_WALK;
		rms = sqrt(sum_x2 / NSTEPS_PER_WALK - mean * mean);
		fprintf(fout, "%d %.12f\n", i, mean);
		fprintf(fout_rms, "%d %.12f\n", i, rms);
	}
	fclose(fout);
	fclose(fout_rms);

	return 0;
}
