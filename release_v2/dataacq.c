/*
 * dataacq.c
 *
 *  Created on: 20.09.2011
 *      Author: prilag
 */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#include <my_global.h>
#include <mysql.h>

#include "modbus.h"
#include "config.h"

extern config_t config; // configuration
int ret;
extern sem_t scheduler_tick;
// mysql
extern MYSQL *conn; // mysql connector
extern MYSQL_RES *res; // mysql result
extern MYSQL_ROW row; // mysql row
char query[255]; // mysql query
// modbus
extern uint8_t *tab_rp_status;
extern uint16_t *tab_rp_registers;
extern modbus_param_t mb_param;
measured_values_t meas[MAX_METER_NUMBER]; // u, i, p, cosfi, energy
average_values_t avg; // average
int i_avg; // cycle variable - samples/cycle
int i_dev; // cycle variable - device number

void average_null(int meters);
void average_calculate(int divi);

int test_vector[] = { 2210, 2220, 2230, 2240, 2250, 2260, 2270, 2280, 2290,
		2300 };

void*
dataacq(void *arg) {

	avg.null = average_null;
	avg.calculate = average_calculate;
	int dev;

	// start data acquisition
	while (1) {
		avg.null(config.samplespercycle);
		// start average
		for (i_avg = 1; i_avg < config.samplespercycle + 1; i_avg++) {

			for (i_dev = 0; i_dev < config.meters; i_dev++) {

				dev = config.idvector[i_dev];
				//printf("device %d\n", dev);
#ifdef MODE_RELEASE
				// reading voltages (3)
				ret = read_holding_registers(&mb_param, dev,
						ADDR_UL1, 3, &meas[dev].UL1);
				// reading currents (3)
				ret = read_holding_registers(&mb_param, dev,
						ADDR_IL1, 3, &meas[dev].IL1);
				// reading powers (3)
				ret = read_holding_registers(&mb_param, dev,
						ADDR_PSUM, 3, &meas[dev].PSUM);
				// reading cosphisum (1)
				ret = read_holding_registers(&mb_param, dev,
						ADDR_COSPHISUM, 1, &meas[dev].COSPHISUM);
#endif // MODE_RELEASE
#ifdef MODE_TEST
				meas[dev].UL1 = test_vector[i_avg - 1];
				meas[dev].UL2 = test_vector[i_avg - 1];
				meas[dev].UL3 = test_vector[i_avg - 1];

				meas[dev].IL1 = test_vector[i_avg - 1];
				meas[dev].IL2 = test_vector[i_avg - 1];
				meas[dev].IL3 = test_vector[i_avg - 1];

				meas[dev].PSUM = test_vector[i_avg - 1];
				meas[dev].QSUM = test_vector[i_avg - 1];
				meas[dev].SSUM = test_vector[i_avg - 1];

				meas[dev].COSPHISUM = test_vector[i_avg - 1];

				printf(
						"L1 %3.1f V L2 %3.1f L3 V %3.1f V IL1 %d mA IL2 %d mA IL3 %d mA\r\n",
						(float) (meas[dev].UL1) / 10, (float) (meas[dev].UL2)
								/ 10, (float) (meas[dev].UL3) / 10,
						meas[dev].IL1, meas[dev].IL2, meas[dev].IL3);
#endif// MODE_TEST
				// average
				avg.calculate(i_avg);
			} // devices
			sem_wait(&scheduler_tick);
		} // avg

		//printf("upload!\n");

		// upload data
		for (i_dev = 0; i_dev < config.meters; i_dev++) {
			// reading real energy (1)
			dev = config.idvector[i_dev];
#ifdef MODE_RELEASE
			 ret = read_holding_registers(&mb_param, dev, ADDR_REALENERGY, 1,
			 &meas[dev].REALENERGY);
#endif // MODE_RELEASE

			sprintf(
					&query[0],
					"INSERT INTO %s values (0, NOW(), %.1f, %.1f, %.1f, %d, %d, %d, %d, %d, %d, %d, %d);",
					config.table, (avg.UL1[dev]) / 10, (avg.UL2[dev]) / 10,
					(avg.UL3[dev]) / 10, (int) avg.IL1[dev],
					(int) avg.IL2[dev], (int) avg.IL3[dev],
					(int) avg.PSUM[dev], (int) avg.QSUM[dev],
					(int) avg.SSUM[dev], (int) avg.COSPHISUM[dev],
					meas[dev].REALENERGY);

			printf("%s\n", &query[0]);

			if (mysql_query(conn, &query[0])) {
				printf("error %u: %s\n", mysql_errno(conn), mysql_error(conn));
				exit(1);
			}
		}
	} // while 1

	pthread_exit(0);
}

void average_null(int meters) {
	int i;
	int dev;

	for (i = 0; i < config.meters; i++) {

		dev = config.idvector[i];

		avg.UL1[dev] = 0.0;
		avg.UL2[dev] = 0.0;
		avg.UL3[dev] = 0.0;

		avg.IL1[dev] = 0.0;
		avg.IL2[dev] = 0.0;
		avg.IL3[dev] = 0.0;

		avg.PSUM[dev] = 0.0;
		avg.QSUM[dev] = 0.0;
		avg.SSUM[dev] = 0.0;

		avg.COSPHISUM[dev] = 0;
	}

	//printf("avg null\n");

	return;
}

void average_calculate(int divi) {

	int i;
	int dev;

	for (i = 0; i < config.meters; i++) {

		dev = config.idvector[i];

		avg.UL1[dev] = ((divi - 1) * avg.UL1[dev] + meas[dev].UL1) / divi;
		avg.UL2[dev] = ((divi - 1) * avg.UL2[dev] + meas[dev].UL2) / divi;
		avg.UL3[dev] = ((divi - 1) * avg.UL3[dev] + meas[dev].UL3) / divi;

		avg.IL1[dev] = ((divi - 1) * avg.IL1[dev] + meas[dev].IL1) / divi;
		avg.IL2[dev] = ((divi - 1) * avg.IL2[dev] + meas[dev].IL2) / divi;
		avg.IL3[dev] = ((divi - 1) * avg.IL3[dev] + meas[dev].IL3) / divi;

		avg.PSUM[dev] = ((divi - 1) * avg.PSUM[dev] + meas[dev].PSUM) / divi;
		avg.QSUM[dev] = ((divi - 1) * avg.QSUM[dev] + meas[dev].QSUM) / divi;
		avg.SSUM[dev] = ((divi - 1) * avg.SSUM[dev] + meas[dev].SSUM) / divi;

		avg.COSPHISUM[dev] = ((divi - 1) * avg.COSPHISUM[dev]
				+ meas[dev].COSPHISUM) / divi;
	}

	return;
}
