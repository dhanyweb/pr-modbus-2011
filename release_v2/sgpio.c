/*
 * sgpio.c
 *
 *  Created on: 10.08.2011
 *      Author: prilag
 */

#include <sys/ioctl.h>
#include "gpio_dev.h"
#include "sgpio.h"
#include <fcntl.h>
#include <stdio.h>

int fd;
#define GPIO_RW 4
#define GPIO_SHOW 2

int sgpio_init(void) {
	if ((fd = open("/dev/gpio", O_RDWR | O_FSYNC)) < 0) {
		printf("Error whilst opening /dev/gpio\n");
		return -1;
	}

	ioctl(fd, GPIO_DIR_OUT, GPIO_RW);
	ioctl(fd, GPIO_DIR_OUT, GPIO_SHOW);
	//printf("dirout\n");

	return 0;
}

int sgpio_set2read(void) {

	ioctl(fd, GPIO_CLEAR, GPIO_RW);
	ioctl(fd, GPIO_SET, GPIO_SHOW);
	//printf("read\n");

	return 0;
}

int sgpio_set2write(void) {

	ioctl(fd, GPIO_SET, GPIO_RW);
	ioctl(fd, GPIO_CLEAR, GPIO_SHOW);
	//printf("write\n");
	return 0;
}
