#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void test_write_null() {
	int fd, count_written,count=5;
	char *string = "Hello World";
	fd = open("/dev/NULL",O_WRONLY);
	if(fd < 0) {
		printf("open /dev/NULL is not possible, s%s.\n", strerror(errno));
		return;
	}
	count_written = write(fd,string,count);
	if(count_written != count) {
		printf("Error wrte /dev/NULL only %s.\n", strerror(errno));
		return;
	}
	printf("Should write %d and written %d.\n",count, count_written);
		close(fd);
}
