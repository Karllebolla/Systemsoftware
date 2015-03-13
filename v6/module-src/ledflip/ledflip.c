#include <pthread.h>
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
int freq = 2,fd, isPressed = 0;
pthread_t thread;

void overflow(struct timespec* t)
{
    if(t->tv_nsec >= 1000000000)
    {
        t->tv_nsec -= 1000000000;
        t->tv_sec++;
    }
}

void* thread_blink(void* ptr)
{
	int value;
	char flipBit[2];
    struct timespec to_wait = {
		.tv_sec = 0,
		.tv_nsec = 1000000000/freq
	};

	fd = open(DRIVER, O_RDWR);
    if(fd < 0)
    {
        printf(DRIVER " can not open.\n");
        
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
		
		to_wait.tv_nsec = 1000000000/freq;
    }
    return NULL;
}

void sigfunction(int sig)
{
	pthread_cancel(thread);
	write(fd, "1", 2);
	close(fd);
	exit(0);
}

int main()
{
    int number, tmp, btnfd, pressed = 0;
    char buf[2];
    int size = sizeof(buf);
    signal(SIGINT, sigfunction); /* abord with STRG-C */

	if(pthread_create(&thread, NULL, thread_blink, NULL) != 0 )
    {
		printf("pthread_create failed.\n");
		return 1;
    }

    while(1)
    {
		
		btnfd = open(DRIVER, O_RDWR);
		if(btnfd < 0)
		{
			printf(DRIVER " can not open.\n");
			return 1;
		}
		
        if((tmp = read(btnfd, buf, 1)) != 1){
			printf(DRIVER " can read only %d bytes.\n", tmp);
			pthread_cancel(thread);
			write(fd, "1", 2);
			close(btnfd);
			close(fd);
			return 1;	
		}
        close(btnfd);
        number = atoi(buf);

        if(buf[0] == '0' && !pressed)
        {	
			pressed = 1;
        } else if (buf[0] == '1' && pressed){
			if (freq >= 100) {
				freq = 2;
			}
            freq += 10;
			pressed = 0;
		}
    }

	write(fd, "1", 2);
	close(btnfd);
	close(fd);
    return 0;
}
