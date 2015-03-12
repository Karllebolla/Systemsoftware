#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "test_openclose.h"

void test_openclose_with_minornumber_zero();
void test_openclose_with_minornumber_one();
void *test_open_minornumber_zero(void *arg);
void *test_open_minornumber_one(void *arg);
int delay_openclose;

void test_openclose_with_minornumber_zero(){
	pthread_t threadOne, threadTwo;	

	pthread_create(&threadOne, NULL, test_open_minornumber_zero, NULL);
	pthread_create(&threadTwo, NULL, test_open_minornumber_zero, NULL);
	
	pthread_join(threadOne, NULL);
	pthread_join(threadTwo, NULL);
}

void *test_open_minornumber_zero(void *arg) {

	int fd, retval=0;
	fd =open("/dev/OPENCLOSE0",O_RDONLY);
	if( fd < 0 ) {
		printf("Open is not possible, %s.\n",strerror(errno));
		retval = -1;
	} else {
		usleep(delay_openclose);
		close(fd);
	}
	pthread_exit(&retval);
}
	
void test_openclose_with_minornumber_one(){
	pthread_t threadOne, threadTwo;
	
	pthread_create(&threadOne, NULL, test_open_minornumber_one, NULL);
	pthread_create(&threadTwo, NULL, test_open_minornumber_one, NULL);
	
	pthread_join(threadOne, NULL);
	pthread_join(threadTwo, NULL);
}

void *test_open_minornumber_one(void *arg) {
	int fd, retval=0;
	fd = open("/dev/OPENCLOSE1",O_RDONLY);
	if( fd < 0) {
		printf("Open is not possible, %s.\n", strerror(errno));
		retval = -1;
	} else {
		usleep(delay_openclose);
		close(fd);
	}
		close(fd);
	pthread_exit(&retval);
}
