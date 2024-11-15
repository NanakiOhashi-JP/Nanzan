#include <stdio.h>
#include <pthread.h>
#include "sock.h"

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t game_start = PTHREAD_COND_INITIALIZER;

int connected_players = 0;

int client_sockets[4];

void *accept_player(void *p) {
    int client_sockfd = *(int *)p;
    free(p);
    
    pthread_mutex_lock(&count_mutex);
    
    connected_players++;

    if (connected_players == 4) {
        pthread_cond_broadcast(&game_start);
    } else {
        while (connected_players < 4) {
            pthread_cond_wait(&game_start, &count_mutex);
        }
    }

    pthread_mutex_unlock(&count_mutex);

    sock_send_intval(1, &client_sockfd);
    /* value 1 means ready to start */
}

int main(void) {
    int server_sockfd = sock_bind(4321);
    printf("server waiting\n");

    while (1) {
        int *client_sockfd = sock_accept(server_sockfd);

        pthread_t thread;
        pthread_create(&thread, NULL, accept_player, client_sockfd);
        pthread_detach(thread);
    }
}
