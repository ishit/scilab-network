#if defined(__linux__)

/* Linux */
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

/*Get client address*/
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int TcpOpen(int port)
{
    int sockfd = 0;
    int connectfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_storage client_addr;
    socklen_t sin_size;
    sin_size = sizeof(client_addr);
    char ipstr[INET_ADDRSTRLEN];
    char sendData[256];
	int pid;
    int yes = 1;

    /*Get socket descriptor*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (port > 65535 || port < 1025)
    {
        perror("Invalid port.");
        return 1;
    }
    serv_addr.sin_port = htons(port);

    /*Reuse sockets*/
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        printf("setsockopt");
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("Binding problem. Try using another port.\n");   
        return 1;
    }

    if(listen(sockfd, 10) == -1)
    {
        printf("Queue full.\n");
        return 1;
    }
    printf("Listening on port %d..\n", port);
    while (1)
    {
        connectfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (connectfd == -1) {
            continue;
        }
        inet_ntop(client_addr.ss_family,
            get_in_addr((struct sockaddr *)&client_addr),
            ipstr, sizeof(ipstr));
        printf("server: got connection from %s\n", ipstr);

        if (!fork()) { 
            close(sockfd);
            if (send(connectfd, "Hello, World!", 13, 0) == -1)
                perror("send");
            close(connectfd);
            exit(0);
        }
        close(connectfd);
        sleep(1);
    }
    return connectfd;
}

int close(int fd){
	shutdown(fd, SHUT_RDWR);
	return 0;    
}
