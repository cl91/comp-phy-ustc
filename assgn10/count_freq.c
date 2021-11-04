#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NSTEPS			(1<<10)
#define NBINS			(1<<17)

/* print the non-normalized frequency count of a distribution */
void count_freq(FILE *fout, int (*pdist)(FILE *, int, int, double *),
		double lcutoff, double rcutoff)
{
	int i;
	int freq[NBINS];
	int rand_nr;
	double range = rcutoff - lcutoff;

	/* initialize freq[] */
	for (i = 0; i < NBINS; i++)
		freq[i] = 0.0;

	/* count the frequency of pdist() */
	for (i = 0; i < NSTEPS; i++) {
		rand_nr = pdist(NULL, 512, 1024, NULL);
		/* eliminate points outside [lcutoff,rcutoff] */
		if ((rand_nr < lcutoff) || (rand_nr > rcutoff))
			continue;
		freq[(int)((rand_nr-lcutoff)*NBINS/range)]++;
	}

	/* print the normalized frequency count */
	for (i = 0; i < NBINS; i++) {
		if (freq[i] != 0) {
			fprintf(fout, "%.12f %d\n",
				lcutoff + (double)(range*i) / (double)NBINS,
				freq[i]);
		}
	}
}
