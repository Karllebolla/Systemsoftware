#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "test_buffer_ts.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DRIVERNAME "/dev/FIFO_BUFFER"

void test_buffer();
void *test_read_buffer(void *arg);
void *test_write_buffer(void *arg);

int fd;

void test_buffer() {

	pthread_t threadRead, threadWrite;

	fd = open(DRIVERNAME, O_RDWR);
	if (fd < 0) {
		printf("open " DRIVERNAME " is not possible, %s.\n", strerror(errno));
		return;
	}
	printf("open " DRIVERNAME " successful.\n");
	pthread_create(&threadRead, NULL, test_read_buffer, NULL);
	pthread_create(&threadWrite, NULL, test_write_buffer, NULL);
	
	pthread_join(threadRead, NULL);
	pthread_join(threadWrite, NULL);
	if (close(fd) < 0) {
		printf("close " DRIVERNAME " is not possible, %s.\n", strerror(errno));
		return;
	}
}

void *test_write_buffer(void *arg) {

	int id = pthread_self(), copied,retval = 0;
	char *buf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";	
	printf("write thread\n");
	
	copied = write(fd, buf,5);
	printf("write %d chars\n", copied);

	printf("%d sleeps 8 sec.\n",id);
	sleep(8);
	printf("%d wake up.\n",id);

	copied = write(fd, buf, 8);
	printf("%d write %d chars.\n",id,copied);
	
	copied = write(fd, buf, 10);
	printf("%d write %d chars.\n",id,copied);
	pthread_exit(&retval);
}


void *test_read_buffer(void *arg) {

	char buf[20];
	int readed,retval = 0, id = pthread_self();
	
	sleep(5);
	printf("read thread\n");
	readed = read(fd,buf,3);
	if(readed != 3) {
		printf("Can't read 4 chars's.\n");
		retval = -1;
		pthread_exit(&retval);
	}
	printf("%d readed %s chars %d\n",id, buf,readed);

	printf("%d sleep 15 sec\n",id);
	sleep(20);
	printf("%d wake up\n",id);

	readed = read(fd,buf,10);
	printf("%d readed %s chars %d\n",id, buf, readed);
	pthread_exit(&retval);
}


