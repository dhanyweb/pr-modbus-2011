/*
 * sch_data_acq.c
 *
 *  Created on: 20.09.2011
 *      Author: prilag
 */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

extern sem_t scheduler_tick;

void *
scheduler(void *arg) {
	int sval = 100;

	while (1) {
		sem_getvalue(&scheduler_tick, &sval);
		printf("scheduler | s: %d\n", sval);
		sleep(1);
		sem_post(&scheduler_tick);
	}
}
