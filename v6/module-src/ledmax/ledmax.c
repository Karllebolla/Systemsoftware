#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "differenz_zeitmessung.h"

#define DRIVER "/dev/mygpio"
#define STEPS 50000

int fd;

void sigfunction(int sig)
{
	write(fd, "1", 2);
	close(fd);
	exit(0);
}

int main()
{
	int value, x;
	double diffTime;
	
	struct timespec start, end;
	
	signal(SIGINT, sigfunction); /* abord with STRG-C */

	fd=open(DRIVER, O_WRONLY);
	if(fd < 0)
    {
        printf("open " DRIVER" is not possible, %s.\n", strerror(errno));
        exit(1);
    }

	clock_gettime(CLOCK_REALTIME, &start);
	
	for(x = 0; x < STEPS; x++) {
		
		if((value = write(fd,"0",2)) != 2) {
			printf("write " DRIVER " only %d Bytes written, %s.\n", value, strerror(errno));
			exit(1);
		}
		if((value = write(fd,"1",2)) != 2) {
			printf("write " DRIVER " only %d Bytes written, %s.\n", value, strerror(errno));
			exit(1);
		}
					
	}
	clock_gettime(CLOCK_REALTIME, &end);
	
	diffTime = diff_time(&start, &end);
	diffTime = STEPS / diffTime;
	
	printf("max. frequenz: %f seconds.\n", diffTime);
	write(fd, "1", 2);
	close(fd);
	return 0;
}
