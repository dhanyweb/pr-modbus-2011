// include - common
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
// include - mysql
#include <my_global.h>
#include <mysql.h>
// include - modbus
#include "modbus.h"
// include - configuration
#include "config.h"

// definitions

// global variables
struct timespec req; // sample interval structure
int ret; // return value for error-checking
config_t config; // configuration
config_t *config_p; // configuration
sem_t scheduler_tick; // semaphore
int err; // return value
// global variables- mysql
MYSQL *conn; // mysql connector
MYSQL_RES *res; // mysql result
MYSQL_ROW row; // mysql row
// global variables - modbus
char serialPort[] = "/dev/ttyS1";
uint8_t *tab_rp_status;
uint16_t *tab_rp_registers;
modbus_param_t mb_param;
// multi-threading variables
pthread_t *threads;
pthread_attr_t pthread_scheduler_attr;
pthread_attr_t pthread_dataacq_attr;

// thread functions
void* scheduler(void *arg);
void* dataacq(void *arg);

// main function
int main(int argc, char** argv) {

	printf("\n-== modbus data acquisition ==-\n\n");

	// reading config file
	if ((config_p = parser(CONFIGFILE)) == NULL) {
		printf("config file read error!");
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
		printf("%s\n", mysql_error(conn));
		exit(1);
	}

	// send SQL query
	if (mysql_query(conn, "SHOW TABLES")) {
		printf("%s\n", mysql_error(conn));
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
#ifdef MODE_RELEASE
	modbus_init_rtu(&mb_param, serialPort, 38400, "none", 8, 1);
	if (modbus_connect(&mb_param) == -1) {
		printf("error - connection failed\n");
		exit(1);
	} else
	printf("connected to modbus\t[OK]\n");
	*/
	//modbus_set_debug(&mb_param, TRUE);
#endif // MODE_RELEASE
	printf("connected to modbus\t[OK]\n");

	// allocate and initialize the memory to store the status and registers
#ifdef MODE_RELEASE
	tab_rp_status = (uint8_t *) malloc(MAX_STATUS * sizeof(uint8_t));
	memset(tab_rp_status, 0, MAX_STATUS * sizeof(uint8_t));

	tab_rp_registers = (uint16_t *) malloc(MAX_REGISTERS * sizeof(uint16_t));
	memset(tab_rp_registers, 0, MAX_REGISTERS * sizeof(uint16_t));
#endif // MODE_RELEASE
	printf("data acquisition\t[OK]\n");

	// creating threads
	threads = (pthread_t *) malloc(2 * sizeof(*threads));
	pthread_attr_init(&pthread_scheduler_attr);
	pthread_attr_init(&pthread_dataacq_attr);

	sem_init(&scheduler_tick, 0, 1);

	if ((err = pthread_create(&threads[0], &pthread_scheduler_attr, scheduler,
			(void*) &config.sampletime))) {
		printf("pthread_create (scheduler) %s\n", strerror(err));
		exit(1);
	}

	if ((err
			= pthread_create(&threads[1], &pthread_dataacq_attr, dataacq, NULL))) {
		printf("pthread_create (dataacq) %s\n", strerror(err));
		exit(1);
	}

	// starting threads
	if ((err = pthread_join(threads[0], NULL ))) {
		printf("pthread_join (scheduler) %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_join(threads[1], NULL ))) {
		printf("pthread_join (dataacq) %s\n", strerror(err));
		exit(1);
	}

	printf("endpoint reached\n");

	// free mem
	free(tab_rp_status);
	free(tab_rp_registers);

	// close connection
	modbus_close(&mb_param);

	exit(0);
}
