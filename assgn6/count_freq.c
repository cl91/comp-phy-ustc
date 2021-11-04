#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#undef NDEBUG
#include <assert.h>

#define NSTEPS	(1<<17)
#define NBINS	(1<<12)

/* print the normalized frequency count of a distribution
 * prob_accum can be either probability distribution or accumulated dist.
 */
void count_freq(FILE *fout, int (*pdist)(double *, int *, int),
		int lcutoff, int rcutoff,
		double *prob_accum, int *energy, int nsample)
{
	int i;
	int freq[NBINS];
	int rand_nr;
	int range = rcutoff - lcutoff;

	assert(range > 0);

	/* initialize random number generator and freq[] */
	srand(time(NULL));
	for (i = 0; i < NBINS; i++)
		freq[i] = 0.0;

	/* count the frequency of pdist() */
	for (i = 0; i < NSTEPS; i++) {
		rand_nr = pdist(prob_accum, energy, nsample);
		/* eliminate points outside [lcutoff,rcutoff] */
		if ((rand_nr < lcutoff) || (rand_nr > rcutoff))
			continue;
		freq[(rand_nr-lcutoff)*NBINS/range]++;
	}

	/* print the normalized frequency count */
	for (i = 0; i < NBINS; i++) {
		fprintf(fout, "%.12f %.12f\n",
			lcutoff + (double) (range * i) / (double) NBINS,
			((double) freq[i]/ (double) NSTEPS));
	}
}
