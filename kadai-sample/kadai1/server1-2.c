#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sock.h"
#include "counter.h"

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_count(int *p){
    int c;
    
    pthread_mutex_lock(&count_mutex);
    c = get_count();
    set_count(c + 1);
    pthread_mutex_unlock(&count_mutex);

    sleep(5);
    
    sock_send_intval(c + 1, p);
}

int main(void)
{
  int server_sockfd = sock_bind(1234);

  printf("server waiting\n");

  while(1) {
    int *client_sockfd = sock_accept(server_sockfd);
    
    pthread_t thread;
        pthread_create(&thread, NULL, (void *(*)(void *))send_count, client_sockfd);
        pthread_detach(thread);
  }
}

