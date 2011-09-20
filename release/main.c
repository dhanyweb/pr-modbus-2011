// include section
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#include <my_global.h>
#include <mysql.h>

#include "modbus.h"
#include "config.h"

// definitions

// global variables
struct timespec req; // sample interval structure
int ret; // return value for error-checking
config_t config; // configuration
config_t *config_p; // configuration
// mysql
MYSQL *conn; // mysql connector
MYSQL_RES *res; // mysql result
MYSQL_ROW row; // mysql row
char query[255]; // mysql query
// modbus
uint8_t *tab_rp_status;
uint16_t *tab_rp_registers;
modbus_param_t mb_param;
measured_values_t measurement; // u, i, p, cosfi, energy

sem_t scheduler_tick;

void * scheduler(void *arg);
void * dataacq(void *arg);

// main function
int main(int argc, char** argv) {

	printf("\n-== modbus data acquisition ==-\n\n");

	// reading config file
	if ((config_p = parser(CONFIGFILE)) == NULL) {
		perror("config file read error!");
		exit(1);
	}
	// parse config, copy configuration to workspace
	memcpy(&config, config_p, sizeof(config_t));

	// setting timers
	req.tv_sec = config.sampletime;
	req.tv_nsec = 0;

	printf("MySQL client version: %s\n", mysql_get_client_info());

	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	// connect to database
	if (!mysql_real_connect(conn, config.serverip, config.user,
			config.password, config.database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	// send SQL query
	if (mysql_query(conn, "SHOW TABLES")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	// output table name
	printf("tables in mysql database:\n");
	printf("---------------------------\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);
	printf("---------------------------\n");

	// init RTU
	/*
	 modbus_init_rtu(&mb_param, serialPort, 38400, "none", 8, 1);
	 if (modbus_connect(&mb_param) == -1) {
	 printf("error - connection failed\n");
	 exit(1);
	 } else
	 printf("connected to modbus\t[OK]\n");
	 */
	//modbus_set_debug(&mb_param, TRUE);

	printf("connected to modbus\t[OK]\n");

	// allocate and initialize the memory to store the status and registers
	/*
	 tab_rp_status = (uint8_t *) malloc(MAX_STATUS * sizeof(uint8_t));
	 memset(tab_rp_status, 0, MAX_STATUS * sizeof(uint8_t));

	 tab_rp_registers = (uint16_t *) malloc(MAX_REGISTERS * sizeof(uint16_t));
	 memset(tab_rp_registers, 0, MAX_REGISTERS * sizeof(uint16_t));
	 */

	printf("data acquisition started\t[OK]\n");

	// creating threads


	pthread_t *threads;
	pthread_attr_t pthread_scheduler_attr;
	pthread_attr_t pthread_dataacq_attr;

	int err;

	threads = (pthread_t *) malloc(2 * sizeof(*threads));
	pthread_attr_init(&pthread_scheduler_attr);
	pthread_attr_init(&pthread_dataacq_attr);

	sem_init(&scheduler_tick, 0, 0);

	if (err = pthread_create(&threads[0], &pthread_scheduler_attr, scheduler,
			NULL)) {
		printf("pthread_create (scheduler) %s\n", strerror(err));
		exit(1);
	}

	if (err = pthread_create(&threads[1], &pthread_dataacq_attr, dataacq, NULL)) {
		printf("pthread_create (dataacq) %s\n", strerror(err));
		exit(1);
	}

	if (err = pthread_join(threads[1], NULL )) {
		printf("pthread_join: %s\n", strerror(err));
		exit(1);
	}

	if (err = pthread_join(threads[0], NULL )) {
		printf("pthread_join: %s\n", strerror(err));
		exit(1);
	}

	while (1) {
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

		nanosleep(&req, NULL);
	}

	// free mem
	//free(tab_rp_status);
	//free(tab_rp_registers);

	// close connection
	//modbus_close(&mb_param);

	return 0;
}

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
