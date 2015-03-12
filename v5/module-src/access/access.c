#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "test_openclose.h"
#include "test_zero.h"
#include "test_write_null.h"
#include "test_read_null.h"
#include "test_buffer_ts.h"
#include "test_sema.h"
#include "test_mutex.h"

struct option longopts[] = {
	{"testOpenclose", required_argument, 0, 'o'},
	{"testZero", required_argument, 0, 'z'},
	{"testWriteNull", no_argument, 0, 'w'},
	{"testReadNull", no_argument, 0, 'r'},
	{"time", required_argument, 0, 't'},
	{"buffer", no_argument, 0, 'b'},
	{"sema", no_argument, 0, 's'},
	{"mutex", no_argument, 0, 'm'},
	{0,0,0,0}
};

int main (int argc, char **argv)
{ 
  int c, minor, delay, option_index=0;
  
  while ((c = getopt_long (argc, argv, "o:z:wrt:usm", longopts, &option_index)) != -1)
    switch (c)
      {
	  case 0:
		 if (longopts[option_index].flag != 0)
            break;
          printf ("option %s", longopts[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;
      case 'o':
		minor =atoi(optarg);		
		if(minor == 0) {
			test_openclose_with_minornumber_zero();
		} else {
			test_openclose_with_minornumber_one();
		}
        break;
      case 'z':
		minor = atoi(optarg);
		if(minor == 0) {
			test_zero_with_minornumber_zero_read_zero();
		} else {
			test_zero_with_minornumber_one_read_hello();
		}
        break;
      case 'w':
		test_write_null();
        break;
      case 'r':
		test_read_null();
        break;
	  case 't':
		delay =atoi(optarg);
		delay_openclose = delay;
		printf("t = openclose\n");
        break;
      case 'b':
		test_buffer();
		break;
	  case 's':
		test_sema();
		break;
	  case 'm':
		test_mutex();
		break;
      case '?':
		fprintf(stdout, "FAIL\n");
		return 1;
      default:
        abort ();
      }
  return 0;
}
