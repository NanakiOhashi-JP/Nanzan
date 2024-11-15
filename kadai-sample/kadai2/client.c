#include <stdio.h>

#include "sock.h"

int main(void)
{
  int res = sock_getnum("127.0.0.1", 4321);

  if (res == 1) 
      printf("Mahjong game is now ready to start!\n");

  return(0);
}

