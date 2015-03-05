#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define MB 1000000

int main() {
	struct sysinfo info;
	
	if(sysinfo(&info)) {
		printf("Error while init the sysinfo struct.\n");
		return -1;
	}
	
	printf("Hello User world \n");
	printf("Uptime: %ld \n", info.uptime);
	printf("Total RAM: %lu MB\n", info.totalram / MB);
	printf("Free RAM: %lu MB\n", info.freeram / MB);
	printf("Process count %hu \n", info.procs);
	printf("Page size: %d B\n",info.mem_unit);
	sleep(1000);
	return 0;
}
