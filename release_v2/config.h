/*
 * config.h
 *
 *  Created on: 02.08.2011
 *      Author: prilag
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include "parser.h"

#define MODE_TEST
//#define MODE_RELEASE

#define MODBUS_DEV0_ADDR     0x01

/* ADDRESS MAP */
#define ADDR_UL1	200
#define ADDR_UL2	201
#define ADDR_UL3	202
#define ADDR_IL1	206
#define ADDR_IL2	207
#define ADDR_IL3	208
#define ADDR_PSUM	279
#define ADDR_QSUM	280
#define ADDR_SSUM	281
#define ADDR_COSPHISUM	276
#define ADDR_REALENERGY	416
/* */

#define MAX_METER_NUMBER 127

typedef struct measured_values {
	uint16_t UL1;
	uint16_t UL2;
	uint16_t UL3;

	uint16_t IL1;
	uint16_t IL2;
	uint16_t IL3;

	uint16_t PSUM;
	uint16_t QSUM;
	uint16_t SSUM;

	uint16_t COSPHISUM;

	uint16_t REALENERGY;
} measured_values_t;

typedef struct average_values {
	double UL1[MAX_METER_NUMBER];
	double UL2[MAX_METER_NUMBER];
	double UL3[MAX_METER_NUMBER];

	double IL1[MAX_METER_NUMBER];
	double IL2[MAX_METER_NUMBER];
	double IL3[MAX_METER_NUMBER];

	double PSUM[MAX_METER_NUMBER];
	double QSUM[MAX_METER_NUMBER];
	double SSUM[MAX_METER_NUMBER];

	double COSPHISUM[MAX_METER_NUMBER];

	void(*null)(int meters);
	void(*calculate)(int divi);

//	uint16_t REALENERGY;
} average_values_t;

#define CONFIGFILE "main.cfg"

#endif /* CONFIG_H_ */

/*
 mysql> DESCRIBE measure_new;
 +------------+-------------+------+-----+---------+-------+
 | Field      | Type        | Null | Key | Default | Extra |
 +------------+-------------+------+-----+---------+-------+
 | meter_id   | smallint(6) | YES  |     | NULL    |       |
 | time       | datetime    | YES  |     | NULL    |       |
 | ul1        | float       | YES  |     | NULL    |       |
 | ul2        | float       | YES  |     | NULL    |       |
 | ul3        | float       | YES  |     | NULL    |       |
 | il1        | smallint(6) | YES  |     | NULL    |       |
 | il2        | smallint(6) | YES  |     | NULL    |       |
 | il3        | smallint(6) | YES  |     | NULL    |       |
 | psum       | smallint(6) | YES  |     | NULL    |       |
 | qsum       | smallint(6) | YES  |     | NULL    |       |
 | ssum       | smallint(6) | YES  |     | NULL    |       |
 | cosphisum  | smallint(6) | YES  |     | NULL    |       |
 | realenergy | int(11)     | YES  |     | NULL    |       |
 +------------+-------------+------+-----+---------+-------+
 13 rows in set (0.00 sec)
 */

