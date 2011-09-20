/*
 * sch_data_acq.c
 *
 *  Created on: 20.09.2011
 *      Author: prilag
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

extern sem_t scheduler_tick;

void*
scheduler(void *arg) {
	//int sval;
	int sampletime;

	sampletime = *(int*) arg;

	//printf("scheduling: %d sec\n", sampletime);

	while (1) {
		//sem_getvalue(&scheduler_tick, &sval);
		//printf("scheduler | s: %d\n", sval);
		sleep(sampletime);
		sem_post(&scheduler_tick);
	}

	pthread_exit(0);
}
