#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#undef NDEBUG
#include <assert.h>

struct point {
	double x, y;
};

/* uniform distribution over [0,1] */
double rand_norm(void)
{
	return (double) rand() / (double) RAND_MAX;
}

/* uniform distribution over [-h,h] */
double rand_unif(double h)
{
	return h * (2*rand_norm() - 1);
}

/* Probability distribution for our canonical ensemble with
 * Hamiltonian H(x,y) = x^2/2 + y^2/2
 */
double prob_dist(struct point p)
{
	return exp(-p.x*p.x/2 - p.y*p.y/2);
}

/* determines if the new configuration R_n is accepted
 * returns true if p = \frac{W(R_n)}{W(R_o)} > rand_norm()
 */
bool is_accepted(double p)
{
	if (p > rand_norm())
		return true;
	else
		return false;
}

/* generate a new configuration based upon the above accepting criterion */
struct point generate_new_configuration(double h, struct point old)
{
	double p;
	struct point new;
	double x = rand_unif(h);
	double y = rand_unif(h);
	new.x = old.x + x;
	new.y = old.y + y;
	p = prob_dist(new) / prob_dist(old);
	if (is_accepted(p))
		return new;
	else
		return old;
}

/* Numerically evaluate the integral
 * I = \frac{\int_{\mathbb{R}^2} p(x)f(x)\, {\rm d}^d x}
 *     {\int_{\mathbb{R}^2} p(x)\, {\rm d}^2 x}		(*)
 * where p(x,y) = \exp(-\frac{x^2}{2}-\frac{y^2}{2}) is the probability
 * distribution for a canonical ensemble with H = x^2/2 + y^2/2
 * using the Metropolis algorithm
 * f(x,y) is given by the function pointer |fcn|
 * Points are taken at time ni + nd * k, where k = 0,1,\ldots,nsteps-1
 */
/* initial configuration */
#define INIT_CONFIG_X	(1.0)
#define INIT_CONFIG_Y	(1.0)
/* static global variable for output file to which all R_n are written */
static FILE *fout_all = NULL;
static FILE *fout_hit = NULL;
/* write all R_n to fout_all, write those R_n which are used to compute
 * the final value of the integral to fout_hit
 */
#define spit_all(cfg)	{						\
		if (fout_all != NULL) {					\
			fprintf(fout_all, "%.5f %.5f\n", cfg.x, cfg.y);	\
		}							\
	}
#define spit_hit(cfg)	{						\
		if (fout_hit != NULL) {					\
			fprintf(fout_hit, "%.5f %.5f\n", cfg.x, cfg.y);	\
		}							\
	}
double metropolis(double (*fcn)(struct point),
		  int ni, int nd, int nsteps, double h)
{
	struct point cfg = {INIT_CONFIG_X, INIT_CONFIG_Y};
	int i, j;
	double sum;

	/* skip initial ni points */
	for (i = 0; i < ni; i++) {
		cfg = generate_new_configuration(h, cfg);
		spit_all(cfg);
	}
	sum = fcn(cfg);
	spit_hit(cfg);
	/* compute the sum \sum_{rm nsteps} A(R_i) */
	for (i = 1; i < nsteps; i++) {
		for (j = 0; j < nd; j++) {
			cfg = generate_new_configuration(h, cfg);
			spit_all(cfg);
		}
		sum += fcn(cfg);
		spit_all(cfg);
		spit_hit(cfg);
	}
	return sum / nsteps;
}

double fcn_x2(struct point p)
{
	return p.x * p.x;
}

double fcn_y2(struct point p)
{
	return p.y * p.y;
}

double fcn_x2_y2(struct point p)
{
	return p.x*p.x + p.y*p.y;
}

int main(void)
{
	srand(time(NULL));

	FILE *fout;
	fout_all = fopen("markov_all", "w");
	fout_hit = fopen("markov_hit", "w");
	fout = fopen("res_x2", "w");
	fprintf(fout, "\\langle x^2\\rangle = %.12f\n",
		metropolis(fcn_x2, 1000, 15, 10000, 10));
	fclose(fout);
	fclose(fout_all);
	fclose(fout_hit);
	fout_all = NULL;
	fout_hit = NULL;

	fout = fopen("res_y2", "w");
	fprintf(fout, "\\langle y^2\\rangle = %.12f\n",
		metropolis(fcn_y2, 1000, 15, 10000, 10));
	fclose(fout);

	fout = fopen("res_x2_y2", "w");
	fprintf(fout, "\\langle x^2+y^2\\rangle = %.12f\n",
		metropolis(fcn_x2_y2, 1000, 15, 10000, 10));
	fclose(fout);

	return 0;
}
