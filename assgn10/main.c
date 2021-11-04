#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "count_freq.h"

#undef NDEBUG
#include <assert.h>

/* uniform dist. over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* sample from uniform distribution over S = {i | !occupied[i]}
 * using the acceptance-rejection algorithm
 */
int sample_uniform(bool occupied[4])
{
	int idx;

	do {
		idx = (int) (rand_norm() * 4.0);
	} while (occupied[idx]);

	return idx;
}

/* take the next step based on previous results
 * returns false if no steps could be taken
 * otherwise returns true and update *plast_x and *plast_y
 */
bool take_next_step(bool *plattice, int dim,
		    int *plast_x, int *plast_y)
{
	bool (*lattice)[dim] = (bool (*)[dim]) plattice;
	int delta_x[4] = {0, 0, 1, -1}; /* 0 -- up, 1 -- down */
	int delta_y[4] = {1, -1, 0, 0}; /* 2 -- right, 3 -- left */
	bool occupied[4] = {false, false, false, false};
	int i, nrway;		/* nrway -- number of possible directions */
	int last_x = *plast_x, last_y = *plast_y;
	int new_x, new_y;

	assert(lattice[last_x][last_y] == true);

	/* check for possible next directions */
	for (i = 0; i < 4; i++) {
		new_x = last_x + delta_x[i];
		new_y = last_y + delta_y[i];
		if ( (new_x < 0) || (new_x >= dim)
		     || (new_y < 0) || (new_y >= dim)
		     || (lattice[new_x][new_y] == true) ) {
			occupied[i] = true;
		} else {
			occupied[i] = false;
		}
	}

	/* count possible next directions */
	nrway = 0;
	for (i = 0; i < 4; i++) {
		if (!occupied[i])
			nrway++;
	}
	/* die if no step could be taken */
	if (nrway == 0)
		return false;

	/* else take the next step and live strong */
	i = sample_uniform(occupied);
	new_x = last_x + delta_x[i];
	new_y = last_y + delta_y[i];
	lattice[new_x][new_y] = true;
	*plast_x = new_x;
	*plast_y = new_y;
	return true;
}

/* perform growing-self-avoiding random walk on a plattice with given dim.x
 * nsteps -- desired steps to walk
 * if not null, writes to fout the steps taken (x-y coordinates)
 * if not null, writes to *pdist \langle r^2\rangle
 * returns actual steps of walking
 */
int gsa_walk(FILE *fout, int dim, int nsteps, double *pdist)
{
	bool (*lattice)[dim] = malloc(dim * dim * sizeof(bool));
	int asteps;		/* actual steps of walking */
	int i, j;
	int last_x, last_y;
	double sum_r2;
	bool alive;

	assert(dim > 0 && nsteps > 0);
	if (lattice == NULL) {
		perror("malloc() lattice");
		abort();
	}

	/* initialize lattice to false */
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			lattice[i][j] = false;
		}
	}

	/* take steps */
	alive = true;
	last_x = last_y = dim / 2;
	lattice[last_x][last_y] = true;
	asteps = 0;
	sum_r2 = 0;
	while (alive && asteps < nsteps) {
		if (fout != NULL) {
			fprintf(fout, "%d %d\n", last_x, last_y);
		}
		sum_r2 += pow(last_x - dim/2, 2)
			+ pow(last_y - dim/2, 2);
		alive = take_next_step((bool *) lattice, dim,
				       &last_x, &last_y);
		asteps++;
	}

	if (pdist != NULL) {
		*pdist = sum_r2 / asteps;
	}

	free(lattice);
	return asteps;
}

int main()
{
	FILE *fout;
	int i;
	double dist;
	int asteps;

	srand(time(NULL));
	do {
		fout = fopen("res", "w");
		if (fout == NULL) {
			perror("fopen() res");
			abort();
		}
		asteps = gsa_walk(fout, 512, 1024, NULL);
		fclose(fout);
	} while (asteps < 100);

	fout = fopen("freq", "w");
	count_freq(fout, gsa_walk, 0, 256);
	fclose(fout);

	fout = fopen("exp", "w");
	for (i = 0; i < 1024; i++) {
		asteps = gsa_walk(NULL, 512, 1024, &dist);
		fprintf(fout, "%.12f %.12f\n", log(asteps), log(dist));
	}
	fclose(fout);

	return 0;
}
