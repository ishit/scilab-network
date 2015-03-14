#if defiend(__linux__)

/* Linux */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "api_scilab.h"
#include "stack-c.h"
#include "Scierror.h"
#include "stdlib.h"
#include "localization.h"
#define CHUNK_SIZE 256

int TcpOpen(int *port);

int sci_server(char *fname)
{
    CheckRhs(1, 1);
    CheckLhs(0, 1);
    SciErr sciErr;
    unsigned int *port;
    int *piAddr = NULL;
    int nRows, nCols;
    int iType = 0;

    sciErr = getAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if (iType != sci_ints)
    {
        Scierror(204, _("%sL Wrong type for input argument #%d: Integer expected.\n"), fname, 1);
        return 0;
    }
    sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, piAddr, &nRows, &nCols, &port);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    CheckDims(1, nRows, nCols, 1, 1);
    TcpOpen(port);
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
    socklen_t = sin_size;
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
    sciprint("Listening on port %d..", *port);
    while (1)
    {
        connectfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (connectfd == -1) {
            continue;
        }
        inet_ntop(client_addr.ss_family,
            (struct sockaddr *)(&client_addr->sin_addr),
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
