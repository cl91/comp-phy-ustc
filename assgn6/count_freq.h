#ifndef COUNT_FREQ_H
#define COUNT_FREQ_H

/* print the normalized frequency count of a distribution
 * prob_accum can be either probability distribution or accumulated dist.
 */
void count_freq(FILE *fout, int (*pdist)(double *, int *, int),
		int lcutoff, int rcutoff,
		double *prob_accum, int *energy, int nsample);

#endif	/* COUNT_FREQ_H */
