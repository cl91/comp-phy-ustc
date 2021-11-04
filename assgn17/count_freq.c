#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NBINS			(1<<7)

/* print the normalized frequency count of a distribution v[] */
void count_freq(FILE *fout, double *v, int nr_particle,
		double lcutoff, double rcutoff)
{
	int i;
	int freq[NBINS];
	int total_freq;
	double range = rcutoff - lcutoff;

	/* initialize freq[] */
	for (i = 0; i < NBINS; i++) {
		freq[i] = 0.0;
	}
	total_freq = 0;

	/* count the frequency of v[] */
	for (i = 0; i < nr_particle; i++) {
		double d = v[i];
		/* ignore points outside [lcutoff,rcutoff] */
		if ((d < lcutoff) || (d > rcutoff))
			continue;
		freq[(int)(((d-lcutoff)/range)*NBINS)]++;
		total_freq++;
	}

	/* print the normalized frequency count */
	for (i = 0; i < NBINS; i++) {
		if (freq[i] != 0) {
			fprintf(fout, "%.12f %.12f\n",
				lcutoff + (range*i) / (double) NBINS,
				(double) freq[i] / (double) total_freq);
		}
	}
}
