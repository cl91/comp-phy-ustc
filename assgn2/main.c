/* main.c -- Assignment 2
 * PB09203226, Chang Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#undef NDEBUG
#include <assert.h>

#define NR_DOORS 3

/* initialize the configuration of lions and girl */
void place_lions_and_girl(bool *is_girl)
{
	int i;

	for (i = 0; i < NR_DOORS; i++) {
		is_girl[i] = false;
	}
	is_girl[rand() / (RAND_MAX / NR_DOORS)] = true;
}

/* the gladiator makes his choice */
int gladiator_chooses_the_door(void)
{
	return rand() / (RAND_MAX / NR_DOORS);
}

/* the emperor opens one of the other doors with lion */
int emperor_opens_the_door_with_lion(int door_gladiator, bool *is_girl)
{
	int i;

	for (i = 0; i < NR_DOORS; i++) {
		if (i != door_gladiator && !is_girl[i])
			return i;
	}
	assert(false);		/* won't reach here */
	return -1;
}

/* let's see what the gladiator will get */
bool gladiator_gets_the_girl(bool no_change, int door_glad, bool *is_girl)
{
	if (no_change) {	/* if the gladiator retains his choice */
		return is_girl[door_glad];
	} else {		/* if the gladiator changes his mind */
		return !is_girl[door_glad];
	}
}

#define NR_STEPS (1<<15)
int main(void)
{
	bool is_girl[NR_DOORS];	/* stores the configration of lions and girl */
	int door_gladiator;	/* door chosen by the gladiator */
	int door_emperor;	/* door opened by the emperor */
	int nr_got_girl;	/* number of times the glad. got the girl */
	int i;

	srand(time(NULL));	/* initialize the seed */

	/* compute the frequency the gladiator got the girl
	   if he retains his choice */
	nr_got_girl = 0;
	for (i = 0; i < NR_STEPS; i++) {
		place_lions_and_girl(is_girl);
		door_gladiator = gladiator_chooses_the_door();
		door_emperor = emperor_opens_the_door_with_lion(
			door_gladiator, is_girl);
		if (gladiator_gets_the_girl(true, door_gladiator, is_girl))
			nr_got_girl++;
	}
	printf("If the gladiator retains his choice:\n"
	       "    Number of steps: %d\n"
	       "    Number of instances he got the girl: %d\n"
	       "    Frequency: %f\n\n", NR_STEPS, nr_got_girl,
	       (double) nr_got_girl / (double) NR_STEPS);

	/* now compute the frequency the gladiator got the girl
	   if he changes his mind */
	nr_got_girl = 0;
	for (i = 0; i < NR_STEPS; i++) {
		place_lions_and_girl(is_girl);
		door_gladiator = gladiator_chooses_the_door();
		door_emperor = emperor_opens_the_door_with_lion(
			door_gladiator, is_girl);
		if (gladiator_gets_the_girl(false, door_gladiator, is_girl))
			nr_got_girl++;
	}
	printf("If the gladiator changes his mind:\n"
	       "    Number of steps: %d\n"
	       "    Number of instances he got the girl: %d\n"
	       "    Frequency: %f\n", NR_STEPS, nr_got_girl,
	       (double) nr_got_girl / (double) NR_STEPS);

	return 0;
}
