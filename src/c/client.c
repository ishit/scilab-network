#if defined(__linux__)
/* Linux  */

#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

#endif

#include <stdio.h>
#include <stdlib.h>
/*
int main(){
    client(1234);
    return 0;
}
*/

int client(char *ip, int port){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buff[256];
    int nread; 

    //Get socket fd
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) > 0){
        printf("Socket ID received.\n");
    }

    //Preparing serv_addr struct
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bzero(&(serv_addr.sin_zero), 8);
        
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0){
        memset(buff, 0, sizeof(buff));
        nread = recv(sockfd, buff, sizeof(buff), 0);
        if(nread > 0){  
            printf("%s\n", buff);
        }
    }

    else{
        printf("Connection not established. Try reconnecting.\n");
    }

    return 0;
}
