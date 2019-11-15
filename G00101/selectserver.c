#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ifaddrs.h>
#include <stddef.h>
#include <sys/errno.h>

#include "selectserver.h"
#include "runningopt.h"

int clientConnect(opt option)
{
    int ret;

    fd_set serverfdsr;

    client_info *client[MAXCONNECTIONNUM];
    fdset fds;
    fdsetReset(&fds);

    while(1)
    {
        dataSend(&fds, client);
        dataRecv(&fds, client);
        FD_ZERO(&serverfdsr);
        FD_SET(option.Socketfd, &serverfdsr);
        ret = select(option.Socketfd + 1, &serverfdsr, NULL, NULL, 0);
        if (ret < 0 && errno != EINTR)
        {
            printf("select error\n");
            exit(-1);
        }
        else
        {
            if (errno == EINTR && ret < 0)
                continue;
        }

        if (ret > 0)
        {
            client[fds.fdNum]=(client_info*)malloc(sizeof(client_info));
            client[fds.fdNum]->client_socket=accept(option.Socketfd, (struct sockaddr *)&client[fds.fdNum]->client_addr, &client[fds.fdNum]->addr_size);
            if (client[fds.fdNum]->client_socket > 0)
            {
                fcntl(client[fds.fdNum]->client_socket, F_SETFL, fcntl(client[fds.fdNum]->client_socket, F_GETFL, 0) | O_NONBLOCK);
                fdsetUpdate(&fds, client[fds.fdNum]->client_socket);
            }
        }
        fdsetZeroSet(&fds);
    }
}

int dataRecv(fdset *fdst, client_info *clientSet[])
{
    if (fdst->fdNum == 0)
        return 0;
    int i;
    while(1)
    {
        for (i = 0; i < fdst->fdMaxNum; i++)
        {
        }
    }
    
}

int dataSend(fdset *fdst, client_info *clientSet[])
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
    return 0;
    
}