#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "modbus.h"
#include "config.h"

#define SLAVE     0x01

#ifdef PR_MOD_X86
char serialPort[] = "/dev/ttyUSB0";
#endif

#ifdef PR_MOD_MIPS
char serialPort[] = "/dev/ttys0";
#endif
/*
 uint32_t gettime(void) {
 struct timeval tv;
 gettimeofday(&tv, NULL);

 return (uint32_t) tv.tv_sec * G_USEC_PER_SEC + tv.tv_usec;
 }
 */

int main(void) {

	int ret;
	int nb_points = 1;
	uint8_t *tab_rp_status;
	uint16_t *tab_rp_registers;
	modbus_param_t mb_param;

	printf("\n\n-== MODBUS TEST program ==-\n\n");

	// init RTU
	modbus_init_rtu(&mb_param, "/dev/ttyUSB0", 38400, "none", 8, 1);
	if (modbus_connect(&mb_param) == -1) {
		printf("ERROR Connection failed\n");
		exit(1);
	} else
		printf("Connected to modbus\n");

	modbus_set_debug(&mb_param, TRUE);

	// allocate and initialize the memory to store the status and registers
	tab_rp_status = (uint8_t *) malloc(MAX_STATUS * sizeof(uint8_t));
	memset(tab_rp_status, 0, MAX_STATUS * sizeof(uint8_t));

	tab_rp_registers = (uint16_t *) malloc(MAX_REGISTERS * sizeof(uint16_t));
	memset(tab_rp_registers, 0, MAX_REGISTERS * sizeof(uint16_t));

	printf("reading power meter ...");

	while (1) {
		ret = read_holding_registers(&mb_param, SLAVE, 200, nb_points,
				tab_rp_registers);

		printf("voltage: %3.1f V\r\n", (float) (tab_rp_registers[0]) / 10);

		sleep(1);
	}

	// free mem
	free(tab_rp_status);
	free(tab_rp_registers);

	// close connection
	modbus_close(&mb_param);

	return 0;
}
