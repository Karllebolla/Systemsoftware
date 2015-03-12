#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void *test_read_null_hello_world(void *arg);
void test_read_null();

void test_read_null(){
	pthread_t threadOne, threadTwo;	

	pthread_create(&threadOne, NULL, test_read_null_hello_world, NULL);
	pthread_create(&threadTwo, NULL, test_read_null_hello_world, NULL);
	
	pthread_join(threadOne, NULL);
	pthread_join(threadTwo, NULL);
}

void *test_read_null_hello_world(void *arg) {
	
	int id = pthread_self(), retval=0;
	int fd, copied;
	char readed[20];

	fd = open("/dev/mynull",O_RDONLY);
	if(fd < 0) {
		printf("open /dev/mynull is not possible, %s.\n", strerror(errno));
		retval=-1;
		pthread_exit(&retval);
	}

	copied = read(fd, readed, 2);
	//printf("%d: %d zeichen gelesen\n",id, copied);
	if (copied != 2) {
		printf("Can't read 2 char's\n");
		close(fd);
		retval=-1;
		pthread_exit(&retval);
	}
	printf("%d: readed char's: %s\n", id, readed);
	memset(readed, 0, sizeof(readed));

	copied = read(fd, readed, 3);
	//printf("%d zeichen gelesen\n",copied);
	if (copied != 3) {
		printf("Can't read 2 char's\n");
		close(fd);
		retval=-1;
		pthread_exit(&retval);
	}
	printf("%d: readed char's: %s\n", id, readed);
	memset(readed, 0, sizeof(readed));

	copied = read(fd, readed, 6);
	//printf("%d zeichen gelesen\n",copied);
	if (copied != 6) {
		printf("Can't read 2 char's\n");
		close(fd);
		retval=-1;
		pthread_exit(&retval);
	}
	printf("%d: readed char's: %s\n", id, readed);
	memset(readed, 0, sizeof(readed));

	copied = read(fd, readed, 6);
	//printf("%d zeichen gelesen\n",copied);
	printf("%d: readed char's: %s\n", id, readed);
	memset(readed, 0, sizeof(readed));

	copied = read(fd, readed, 6);
	//printf("%d zeichen gelesen\n",copied);
	printf("%d: readed char's: %s\n", id, readed);
	memset(readed, 0, sizeof(readed));

	close(fd);
	pthread_exit(&retval);
}
