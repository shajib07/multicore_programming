/*
* @@name:	dining_philosophers_backoff_strategy.c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define NUM_PHIL 10

pthread_mutex_t fork_locks[NUM_PHIL];
int exit_flag = 0;

void* philosopher(void* argc) 
{
	int id = *((int*) argc);
	int left = id;
	int right = (id + 1) % NUM_PHIL;

	while(exit_flag == 0)
	{
	    	int has_eaten = 0;
		int first = left;
		int second = right;

		/* lock first fork */
		pthread_mutex_lock(&fork_locks[first]);

	    	printf("Philosopher %d picked up first fork %d.\n", id, first); fflush(stdout);

	    	/* try to lock second fork */
		int ret = pthread_mutex_trylock(&fork_locks[second]);
		if (ret == EBUSY) 
		{
	      	/* picking up the second fork was not successful */
	      	printf("Philosopher %d cannot get second fork %d. Will put back first fork.\n", id, second); fflush(stdout);
		}
		else
		{
	      	/* simulate picking up the second fork */
	      	printf("Philosopher %d picked up second fork %d.\n", id, second); fflush(stdout);
  	      	
  	      	has_eaten = 1;
	      	printf("Philosopher %d is eating.\n",id); fflush(stdout);

	    	/*unlock second fork */
	    	pthread_mutex_unlock(&fork_locks[second]);
      		printf("Philosopher %d put back second fork.\n",id); fflush(stdout);
		}
		
		/* unlock second fork */
	    	pthread_mutex_unlock(&fork_locks[first]);
	    	printf("Philosopher %d put back first fork.\n",id); fflush(stdout);

	    	// think only after eating
	    	if (has_eaten == 1)
	      		printf("Philosopher %d is thinking.\n",id); fflush(stdout);
	}

}

	
int main(int argc, char **argv)
{
	int i;
	
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
  	for (int i = 0; i < NUM_PHIL; ++i)
  	{
  		pthread_mutex_destroy(&fork_locks[i]);
  	}
	return EXIT_SUCCESS;
}
