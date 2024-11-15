#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CONN 1

void sock_send_intval(int num, int *client_sockfd){
    write(*client_sockfd, &num, sizeof(num));
    close(*client_sockfd);
    free(client_sockfd);
}

int sock_bind(int portno){
    int server_sockfd, *client_sockfd ;
    int server_len;
    struct sockaddr_in server_addr ;
    int rc = 0;

    server_sockfd = socket(AF_INET,SOCK_STREAM,0);

    server_addr.sin_family = AF_INET ;
    server_addr.sin_addr.s_addr = INADDR_ANY ;
    server_addr.sin_port = htons(portno) ;

    server_len = sizeof(server_addr);
    rc = bind(server_sockfd , (struct sockaddr *)&server_addr, server_len);
    if(rc < 0){
        perror("Error: bind() ");
        exit(1);
    }

    listen(server_sockfd, MAX_CONN);

    return(server_sockfd);
}

int *sock_accept(int server_sockfd) {
    int *client_sockfd;
    client_sockfd =  (int *)malloc(sizeof(int));
    unsigned int client_len;
    struct sockaddr_in client_addr;
    client_sockfd =  (int *)malloc(sizeof(int));
    *client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
    return client_sockfd;
}

int sock_getnum(char *hostname, int portno){
    int sockfd;
    struct sockaddr_in addr;
    int num;

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(hostname);
    addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1 ) {
      perror("Connection error!! ");
      exit(1);
    }

    write(sockfd, NULL, 0);
    read(sockfd, &num, sizeof(num));
    close(sockfd);
    return(num);
}
