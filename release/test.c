#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "modbus.h"
#include "config.h"

#include <mysql/mysql.h>
#include <mysql/my_global.h>

/*
 uint32_t gettime(void) {
 struct timeval tv;
 gettimeofday(&tv, NULL);

 return (uint32_t) tv.tv_sec * G_USEC_PER_SEC + tv.tv_usec;
 }
 */

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

int main(void) {

	int ret;
	int nb_points = 1;
	uint8_t *tab_rp_status;
	uint16_t *tab_rp_registers;
	modbus_param_t mb_param;

	measured_values_t measurement;



	MYSQL *conn;
		MYSQL_RES *res;
		MYSQL_ROW row;
		MYSQL_RES *result;
		int num_fields;
		int i;

		char *server = "192.168.1.173";
		char *user = "root";
		char *password = "root"; /* set me first */
		char *database = "first";

		conn = mysql_init(NULL);

		printf("MySQL client version: %s\n", mysql_get_client_info());

		conn = mysql_init(NULL);
		if (conn == NULL) {
			printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
			exit(1);
		}

		// connect to database
		if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
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
		printf("MySQL Tables in mysql database:\n");
		while ((row = mysql_fetch_row(res)) != NULL)
			printf("%s \n", row[0]);

	printf("\n\n-== MODBUS TEST program ==-\n\n");

	// init RTU
	modbus_init_rtu(&mb_param, serialPort, 38400, "none", 8, 1);
	if (modbus_connect(&mb_param) == -1) {
		printf("ERROR Connection failed\n");
		exit(1);
	} else
		printf("Connected to modbus\n");

	//modbus_set_debug(&mb_param, TRUE);

	// allocate and initialize the memory to store the status and registers
	tab_rp_status = (uint8_t *) malloc(MAX_STATUS * sizeof(uint8_t));
	memset(tab_rp_status, 0, MAX_STATUS * sizeof(uint8_t));

	tab_rp_registers = (uint16_t *) malloc(MAX_REGISTERS * sizeof(uint16_t));
	memset(tab_rp_registers, 0, MAX_REGISTERS * sizeof(uint16_t));

	printf("reading power meter ...\n");

	while (1) {
		/*
		 ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR, 200, nb_points,
		 tab_rp_registers);

		 printf("L1 %3.1f V\r\n", (float) (tab_rp_registers[0]) / 10);
		 */

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
		ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR, ADDR_COSPHISUM, 1,
				&measurement.COSPHISUM);

		// reading real energy (1)
		ret = read_holding_registers(&mb_param, MODBUS_DEV0_ADDR, ADDR_REALENERGY, 1,
				&measurement.REALENERGY);

printf	("L1 %3.1f V L2 %3.1f L3 V %3.1f V IL1 %d mA IL2 %d mA IL3 %d mA\r\n",
			(float) (measurement.UL1) / 10,
			(float) (measurement.UL2) / 10,
			(float) (measurement.UL3) / 10,
			measurement.IL1,
			measurement.IL2,
			measurement.IL3);

			sleep(1);
		}

		// free mem
		free(tab_rp_status);
		free(tab_rp_registers);

		// close connection
		modbus_close(&mb_param);

		return 0;
	}
