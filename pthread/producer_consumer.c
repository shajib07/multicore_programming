/*
* @@name:	producer_consumer.c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 1000000

pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;

int buffer = 0;

void* producer(void* argc)
{
	for (int i = 0; i < MAX; ++i)
	{
		pthread_mutex_lock(&the_mutex);
		while(buffer != 0)
		{
			pthread_cond_wait(&condp, &the_mutex);
			printf("producer is waiting ...\n");
		}

		buffer = i;
		pthread_cond_signal(&condc);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}

void* consumer(void* argc)
{
	for (int i = 0; i < MAX; ++i)
	{
		pthread_mutex_lock(&the_mutex);
		while(buffer == 0)
		{
			pthread_cond_wait(&condc, &the_mutex);
			printf("consumer is waiting ...\n");
		}

		buffer = 0;
		pthread_cond_signal(&condp);
		pthread_mutex_unlock(&the_mutex);
	}
	pthread_exit(0);
}


int main(int argc, char const *argv[])
{
	pthread_t prod, con;

	pthread_mutex_init(&the_mutex, 0);

	pthread_cond_init(&condc, 0);
	pthread_cond_init(&condp, 0);

	pthread_create(&prod, NULL, producer, 0);
	pthread_create(&con, NULL, consumer, 0);

	pthread_join(prod, 0);
	pthread_join(con, 0);

	pthread_cond_destroy(&condc);
	pthread_cond_destroy(&condp);

	pthread_mutex_destroy(&the_mutex);	
	return 0;
}
