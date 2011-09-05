/*
 * config.h
 *
 *  Created on: 02.08.2011
 *      Author: prilag
 */

#ifndef CONFIG_H_
#define CONFIG_H_

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
/*  */

#endif /* CONFIG_H_ */
