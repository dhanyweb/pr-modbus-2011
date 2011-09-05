#include <stdio.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "gpio_dev.h"

int main(int argc, char **argv) {

	int gpio_pin;
	int fd;

	if ((fd = open("/dev/gpio", O_RDWR)) < 0) {
		printf("Error whilst opening /dev/gpio\n");
		return -1;
	}

	gpio_pin = 4;

	ioctl(fd, GPIO_DIR_OUT, gpio_pin);

	while (1) {
		ioctl(fd, GPIO_SET, gpio_pin);
		sleep(1);
		ioctl(fd, GPIO_CLEAR, gpio_pin);
		sleep(1);
	}

	return 0;
}

/*
 ioctl( fd, GPIO_DIR_IN, gpio_pin);
 ioctl(fd, GPIO_GET, gpio_pin);
 */
