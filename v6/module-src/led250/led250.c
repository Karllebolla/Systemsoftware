#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define DRIVER "/dev/mygpio"
#define HZ 250

int fd;

void sigfunction(int sig)
{  
	write(fd, "1", 2);
	close(fd);
	exit(0);
}

int main()
{
	int value;
	char flipBit[2] = {"1"};
	
	struct timespec to_wait = {
		.tv_sec = 0,
		.tv_nsec = 1000000000/HZ
	};
	
	signal(SIGINT, sigfunction); /* abord with STRG-C */
	
	fd=open(DRIVER, O_WRONLY);
	if(fd < 0)
    {
        printf("open " DRIVER" is not possible, %s.\n", strerror(errno));
        exit(1);
    }
    while(1) 
    {
		clock_nanosleep(CLOCK_REALTIME, 0, &to_wait, NULL);
				
		if((value = write(fd,flipBit,1)) != 1) {
			printf("write " DRIVER " only %d Bytes written, %s.\n", value, strerror(errno));
			exit(1);
		}
		if(flipBit[0] == '1')
			flipBit[0] = '0';
		else
			flipBit[0] = '1';
					
	}
	return 0;
}
