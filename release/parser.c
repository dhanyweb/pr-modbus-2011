/*
 * parser.c
 *
 *  Created on: 16.09.2011
 *      Author: prilag
 */

// include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "parser.h"

// definitions

// variables
FILE * pConfigFile;
char *parameter;
char *value;
char line[MAXLINELENGTH];
int i;
config_t config;

config_t *parser(char* configfile) {

	if ((pConfigFile = fopen(configfile, "r")) < 0) {
		return NULL;
	}

	while (fgets(line, 100, pConfigFile) != NULL) {
		parameter = strtok(line, "\t");
		value = strtok(NULL, "\t\n");

		if (strcmp(parameter, "meters") == 0) {
			config.meters = atoi(value);
		} else if (strcmp(parameter, "sampletime") == 0) {
			config.sampletime = atoi(value);
		} else if (strcmp(parameter, "uploadtime") == 0) {
			config.uploadtime = atoi(value);
		} else if (strcmp(parameter, "serverip") == 0) {
			strcpy(config.serverip, value);
		} else if (strcmp(parameter, "user") == 0) {
			strcpy(config.user, value);
		} else if (strcmp(parameter, "password") == 0) {
			strcpy(config.password, value);
		} else if (strcmp(parameter, "database") == 0) {
			strcpy(config.database, value);
		} else if (strcmp(parameter, "idvector") == 0) {
			config.idvector[0] = atoi(strtok(value, "[ ]"));
			for (i = 1; i < config.meters; i++) {
				config.idvector[i] = atoi(strtok(NULL, "[ ]"));
			}
		}
	}

	// printing variables
	printf(
			"number of meters\t%d\nsample interval\t\t%d\nupload interval\t\t%d\n",
			config.meters, config.sampletime, config.uploadtime);
	printf(
			"server address\t\t%s\nuser\t\t\t%s\npassword\t\t%s\ndatabase\t\t%s\n",
			config.serverip, config.user, config.password, config.database);
	printf("idvector\t\t[ ");
	for (i = 0; i < config.meters; i++) {
		printf("%d ", config.idvector[i]);
	}
	printf("]\n");

	// closing file
	if (fclose(pConfigFile) < 0) {
		perror("close");
		return NULL;
	}

	return &config;
}

