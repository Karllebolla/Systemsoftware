#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "test_sema.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DRIVERNAME "/dev/LOCK"

void *test_openone(void *arg);
void *test_opentwo(void *arg);

int fd;

void test_sema() {

	pthread_t threadOpenOne, threadOpenTwo;

	
	printf("open " DRIVERNAME " successful.\n");
	pthread_create(&threadOpenOne, NULL, test_openone, NULL);
	pthread_create(&threadOpenTwo, NULL, test_opentwo, NULL);
	
	pthread_join(threadOpenOne, NULL);
	pthread_join(threadOpenTwo, NULL);
	
}

void *test_openone(void *arg) {

	int fd,retval = 0;
	fd = open(DRIVERNAME, O_RDONLY);
	if (fd < 0) {
		printf("open " DRIVERNAME " is not possible, %s.\n", strerror(errno));
		retval = -1;
		pthread_exit(&retval);
	}
	
	if (close(fd) < 0) {
		printf("close " DRIVERNAME " is not possible, %s.\n", strerror(errno));
		retval = -1;
		pthread_exit(&retval);
	}
	
	pthread_exit(&retval);
}


void *test_opentwo(void *arg) {

	int fd, retval = 0;
	fd = open(DRIVERNAME, O_RDONLY);
	if (fd < 0) {
		printf("open " DRIVERNAME " is not possible, %s.\n", strerror(errno));
		retval = -1;
		pthread_exit(&retval);
	}
	
	if (close(fd) < 0) {
		printf("close " DRIVERNAME " is not possible, %s.\n", strerror(errno));
		retval = -1;
		pthread_exit(&retval);
	}
	
	pthread_exit(&retval);
}

