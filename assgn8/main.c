#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "count_freq.h"

#define CONST_PI	(4.0*atan(1.0))

/* uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* sample from the exponential distribution p(x) = \exp(-x-1), (x>-1)
 * mean = 0, variance = 1
 */
double sample_exp(void)
{
	return -log(rand_norm())-1;
}

/* sample from Gaussian distribution p(x) = \exp(-x^2)/\sqrt{2\pi}
 * mean = 0, variance = 1
 */
double sample_gauss(void)
{
	return sqrt(-2.0*log(rand_norm())) * cos(2.0*CONST_PI*rand_norm());
}

/* sample from Y = (X_1 + X_2) / 2, with X_1 ~ Exp(see above),
 * X_2 ~ Norm(0,1)
 */
double sample_sum2(void)
{
	return (sample_exp() + sample_gauss()) / 2.0;
}

/* sample from Z = (X_1 + X_2 + X_3) / 3, with X_1 ~ Exp(see above),
 * X_2 ~ Norm(0,1), X_3 ~ Uniform([-(1.5)^(1/3),(1.5)^(1/3)])
 */
double sample_sum3(void)
{
	return (sample_exp() + sample_gauss() +
		(2.0*rand_norm() - 1.0) * pow(1.5, 1.0/3.0)) / 3.0;
}

int main(void)
{
	FILE *fout;

	fout = fopen("result_sum2", "w");
	if (fout == NULL) {
		perror("fopen() result_sum2");
		abort();
	}
	count_freq(fout, sample_sum2);
	fclose(fout);

	fout = fopen("result_sum3", "w");
	if (fout == NULL) {
		perror("fopen() result_sum3");
		abort();
	}
	count_freq(fout, sample_sum3);
	fclose(fout);

	return 0;
}
