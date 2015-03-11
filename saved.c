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
#include "api_scilab.h"
#include "Scierror.h"
#define CHUNK_SIZE 256
int sci_server(char *fname);
int TcpOpen(int *port);

int sci_server(char *fname)
{
    SciErr sciErr;
    int iRet = 0;
    int *piAddr = NULL;
    double *port = 0;
    int nRows, nCols;
    int iPrec = 0;
    int iType = 0;
    int iPort;
    double retVal = 0;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if(sciErr.iErr){
    	//printError(&sciErr, 0);
	sciprint("The error.\n");
	//AssignOutputVariable(pvApiCtx, 1) = 0;
	return 1;
    }
    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if(sciErr.iErr || iType != sci_matrix){
    	printError(&sciErr, 0);
	return 1;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &nRows, &nCols, &port);
    iPort = (int)(*port);
    TcpOpen(&iPort);
    iRet = createScalarInteger32(pvApiCtx, nbInputArgument(pvApiCtx) + 1, retVal);
    if(iRet){
    	AssignOutputVariable(pvApiCtx, 1) = 0;
	return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int TcpOpen(int *port)
{
    int sockfd = 0;
    int connectfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_storage client_addr;
    socklen_t sin_size;
    sin_size = sizeof(client_addr);
    char ipstr[INET_ADDRSTRLEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (*port > 65535 || *port < 1025)
    {
        perror("Invalid port.");
        return 1;
    }
    serv_addr.sin_port = htons(*port);

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        sciprint("Binding problem. Try using another port.\n");   
        return 1;
    }

    if(listen(sockfd, 10) == -1)
    {
        sciprint("Queue full.\n");
        return 1;
    }
    sciprint("Listening on port %d..\n", *port);
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

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (send(connectfd, "Hello, world!", 13, 0) == -1)
                perror("send");
            close(connectfd);
            exit(0);
        }
        close(connectfd);
        sleep(1);
    }
    return 0;
}
