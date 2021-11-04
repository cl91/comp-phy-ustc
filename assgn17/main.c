#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>

/* number of particles */
#define NR_PARTICLE	(1000)
/* mass of each particle */
#define MASS_PARTICLE	(2.0)

/* data structure for the configuration of the system */
struct config {
	double v[NR_PARTICLE];
	double e_demon;
};

/* Uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* Uniform distribution over [-h,h] */
double rand_unif(double h)
{
	return h * (2.0 * rand_norm() - 1.0);
}

/* Uniform distribution over [0,NR_PARTICLE-1]\cap\mathbb{Z} */
int rand_index(void)
{
	int i = rand_norm() * NR_PARTICLE;
	if (i == NR_PARTICLE)
		return NR_PARTICLE - 1;
	else
		return i;
}

/* Energy function (Hamiltonian) for a single free particle in 1D:
 * U = \frac{1}{2}m v^2
 */
double get_energy(double v)
{
	return v * v * 0.5 * MASS_PARTICLE;
}

/* Compute next configuration based on previous one
 * Stores result in cfg
 */
void get_next_config(struct config *cfg, double h)
{
	int index = rand_index();
	double new_v = cfg->v[index] + rand_unif(h);
	double delta_u = get_energy(new_v) - get_energy(cfg->v[index]);

	if (delta_u < 0 || cfg->e_demon > delta_u) {
		cfg->e_demon -= delta_u;
		cfg->v[index] = new_v;
	}
}

int main(int argc, char **argv)
{
	if (argc != 7) {
		fprintf(stderr, "usage: %s initv freq e_demon nsteps h temp\n"
			"    initv: initial velocity of every particle\n"
			"    freq: file to which frequency counting "
			"is written\n"
			"    e_demon: initial demon energy\n"
			"    nsteps: number of iteration steps\n"
			"    h: length of each step\n"
			"    temp: file to which temperature is written\n",
			argv[0]);
		exit(1);
	}

	double init_v = strtod(argv[1], NULL);
	struct config *cfg = (struct config *) malloc(sizeof *cfg);
	cfg->e_demon = strtod(argv[3], NULL);
	int i;
	for (i = 0; i < NR_PARTICLE; i++)
		cfg->v[i] = init_v;

	int nsteps = atoi(argv[4]);
	srand(time(NULL));
	for (i = 0; i < nsteps; i++) {
		get_next_config(cfg, strtod(argv[5], NULL));
	}

	double min_v = cfg->v[0], max_v = cfg->v[0];
	for (i = 1; i < NR_PARTICLE; i++) {
		if (min_v > cfg->v[i])
			min_v = cfg->v[i];
		if (max_v < cfg->v[i])
			max_v = cfg->v[i];
	}

	FILE *fout = fopen(argv[2], "w");
	if (fout == NULL) {
		err(1, "fopen(\"%s\") failed", argv[2]);
	}
	void count_freq(FILE *fout, double *v, int nr_particle,
			double lcutoff, double rcutoff);
	count_freq(fout, cfg->v, NR_PARTICLE, min_v, max_v);
	fclose(fout);

	fout = fopen(argv[6], "w");
	if (fout == NULL) {
		err(1, "fopen(\"%s\") failed", argv[6]);
	}
	double sum = 0.0;
	for (i = 0; i < NR_PARTICLE; i++) {
		sum += get_energy(cfg->v[i]);
	}
	fprintf(fout, "%.12f\n", 2.0 * sum);
	fclose(fout);

	printf("e_demon = %.5f\n", cfg->e_demon);

	return 0;
}
