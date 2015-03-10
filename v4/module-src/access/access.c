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
#include "test_openclose.h"
#include "test_zero.h"
#include "test_write_null.h"

int main (int argc, char **argv)
{ 
  int c, minor, delay;
  opterr = 0;

  while ((c = getopt (argc, argv, "o:z:nt:")) != -1)
    switch (c)
      {
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
      case 'n':
		test_write_null();
        break;
	  case 't':
		delay =atoi(optarg);
		delay_openclose = delay;
        break;
      case '?':
		fprintf(stdout, "FAIL\n");
		return 1;
      default:
        abort ();
      }
  return 0;
}
