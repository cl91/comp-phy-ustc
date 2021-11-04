#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#undef NDEBUG
#include <assert.h>

#define CONST_PI	(atan(1.0)*4.0)

/* macros for packing and unpacking directional information to an int */
#define get_incoming(x, k)	((x) & (1 << (k)))
#define set_incoming(x, k)	((x) |= (1 << (k)))
#define unset_incoming(x, k)	((x) &= ~(1 << (k)))

/* 0th bit of an int indicates particle content coming from the left
 * 1th -- right, 2 -- up, 3 -- down
 * NDIR -- number of total directions
 */
enum { LEFT, RIGHT, UP, DOWN, NDIR };

/* take the next step based on previous simulation results
 * needs two lattices, old lattice --- poldlat, new lattice --- pnewlat
 */
void take_next_step(char *poldlat, char *pnewlat, int dim)
{
	assert(poldlat != NULL);
	assert(pnewlat != NULL);
	assert(dim > 0);

	char (*oldlat)[dim] = (char (*)[dim]) poldlat;
	char (*newlat)[dim] = (char (*)[dim]) pnewlat;
	int i, j;

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			char incoming[NDIR];
			int k;
			for (k = 0; k < NDIR; k++)
				incoming[k] = get_incoming(oldlat[i][j], k);
			/* rules for HPP model: */
			/* \rightattow\leftarrow  =>  \uparrow\downarrow */
			if (incoming[LEFT] && incoming[RIGHT]
			    && i != 0 && i != dim-1) {
				set_incoming(newlat[i-1][j], DOWN);
				set_incoming(newlat[i+1][j], UP);
				unset_incoming(oldlat[i][j], LEFT);
				unset_incoming(oldlat[i][j], RIGHT);
				incoming[LEFT] = 0;
				incoming[RIGHT] = 0;
			}
			/* \downarrow\uparrow => \leftarrow\rightarrow */
			if (incoming[UP] && incoming[DOWN]
			    && j != 0 && j != dim-1) {
				set_incoming(newlat[i][j-1], RIGHT);
				set_incoming(newlat[i][j+1], LEFT);
				unset_incoming(oldlat[i][j], UP);
				unset_incoming(oldlat[i][j], DOWN);
				incoming[UP] = 0;
				incoming[DOWN] = 0;
			}
			/* rules 1: particles pass right through */
			if (incoming[LEFT] && !incoming[RIGHT]
			    && j != dim-1) {
				set_incoming(newlat[i][j+1], LEFT);
				unset_incoming(oldlat[i][j], LEFT);
				incoming[LEFT] = 0;
			}
			if (incoming[RIGHT] && !incoming[LEFT]
			    && j != 0) {
				set_incoming(newlat[i][j-1], RIGHT);
				unset_incoming(oldlat[i][j], RIGHT);
				incoming[RIGHT] = 0;
			}
			if (incoming[UP] && !incoming[DOWN]
			    && i != dim-1) {
				set_incoming(newlat[i-1][j], UP);
				unset_incoming(oldlat[i][j], UP);
				incoming[UP] = 0;
			}
			if (incoming[DOWN] && !incoming[UP]
			    && i != 0) {
				set_incoming(newlat[i+1][j], DOWN);
				unset_incoming(oldlat[i][j], DOWN);
				incoming[DOWN] = 0;
			}
			/* boundary rules -- reflection at boundary */
			if (j == 0 && incoming[RIGHT]) {
				set_incoming(newlat[i][j], LEFT);
				unset_incoming(oldlat[i][j], RIGHT);
			}
			if (j == dim-1 && incoming[LEFT]) {
				set_incoming(newlat[i][j], RIGHT);
				unset_incoming(oldlat[i][j], LEFT);
			}
			if (i == 0 && incoming[DOWN]) {
				set_incoming(newlat[i][j], UP);
				unset_incoming(oldlat[i][j], DOWN);
			}
			if (i == dim-1 && incoming[UP]) {
				set_incoming(newlat[i][j], DOWN);
				unset_incoming(oldlat[i][j], UP);
			}
		}
	}
}

/* convert angle to direction
 *     U
 *   L   R
 *     D
 */
int conv_angle_to_direc(double angle)
{
	if (angle > CONST_PI / 4 && angle < CONST_PI * 3 / 4)
		return UP;
	if (angle >= CONST_PI * 3 / 4 && angle < CONST_PI * 5 / 4)
		return LEFT;
	if (angle >= CONST_PI * 5 / 4 && angle < CONST_PI * 7 / 4)
		return DOWN;
	return RIGHT;
}

/* Do the HPP simulation, initial configuration is a ring of radius
 * r_0 to r_1
 */
char *hpp_simulation(int dim, double r0, double r1, int nsteps)
{
	assert(dim > 0);
	assert(nsteps >= 0);

	char (*oldlat)[dim] = (char (*)[dim])
		malloc(dim * dim * sizeof(char));
	char (*newlat)[dim] = (char (*)[dim])
		malloc(dim * dim * sizeof(char));
	if (oldlat == NULL || newlat == NULL) {
		perror("malloc (lattice)");
		abort();
	}

	/* initialize the lattice */
	double radius, angle;
	memset(oldlat, 0, dim * dim * sizeof(char));
	memset(newlat, 0, dim * dim * sizeof(char));
	for (radius = r0; radius < r1; radius += 0.5) {
		for (angle = 0; angle < 2*CONST_PI; angle += 0.5/radius) {
			int direc = conv_angle_to_direc(angle);
			int x = radius * cos(angle) + dim/2;
			int y = radius * sin(angle) + dim/2;
			set_incoming(oldlat[y][x], direc);
		}
	}

	/* do the simulation */
	int i;
	char (*tmp)[dim];
	for (i = 0; i < nsteps; i++) {
		take_next_step((char *) oldlat, (char *) newlat, dim);
		/* swap oldlat and newlat */
		tmp = oldlat;
		oldlat = newlat;
		newlat = tmp;
	}

	free(newlat);
	return (char *) oldlat;
}

/* write the particle content of the given lattice to a file */
void dump_lattice_to_file(FILE *fout, char *plattice, int dim)
{
	assert(fout != NULL);
	assert(plattice != NULL);
	assert(dim > 0);

	char (*lattice)[dim] = (char (*)[dim]) plattice;
	int i, j;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			if (lattice[i][j])
				fprintf(fout, "%d %d\n", j, i);
}

/* write the particle content of the given lattice to stdout */
void dump_lattice_to_stdout(char *plattice, int dim)
{
	assert(plattice != NULL);
	assert(dim > 0);
	char (*lattice)[dim] = (char (*)[dim]) plattice;
	int i, j;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			if (lattice[i][j])
				printf("lattice[%d][%d]: %x\n",
				       i, j, lattice[i][j]);
}


/* unit test for take_next_step(), tests rules 1 */
#define test_single(dir)	do {				\
		int dim = 8;					\
		char oldlat[dim][dim];				\
		char newlat[dim][dim];				\
		memset(oldlat, 0, sizeof oldlat);		\
		memset(newlat, 0, sizeof newlat);		\
		set_incoming(oldlat[4][4], dir);		\
		printf("before: (direction == " #dir ")\n");	\
		dump_lattice_to_stdout((char *) oldlat, dim);	\
		printf("after:\n");				\
		take_next_step((char *) oldlat,			\
			       (char *) newlat, dim);		\
		dump_lattice_to_stdout((char *) newlat, dim);	\
		putchar('\n');					\
		fflush(stdout);					\
	} while (0)

/* unit test for take_next_step(), tests rule 2, 3 */
#define test_double(dir1, dir2)	do {				\
		int dim = 8;					\
		char oldlat[dim][dim];				\
		char newlat[dim][dim];				\
		memset(oldlat, 0, sizeof oldlat);		\
		memset(newlat, 0, sizeof newlat);		\
		set_incoming(oldlat[4][4], dir1);		\
		set_incoming(oldlat[4][4], dir2);		\
		printf("before: (direction == " #dir1		\
		       ", " #dir2 ")\n");			\
		dump_lattice_to_stdout((char *) oldlat, dim);	\
		printf("after:\n");				\
		take_next_step((char *) oldlat,			\
			       (char *) newlat, dim);		\
		dump_lattice_to_stdout((char *) newlat, dim);	\
		putchar('\n');					\
		fflush(stdout);					\
	} while (0)

/* unit test for take_next_step() */
void test_take_next_step(void)
{
	test_single(LEFT);
	test_single(RIGHT);
	test_single(UP);
	test_single(DOWN);
	test_double(LEFT, RIGHT);
	test_double(LEFT, UP);
	test_double(UP, DOWN);
	test_double(RIGHT, DOWN);
}

/* just as the name suggests: */
#define plot_lattice_versus_time(dim, r0, r1, nsteps)		do {	\
		char filename[128];					\
		snprintf(filename, sizeof filename,			\
			 "res_%03d", nsteps);				\
		fout = fopen(filename, "w");				\
		if (fout == NULL) {					\
			perror("fopen()");				\
			abort();					\
		}							\
		dump_lattice_to_file(fout,				\
				     hpp_simulation(dim, r0, r1,	\
						    nsteps), dim);	\
		fclose(fout);						\
	} while (0)

int main(void)
{
	FILE *fout;
	int dim = 256;
	double r0 = 30, r1 = 40;
	int nsteps;

	for (nsteps = 1; nsteps <= 32; nsteps *= 2)
		plot_lattice_versus_time(dim, r0, r1, nsteps);
	for (nsteps = 40; nsteps < 200; nsteps += 10)
		plot_lattice_versus_time(dim, r0, r1, nsteps);

	return 0;
}
