/*
* @@name:	dining_philosophers_locking_hierarchy.c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define NUM_PHIL 10

// define macros for finding the minimum and the maximum of two values
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

pthread_mutex_t fork_locks[NUM_PHIL];
int exit_flag = 0;

void* philosopher(void* argc)
{
  	int id = *((int*) argc);         // id of philosopher
  	int left = id;                   // id of lock for left fork
  	int right = (id + 1) % NUM_PHIL; // id of lock for right fork

	while(!exit_flag)
	{
		int first = min(left, right);   // take the fork with the lower id first
		int second = max(left, right);  // and the fork with the higher id last

		/* lock first fork */
		pthread_mutex_lock(&fork_locks[first]);

		/* simulate picking up the first fork */
		printf("Philosopher %d picked up first fork %d.\n", id, first); fflush(stdout);

		/* lock second fork */
		pthread_mutex_lock(&fork_locks[second]);

		printf("Philosopher %d picked up second fork %d.\n", id, second); fflush(stdout);

		printf("Philosopher %d is eating.\n",id); fflush(stdout);

		/*unlock second fork */
		pthread_mutex_unlock(&fork_locks[second]);
		printf("Philosopher %d put back second fork.\n",id); fflush(stdout);

		/* unlock second fork */
		pthread_mutex_unlock(&fork_locks[first]);
		printf("Philosopher %d put back first fork.\n",id); fflush(stdout);

		printf("Philosopher %d is thinking.\n",id); fflush(stdout);
	}
}

int main(int argc, char const *argv[])
{
	int i = 0;

	// initialise the locks for all forks
	for (i = 0; i < NUM_PHIL; ++i)
	{
		pthread_mutex_init(&fork_locks[i], NULL);
	}

    // create a thread for each philosopher
	pthread_t threads[NUM_PHIL];
	int ids[NUM_PHIL];

	for (i = 0; i < NUM_PHIL; ++i)
	{
		ids[i] = i;
		pthread_create(&threads[i], NULL, philosopher, &ids[i]);
	}

	// wait for key press, then tell threads to exit
	getchar();
	exit_flag = 1;

 	// wait for all threads
	for (i = 0; i < NUM_PHIL; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	// destroy the locks for the forks
	for (i = 0; i < NUM_PHIL; ++i)
	{
		pthread_mutex_destroy(&fork_locks[i]);
	}

	return EXIT_SUCCESS;
}
