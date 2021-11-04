#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "schrage.h"
#include "randomz.h"

void test_rand(int nsteps, FILE *fout, double (*prand)(double))
{
	double z;
	double z_next;
	int i;
	uint32_t x;

	/* setting seed */
	x = time(NULL);
	z = prand( (double) x / (double) UINT32_MAX );

	for (i = 0; i < nsteps; i++) {
		z_next = prand(z); /* generate random number */
		fprintf(fout, "%.12f\t%.12f\n", z, z_next);
		z = z_next;
	}
}

int main(int argc, char **argv)
{
	int nsteps;
	FILE *fout;

	/* parse command line to get the
	   number of random numbers to generate */
	if (argc != 2 || atoi(argv[1]) <= 0) {
		fprintf(stderr,
			"usage: %s NUMBER_OF_DATA_POINTS_TO_GENERATE\n",
			argv[0]);
		abort();
	}
	nsteps = atoi(argv[1]);

	fout = fopen("result_schrage", "w");
	if (fout == NULL) {
		perror("fopen(result_schrage) fails");
		abort();
	}
	test_rand(nsteps, fout, rand_schrage);
	fout = fopen("result_randomz", "w");
	if (fout == NULL) {
		perror("fopen(result_randomz) fails");
		abort();
	}
	test_rand(nsteps, fout, randomz);
	return 0;
}
