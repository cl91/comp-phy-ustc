#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#undef NDEBUG
#include <assert.h>

#include "count_freq.h"

/* generates uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* implements direct sampling of energy values according to
 * accumulating probability accum[]
 */
int sample_direct(double *accum, int *energy, int nsample)
{
	double xi = rand_norm();
	int i, j, mid;

	i = 0;
	j = nsample-1;
	if (xi <= accum[0])
		return energy[0];
	while (i < j-1) {	/* binary search */
		mid = (i + j) / 2;
		if (xi < accum[mid])
			j = mid;
		else if (xi > accum[mid])
			i = mid;
		else
			return energy[mid];
	}
	return energy[j];
}

#define IDX_ECUTOFF_1	93
#define IDX_ECUTOFF_2	104
#define IDX_MAXPROB_1	63
#define IDX_MAXPROB_2	99
#define IDX_MAXPROB_3	105

/* sample from rectangular distribution function F(x) */
int sample_rect(double *prob, int nsample)
{
	double xi;
	double normfac;		/* normalization factor */

	normfac = prob[IDX_MAXPROB_1] * (IDX_ECUTOFF_1+1) +
		prob[IDX_MAXPROB_2] * (IDX_ECUTOFF_2-IDX_ECUTOFF_1) +
		prob[IDX_MAXPROB_3] * (nsample-1-IDX_ECUTOFF_2);

	xi = normfac * rand_norm();
	if (xi <= prob[IDX_MAXPROB_1] * (IDX_ECUTOFF_1+1))
		return xi / prob[IDX_MAXPROB_1];
	else if (xi <= prob[IDX_MAXPROB_1] * (IDX_ECUTOFF_1+1)
		 + prob[IDX_MAXPROB_2] * (IDX_ECUTOFF_2-IDX_ECUTOFF_1))
		return (xi - prob[IDX_MAXPROB_1] * (IDX_ECUTOFF_1+1)) /
			prob[IDX_MAXPROB_2] + IDX_ECUTOFF_1 + 1;
	else
		return (xi - prob[IDX_MAXPROB_1] * (IDX_ECUTOFF_1+1) -
			prob[IDX_MAXPROB_2] * (IDX_ECUTOFF_2-IDX_ECUTOFF_1)) /
			prob[IDX_MAXPROB_3] + IDX_ECUTOFF_2 + 1;
}

/* records number of total sampling instances and accepted ones */
static int Nr_accepted = 0;
static int Nr_total_instance = 0;
/* implements acceptance-rejection sampling of energy values
 * for probability distribution prob[]
 */
int sample_accrej(double *prob, int *energy, int nsample)
{
	double xi;		/* xi ~ Uniform([0,1]) */
	int idx;		/* idx ~ F(i) */
	double lambda;		/* temporary variable */

	do {
		Nr_total_instance++;
		xi = rand_norm();
		idx = sample_rect(prob, nsample);
		assert(idx >= 0 && idx < nsample);
		if (idx <= IDX_ECUTOFF_1)
			lambda = prob[idx] / prob[IDX_MAXPROB_1];
		else if (idx <= IDX_ECUTOFF_2)
			lambda = prob[idx] / prob[IDX_MAXPROB_2];
		else
			lambda = prob[idx] / prob[IDX_MAXPROB_3];
	} while (xi > lambda);
	Nr_accepted++;

	return energy[idx];
}

#define BUFSIZE	(128)
int main(void)
{
	int energy[BUFSIZE];	/* energy values of experimental data */
	int count[BUFSIZE];	/* count rates of exp. data */
	double prob[BUFSIZE];	/* normalized count (probability) */
	double accum[BUFSIZE];	/* accumulated norm. count */
	int nsample;		/* number of points in exp. data */
	int total_count;	/* total number of counts */
	FILE *fin;		/* data file to be read */
	FILE *fout;
	double efficiency;	/* acceptance rate of acc-rej sampling */
	int i;

	fin = fopen("data", "r");
	if (fin == NULL) {
		perror("fopen() data");
		abort();
	}

	/* read data file into energy[] and count[] */
	i = 0;
	total_count = 0;
	while (fscanf(fin, "%d %d", &energy[i], &count[i]) > 0) {
		total_count += count[i];
		i++;
	}
	nsample = i;
	fclose(fin);

	/* normalize count rate to get probability */
	for (i = 0; i < nsample; i++) {
		prob[i] = (double) count[i] / (double) total_count;
	}

	/* compute accumulated probability accum[] */
	accum[0] = prob[0];
	for (i = 1; i < nsample; i++) {
		accum[i] = prob[i] + accum[i-1];
	}

	/* print normalized freq. count of the original data */
	fout = fopen("freq_data", "w");
	if (fout == NULL) {
		perror("fopen() freq_data");
		abort();
	}
	for (i = 0; i < nsample; i++) {
		fprintf(fout, "%d %.12f\n", energy[i], prob[i]);
	}
	fclose(fout);

	/* print normalized frequency count of direct sampling */
	fout = fopen("freq_direct", "w");
	if (fout == NULL) {
		perror("fopen() freq_direct");
		abort();
	}
	count_freq(fout, sample_direct, energy[0], energy[nsample-1],
		   accum, energy, nsample);
	fclose(fout);

	/* print normalized frequency count of acceptance-rejection sampling */
	fout = fopen("freq_accrej", "w");
	if (fout == NULL) {
		perror("fopen() freq_accrej");
		abort();
	}
	count_freq(fout, sample_accrej, energy[0], energy[nsample-1],
		   prob, energy, nsample);
	fclose(fout);

	/* compute efficiency (acceptance rate) of acc-rej sampling */
	fout = fopen("accrej_efficiency", "w");
	if (fout == NULL) {
		perror("fopen() accrej_efficiency");
		abort();
	}
	efficiency = (double) Nr_accepted / (double) Nr_total_instance;
	fprintf(fout, "%.10f\n", efficiency);
	fclose(fout);

	return 0;
}
