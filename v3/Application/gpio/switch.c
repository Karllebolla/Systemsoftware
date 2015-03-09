#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    char buf[2];
    int fd = 0;
    int size = sizeof(buf);
    int countSwitch = 0;
    int number = 1;


    while(1)
    {
        fd = open("/sys/class/gpio/export", O_WRONLY);
        if(fd < 0 )
        {
            printf("Export failed\n");
            return 1;
        }

        if(write(fd, "17\n", 3) != 3)
        {
            printf("Write gpio 17 failed export.\n");
            return 1;
        }

        if(close(fd) != 0)
        {
            printf("File descriptor for export gpio 17 failed.\n");
            return 1;
        }


        fd = open("/sys/class/gpio/gpio17/value", O_RDONLY);
        if(fd < 0)
        {
            printf("Open value file failed.\n");
            return 1;
        }

        read(fd, buf, size);

        number = atoi(buf);

	
        if(number == 0)
        {
            countSwitch++;
            printf("Inhalt von countSwitch: %d\n", countSwitch);
            usleep(500000);
        }


        close(fd);

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if(fd < 0)
        {
            printf("Unexoprt failed\n");
            return 1;
        }

        if(write(fd, "17\n", 3) != 3)
        {
            printf("Write gpio 17 failed unexport.\n");
            return 1;
        }

        close(fd);
    }

    return 0;
}
