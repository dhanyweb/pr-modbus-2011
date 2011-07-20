
/**
*   @file	test_2.c
*   @brief	simple C program that connects to my MySQL Database server
*   @author	prilag
*   @date July, 2011
*/

#include <mysql/mysql.h>
#include <mysql/my_global.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int table_exist(MYSQL* conn_, char* table_name);

int main(int argc, char **argv) {

	MYSQL* conn;

	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;

	int num_fields;
	int num_rows;
	int i;

	char *server = "localhost";
	char *user = "root";
	char *password = "root"; // set me first
	char *database = "modbus";

	// MySQL client information
	printf("\n-== MySQL client version: %s ==-\n\n", mysql_get_client_info());

	// obtaining MySQL connection handler (conn)
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	// connect to database
	if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)
			== 0) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	// send SQL query - list tables
	if (mysql_query(conn, "show tables")) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	// output table name
	printf("MySQL Tables in mysql database:\n");

	while ((row = mysql_fetch_row(res)) != NULL)
		printf("\t-> %s \n", row[0]);

	// check if table device exists or not - if not, create it
	num_rows = table_exist(conn, "'device'");

	if (num_rows == 1)
		printf("table %s exists!\n", "device");
	else {
		printf("table %s doesn't exists!\n", "device");
		printf("creating table %s\n", "device");
		mysql_query(conn,
				"CREATE TABLE device (meter_id INT, descr VARCHAR(30) CHARACTER SET utf8)");
		mysql_query(conn, "INSERT INTO device values (0, 'SIEMENS S200')");
		mysql_query(conn, "INSERT INTO device values (2, 'SIEMENS S400')");
		mysql_query(conn, "INSERT INTO device values (2, 'SIEMENS S400')");
		mysql_query(conn, "INSERT INTO device values (3, 'SIEMENS S50')");
		mysql_query(conn, "INSERT INTO device values (4, 'NONAME')");
	}

	// check if table measure exists or not - if not, create it
	num_rows = table_exist(conn, "'measure'");

	if (num_rows == 1)
		printf("table %s exists!\n", "measure");
	else {
		printf("table %s doesn't exists!\n", "measure");
		mysql_query(
				conn,
				"CREATE TABLE measure (meter_id INT, time DATETIME,	curr_time TIME, voltage FLOAT, current FLOAT, cosfi FLOAT)");
	}

	if (mysql_query(conn, "SELECT * FROM device")) {
		printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	res = mysql_store_result(conn);

	num_fields = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res))) {
		for (i = 0; i < num_fields; i++) {
			if (i == 0) {
				while ((field = mysql_fetch_field(res)) != 0) {
					printf("%s\t\t", field->name);
				}
				printf("\n");
			}
			printf("%s\t", row[i] ? row[i] : "NULL");
		}
	}
	printf("\n");

	// fill database with random values

	int meter_id;
	float voltage;
	float current;
	float cosfi;
	char query[255];

	// initalize random seed
	srand(time(NULL));

	for (i = 0; i < 10; i++) {

		meter_id = rand() % 4 + 1;
		voltage = (float)(rand() % 21 + 210);
		current = (float)(rand() % 10) / 10 + 4;
		cosfi = (float)rand() / RAND_MAX;

		sprintf(&query[0],
				"INSERT INTO measure values (%f, NOW(), NOW(), %f, %f, %f);",
				(float) meter_id, (float) voltage, (float) current,
				(float) cosfi);

		printf("%s\n", &query[0]);

		if (mysql_query(conn, &query[0])) {
			printf("Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
	}

	// close connection
	mysql_free_result(res);
	mysql_close(conn);

	return 0;
}

/**
*   this function checks whether a table exists in the database or not
*   @param	conn
*   @param	table_name
*   @date	July, 2011
*   @return 1 if exists 0 if not
*/

int table_exist(MYSQL* conn_, char* table_name) {
	MYSQL_RES *res;
	int num_rows;
	char query[255];

	// check if table <table_name> exists or not
	sprintf(&query[0], "SHOW TABLES LIKE %s", table_name);
	//strcat(&query[0], table_name);

	if (mysql_query(conn_, query)) {
		printf("Error %u: %s\n", mysql_errno(conn_), mysql_error(conn_));
		exit(1);
	}

	res = mysql_store_result(conn_);
	num_rows = mysql_num_rows(res);

	return num_rows;
}
