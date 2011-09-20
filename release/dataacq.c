/*
 * dataacq.c
 *
 *  Created on: 20.09.2011
 *      Author: prilag
 */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

extern sem_t scheduler_tick;

void *
dataacq(void *arg) {
	int sval = 100;

	while (1) {
		sem_getvalue(&scheduler_tick, &sval);
		printf("dataacq | s: %d\n", sval);
		sem_wait(&scheduler_tick);
	}
}
