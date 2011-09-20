/*
 * parser.h
 *
 *  Created on: 16.09.2011
 *      Author: prilag
 */

#ifndef PARSER_H_
#define PARSER_H_

#define MAXLINELENGTH 255

typedef struct config {
	int meters;
	int idvector[128];
	int sampletime;
	int uploadtime;
	char serverip[20];
	char user[50];
	char password[50];
	char database[50];
} config_t;

config_t *parser(char* configfile);

#endif /* PARSER_H_ */
