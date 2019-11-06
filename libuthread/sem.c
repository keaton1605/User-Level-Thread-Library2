#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "thread.h"

struct semaphore {
	queue_t blockedQueue;
	int count;
	/* TODO Phase 1 */
};

sem_t sem_create(size_t count)
{
	enter_critical_section();

	sem_t newSem = malloc(sizeof(sem_t));
	newSem->count = count;
	newSem->blockedQueue = queue_create();

	exit_critical_section();	

	return newSem;
	/* TODO Phase 1 */
}

int sem_destroy(sem_t sem)
{
	/* check if sem exists */
	if (sem == NULL)
		return -1;
	/* Check if blocked queue is not empty */
	else if (queue_destroy(sem->blockedQueue) == -1)
		return -1;	

	free(sem);

	return 0;
	/* TODO Phase 1 */
}

int sem_down(sem_t sem)
{
	pthread_t tid;

	/* Check if sem exists */
	if (sem == NULL)
		return -1;

	enter_critical_section();

	if (sem->count == 0)
	{
		tid = pthread_self();
		queue_enqueue(sem->blockedQueue, (void*)tid);
		thread_block();
	}
	sem->count--;

	exit_critical_section();
	return 0;
	/* TODO Phase 1 */
}

int sem_up(sem_t sem)
{
	pthread_t tid;

	/* Check if sem exists */
	if (sem == NULL)
		return -1;

	enter_critical_section();

	if (queue_length(sem->blockedQueue) != 0)
	{
		queue_dequeue(sem->blockedQueue, (void**)&tid);
		thread_unblock(tid);
	}

	sem->count++;

	exit_critical_section();
	return 0;
	/* TODO Phase 1 */
}

int sem_getvalue(sem_t sem, int *sval)
{
	int numInQueue = 0;

	if (sem == NULL || sval == NULL)
		return -1;

	enter_critical_section();

	if (sem->count > 0)
		*sval = sem->count;
	else if (sem->count == 0)
	{
		numInQueue = queue_length(sem->blockedQueue);
		*sval = -numInQueue;
	}

	exit_critical_section();

	return 0;
	/* TODO Phase 1 */
}

