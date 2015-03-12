#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void test_zero_with_minornumber_one_read_hello();
void test_zero_with_minornumber_zero_read_zero();
void *test_read_hello(void *arg);
void *test_read_zero();

void test_zero_with_minornumber_one_read_hello() {
	pthread_t threadOne;
	
	pthread_create(&threadOne, NULL,test_read_hello,NULL);
	
	pthread_join(threadOne, NULL);
}

void *test_read_hello(void *arg) {
	int fd,count,size_buffer=5, retval=0;
	char *buffer = malloc(sizeof(char)*size_buffer);
	
	fd = open("/dev/ZERO1",O_RDONLY);
	if(fd < 0) {
		printf("Open /dev/ZERO1 is not possible, %s.\n", strerror(errno));
		retval = -1;
		pthread_exit(&retval);
	} 
	count = read(fd,buffer,size_buffer);
	printf("count %d buffer %d\n",count,size_buffer);
	
	if(count != size_buffer) {
		printf("Error read /dev/ZERO1 only %s.\n",strerror(errno));
		close(fd);
		retval = -1;
		pthread_exit(&retval);
	}
	close(fd);
	pthread_exit(&retval);
}

void test_zero_with_minornumber_zero_read_zero() {
	pthread_t threadOne;
	pthread_create(&threadOne, NULL, test_read_zero, NULL);
	pthread_join(threadOne, NULL);
}
	
void *test_read_zero(){
	int fd,count,size_buffer = 2, retval=0;
	char *buffer = malloc(sizeof(char) * size_buffer);

	fd = open("/dev/ZERO0", O_RDONLY);
	if (fd < 0) {
		printf("open /dev/ZERO0 is not possible, %s.\n", strerror(errno));
		retval = -1;
		pthread_exit(&retval);
	}
	
	count = read(fd, buffer,size_buffer);
	printf("count %d buffer %d\n",count,size_buffer);
	if(count != size_buffer) {
		printf("Error read /dev/ZERO0 only %s.\n",strerror(errno));
		close(fd);
		retval = -1;
		pthread_exit(&retval);
	}
	close(fd);
	pthread_exit(&retval);
}
