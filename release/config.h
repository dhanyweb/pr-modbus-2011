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

char serialPort[] = "/dev/ttyS1";

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

#define CONFIGFILE "main.cfg"

#endif /* CONFIG_H_ */
