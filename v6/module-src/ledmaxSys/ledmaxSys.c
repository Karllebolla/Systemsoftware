#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "differenz_zeitmessung.h"

#define STEPS 50000

int fd = 0;

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

	fd = open("/sys/class/gpio/export", O_WRONLY);
    if(fd < 0 )
    {
        printf("Export failed\n");
        return 1;
    }

    if(write(fd, "18\n", 3) != 3)
    {
        printf("Write gpio 18 failed export.\n");
        return 1;
    }
    close(fd);
      
    fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
    if (fd < 0)
    {
        printf("Can not open direction file.\n");
        exit(1);
    }
    

      
    
    clock_gettime(CLOCK_REALTIME, &start);
	
	for(x = 0; x < STEPS; x++) {
		
	if(write(fd, "out\n", 4) != 4)
    {
        printf("Can not write in direction file.\n");
        exit(1);
    }
	if(write(fd, "in\n", 3) != 3)
    {
        printf("Can not write in direction file.\n");
        exit(1);
    }
					
	}
	clock_gettime(CLOCK_REALTIME, &end);
	
	diffTime = diff_time(&start, &end);
	diffTime = STEPS / diffTime;

	printf("max. frequenz: %f seconds.\n", diffTime);
	write(fd, "1", 2);
	close(fd);
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if(fd < 0)
    {
        printf("Unexoprt failed.\n");
        return 1;
    }

    if(write(fd, "18\n", 3) != 3)
    {
        printf("Write gpio 18 failed unexport.\n");
        return 1;
    }

    close(fd);
    return 0;
}
