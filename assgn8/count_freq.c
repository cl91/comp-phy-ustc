#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
