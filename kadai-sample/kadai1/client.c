#include <stdio.h>
#include <time.h>

#include "sock.h"

int main(void)
{
  time_t      start_time;
  start_time = time(NULL);

  printf("Count number is %d. \n",sock_getnum("127.0.0.1", 1234)); 

  printf("Elapsed time: %.1f sec.\n", difftime(time(NULL), start_time));
  return(0);
}

