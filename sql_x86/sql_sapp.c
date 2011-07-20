/* Simple C program that connects to MySQL Database server*/
#include <mysql/mysql.h>
#include <mysql/my_global.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_RES *result;
	int num_fields;
	int i;

	char *server = "localhost";
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
	if (mysql_query(conn, "show tables")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	// output table name
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);

	mysql_query(conn, "SELECT * FROM authors");
	result = mysql_store_result(conn);

	num_fields = mysql_num_fields(result);

	while ((row = mysql_fetch_row(result))) {
		for (i = 0; i < num_fields; i++) {
			printf("%s ", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}

	// close connection
	mysql_free_result(res);
	mysql_close(conn);

	return 0;
}

