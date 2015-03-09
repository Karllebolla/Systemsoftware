#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

int isPressed = 0;

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
    int fd;
    int periode = 200000000;
    struct timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);
    t.tv_nsec += periode;
    overflow(&t);

    while(1)
    {
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
        if( fd < 0 )
        {
            printf("Can not open direction file.\n");
            exit(1);
        }
        if(write(fd, "out\n", 4) != 4)
        {
            printf("Can not write in direction file\n");
            exit(1);
        }
        close(fd);
        t.tv_nsec += periode;

        overflow(&t);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
        if (fd < 0)
        {
            printf("Can not open direction file.s\n");
            exit(1);
        }
        if(write(fd, "in\n", 3) != 3)
        {
            printf("Can not write in direction file.\n");
            exit(1);
        }
        close(fd);
        t.tv_nsec += periode;
        overflow(&t);
    }
    return NULL;
}

void sigfunction(int sig)
{

    int fd;
    if(sig != SIGINT)
    {
        return;
    }
    else
    {
        if(isPressed != 0)
        {
            fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
            if (fd < 0)
            {
                printf("Can not open direction file.\n");
                exit(1);
            }
            if(write(fd, "in\n", 3) != 3)
            {
                printf("Can not write in direction file.\n");
                exit(1);
            }
            close(fd);

            fd = open("/sys/class/gpio/unexport", O_WRONLY);
            if(fd < 0)
            {
                printf("Can not open unexport file.\n");
                exit(1);
            }
            if(write(fd, "18\n", 3) != 3)
            {
                printf("Can not write in export file.s\n");
                exit(1);
            }
            close(fd);
        }

        //close gpio17
        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if(fd < 0)
        {
            printf("Can not open unexport file.\n");
            exit(1);
        }
        if(write(fd, "17\n", 3) != 3)
        {
            printf("Can not write in export file.\n");
            exit(1);
        }
        close(fd);
        exit(1);
    }
}



int main()
{

    int fd, number;
    char buf[2];
    int size = sizeof(buf);
    pthread_t thread;
    signal(SIGINT, sigfunction); //abord with STRG-C


    fd = open("/sys/class/gpio/export", O_WRONLY);
    if(fd < 0)
    {
        printf("Can not open export file 17.\n");
        exit(1);
    }
    if(write(fd, "17\n", 3) != 3)
    {
        printf("Can not write in export file 17.\n");
        exit(1);
    }
    close(fd);

    while(1)
    {
        fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);
        if(fd < 0)
        {
            printf("Can not open value file 17.\n");
            exit(1);
        }
        read(fd, buf, size);
        close(fd);
        number = atoi(buf);

        if(number == 0 && isPressed == 0)
        {
            fd = open("/sys/class/gpio/export", O_WRONLY);
            if(fd < 0 )
            {
                printf("Can not open export file.\n");
                exit(1);
            }
            if(write(fd, "18\n", 3) != 3)
            {
                printf("Can not write in export file.\n");
                exit(1);
            }
            close(fd);

            if(pthread_create(&thread, NULL, thread_blink, NULL) != 0 )
            {
                printf("pthread_create failed.\n");
                return 1;
            }
            isPressed++;
            usleep(500000);
        }
        else if(number == 0 && isPressed == 1 )
        {
            isPressed = 0;
            pthread_cancel(thread);
            fd = open("/sys/class/gpio/unexport", O_WRONLY);
            if(fd < 0)
            {
                printf("Can not open unexport file.\n");
                exit(1);
            }
            if(write(fd, "18\n", 3) != 3)
            {
                printf("Can not write in unexport file.\n");
                exit(1);
            }
            close(fd);

            usleep(500000);

        }
    }
    //pthread_join(thread, NULL);
    return 0;
}
