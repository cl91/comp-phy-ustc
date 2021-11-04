#ifndef COUNT_FREQ_H
#define COUNT_FREQ_H

void count_freq(FILE *fout, int (*pdist)(FILE *, int, int, double *),
		double, double);

#endif	/* COUNT_FREQ_H */
