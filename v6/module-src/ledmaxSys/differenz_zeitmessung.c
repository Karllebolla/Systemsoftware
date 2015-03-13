#include "differenz_zeitmessung.h"

#define NANOSECONDS_PER_SECOND 1000000000

double diff_time(struct timespec *start, struct timespec *end)
{
	double ret = 0;
	struct timespec result;
	
	if ((end->tv_sec < start->tv_sec) ||
			((end->tv_sec == start->tv_sec) &&
				(end->tv_nsec <= start->tv_nsec))) { 
		/* end before start */
		result.tv_sec = result.tv_nsec = 0;
	}

	result.tv_sec = end->tv_sec - start->tv_sec;
	result.tv_nsec = end->tv_nsec - start->tv_nsec;

	if(result.tv_nsec < 0 )
	{
		result.tv_sec--;
		result.tv_nsec = NANOSECONDS_PER_SECOND + result.tv_nsec;
	}

	//printf("sec  : %d\n", result.tv_sec);
	//printf("nsec : %d\n", result.tv_nsec);

	ret = NANOSECONDS_PER_SECOND * result.tv_sec + result.tv_nsec;
	ret = ret / NANOSECONDS_PER_SECOND;
	//ret = (NANOSECONDS_PER_SECOND * result.tv_sec + result.tv_nsec) / NANOSECONDS_PER_SECOND;
	//ret =  result.tv_sec + ( result.tv_nsec / NANOSECONDS_PER_SECOND) ;
	
	//printf("result1 %lu\n", NANOSECONDS_PER_SECOND * result.tv_sec + result.tv_nsec);
	return ret;  
}
