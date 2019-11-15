#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ifaddrs.h>
#include <stddef.h>

#include "selectserver.h"
#include "runningopt.h"

int clientConnect(opt option)
{
    client_info *client[MAXCONNECTIONNUM];
    fdset fds;
    int ret;

    fdsetZeroSet(&fds);

    while(1)
    {

        fdsetZeroSet(&fds);
    }
}

int dataRecv(fdset *fdst)
{
    if (fdst->fdNum == 0)
        return 0;
}

int dataSend(fdset *fdst)
{
    if (fdst->fdNum == 0)
        return 0;
}

int fdsetReset(fdset *fdst)
{
    fdst->fdNum=0;
    fdst->fdMaxNum=0;
    fdst->maxfds=0;
    FD_ZERO(&fdst->fdsr);
    int i;
    for (i = 0; i < MAXCONNECTIONNUM; ++i)
    {
        fdst->fds[i] = 0;
    }
    return 0;
}

int fdsetZeroSet(fdset *fdst)
{
    FD_ZERO(&fdst->fdsr);
    int i;
    for(i=0;i<fdst->fdMaxNum;++i)
    {
        if(fdst->fds[i]!=-1)
            FD_SET(fdst->fds[i], &fdst->fdsr);
    }
    return 0;
}

int fdsetUpdate(fdset *fdst,int socketfd)
{
    fdst->fds[fdst->fdMaxNum] = socketfd;
    ++fdst->fdNum;
    ++fdst->fdMaxNum;
    if (socketfd > fdst->maxfds)
        fdst->maxfds = socketfd;
}


int fdsetClose(fdset *fdst, int socketfd)
{
    close(socketfd);
    FD_CLR(socketfd, &fdst->fdsr);
    --fdst->fdNum;

    if (fdst->maxfds == socketfd)
        fdst->maxfds = 0;

    int i;
    for(i=0;i<fdst->fdMaxNum;++i)
    {
        if(fdst->fds[i]==socketfd)
        {
            fdst->fds[i] = -1;
            continue;
        }    
        if(fdst->fds[i]>fdst->fdMaxNum)
        {
            fdst->fdMaxNum=fdst->fds[i];
            continue;
        }
    }

    
}