#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#undef NDEBUG
#include <assert.h>

/* uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* sample from uniform distribution over the set {0, 1, 2, 3}
 * which stands for 4 different direction (see random_Walk())
 */
int sample_unif_dir(void)
{
	int dir = rand_norm() * 4.0;
	assert(dir >= 0 && dir <= 4);
	if (dir == 4)
		dir = 3;
	return dir;
}

#define CONST_PI	(4.0*atan(1.0))

/* do the random walk over the given lattice, returns true if
 * the particle meets the cluster, returns false if the particle
 * falls outside the boundary circle
 * if the particle meets the cluster, *px and *py will also be set
 * to the final position of the particle
 */
bool random_walk(bool *plattice, int dim, double max_radius,
		 int *px, int *py)
{
	bool (*lattice)[dim] = (bool (*)[dim]) plattice;
	int delta_x[4] = {0, 0, 1, -1}; /* 0 -- up, 1 -- down */
	int delta_y[4] = {1, -1, 0, 0}; /* 2 -- right, 3 -- left */
	int i, x, y;

	assert(plattice != NULL);
	assert(dim > 0);
	assert(max_radius > 0);
	assert(px != NULL);
	assert(py != NULL);
	assert(max_radius < dim/2);

	x = *px;
	y = *py;
	do {
		i = sample_unif_dir();
		x += delta_x[i];
		y += delta_y[i];
		if (lattice[x][y]) {
			*px = x - delta_x[i];
			*py = y - delta_y[i];
			return true;
		}
	} while ( (0 <= x) && (x < dim) && (0 <= y) && (y < dim) &&
		  (pow(x-dim/2,2) + pow(y-dim/2,2) < pow(max_radius, 2)) );
	return false;
}

/* perform DLA simulation */
void dla_simulation(bool *plattice, int dim, double radius, int nparticles)
{
	bool (*lattice)[dim] = (bool (*)[dim]) plattice;
	int i, j, x, y;
	double theta;

	assert(plattice != NULL);
	assert(dim > 0);
	assert(radius > 0);
	assert(nparticles > 0);
	assert(2 * radius < dim / 2);

	/* initialize the cluster */
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			lattice[i][j] = false;
	lattice[dim/2][dim/2] = true;

	/* perform the prescribed random walks */
	for (i = 0; i < nparticles; i++) {
		do {
			theta = rand_norm() * 2.0 * CONST_PI;
			x = dim/2 + radius * cos(theta);
			y = dim/2 + radius * sin(theta);
		} while (!random_walk((bool *) lattice,
				      dim, 2.0 * radius, &x, &y));
		lattice[x][y] = true;
	}
}

#define CLUSTER_DIM	(1024)
int main(void)
{
	bool (*lattice)[CLUSTER_DIM] = (bool (*)[CLUSTER_DIM])
		malloc(CLUSTER_DIM * CLUSTER_DIM * sizeof(bool));
	FILE *fout = fopen("res", "w");
	int i, j;

	if (fout == NULL) {
		perror("fopen() res");
		abort();
	}
	if (lattice == NULL) {
		perror("malloc() lattice");
		abort();
	}

	dla_simulation((bool *) lattice, CLUSTER_DIM,
		       CLUSTER_DIM/4 - 1, 4096);
	for (i = 0; i < CLUSTER_DIM; i++)
		for (j = 0; j < CLUSTER_DIM; j++)
			if (lattice[i][j])
				fprintf(fout, "%d %d\n", i, j); 

	fclose(fout);
	free(lattice);
	return 0;
}
