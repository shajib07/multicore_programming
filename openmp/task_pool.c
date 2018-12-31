/*
* @@name:	task_pool.c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/

#include <stdio.h>
#include <omp.h>

#define MAX_TASK_INDEX 10
int current_task_index = -1;

int nextTask() {
	int t = -1;
#pragma omp critical(nextTask)
	{
		if(current_task_index < MAX_TASK_INDEX)
		t = current_task_index++;

	}
	return t;
}

void runTask(int task) 
{
	printf("Thread %d executes task %d \n",omp_get_thread_num(), task);
}

int main()
{
#pragma omp parallel num_threads(4)
    {
        int task = nextTask();

        while (task != -1)
        {
            runTask(task);
            task = nextTask();
        }
    }
	return 0;
}
