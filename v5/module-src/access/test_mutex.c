#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "test_mutex.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DRIVERNAME "/dev/OPEN_ONCE"

void *test_openoneM(void *arg);
void *test_opentwoM(void *arg);

int fd;

void test_mutex() {

	pthread_t threadOpenOne, threadOpenTwo;

	
	printf("open " DRIVERNAME " successful.\n");
	pthread_create(&threadOpenOne, NULL, test_openoneM, NULL);
	pthread_create(&threadOpenTwo, NULL, test_opentwoM, NULL);
	
	pthread_join(threadOpenOne, NULL);
	pthread_join(threadOpenTwo, NULL);
	
}

void *test_openoneM(void *arg) {

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


void *test_opentwoM(void *arg) {

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

