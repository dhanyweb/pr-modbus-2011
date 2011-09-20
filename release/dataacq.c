/*
 * dataacq.c
 *
 *  Created on: 20.09.2011
 *      Author: prilag
 */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#include <my_global.h>
#include <mysql.h>

#include "modbus.h"
#include "config.h"

extern sem_t scheduler_tick;
// mysql
extern MYSQL *conn; // mysql connector
extern MYSQL_RES *res; // mysql result
extern MYSQL_ROW row; // mysql row
char query[255]; // mysql query
// modbus
uint8_t *tab_rp_status;
uint16_t *tab_rp_registers;
modbus_param_t mb_param;
measured_values_t measurement; // u, i, p, cosfi, energy

void *
dataacq(void *arg) {
	int sval = 100;

	while (1) {
		//sem_getvalue(&scheduler_tick, &sval);
		//printf("dataacq | s: %d\n", sval);

		// start data acquisition
		sem_wait(&scheduler_tick);

		/*
		 // reading voltages (3)
		 ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR, ADDR_UL1, 3,
		 &measurement.UL1);
		 // reading currents (3)
		 ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR, ADDR_IL1, 3,
		 &measurement.IL1);

		 // reading powers (3)
		 ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR, ADDR_PSUM, 3,
		 &measurement.PSUM);

		 // reading cosphisum (1)
		 ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR,
		 ADDR_COSPHISUM, 1, &measurement.COSPHISUM);

		 // reading real energy (1)
		 ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR,
		 ADDR_REALENERGY, 1, &measurement.REALENERGY);

		 printf(
		 "L1 %3.1f V L2 %3.1f L3 V %3.1f V IL1 %d mA IL2 %d mA IL3 %d mA\r\n",
		 (float) (measurement.UL1) / 10, (float) (measurement.UL2) / 10,
		 (float) (measurement.UL3) / 10, measurement.IL1,
		 measurement.IL2, measurement.IL3);
		 */
		sprintf(
				&query[0],
				"INSERT INTO measure_new values (0, NOW(), %.1f, %.1f, %.1f, %d, %d, %d, %d, %d, %d, %d, %d);",
				(float) (measurement.UL1) / 10, (float) (measurement.UL2) / 10,
				(float) (measurement.UL3) / 10, measurement.IL1,
				measurement.IL2, measurement.IL3, measurement.PSUM,
				measurement.QSUM, measurement.SSUM, measurement.COSPHISUM,
				measurement.REALENERGY);

		printf("%s\n", &query[0]);

		if (mysql_query(conn, &query[0])) {
			printf("error %u: %s\n", mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
	}

}
