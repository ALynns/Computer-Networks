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
            client[fds.fdMaxNum]=(client_info*)malloc(sizeof(client_info));
            client[fds.fdMaxNum]->client_socket=accept(option.Socketfd, NULL, NULL);
            if (client[fds.fdMaxNum]->client_socket > 0)
            {
                printf("Client %d connected\n", fds.fdMaxNum);
                fcntl(client[fds.fdMaxNum]->client_socket, F_SETFL, fcntl(client[fds.fdMaxNum]->client_socket, F_GETFL, 0) | O_NONBLOCK);
                fdsetUpdate(&fds, client[fds.fdMaxNum]->client_socket);
            }
        }
        fdsetZeroSet(&fds);
    }
}

int dataRecv(fdset *fdst, client_info *clientSet[])
{
    if (fdst->fdMaxNum == 0)
        return 0;
    int i, ret;
    while(1)
    {
        fdsetZeroSet(fdst);
        ret = select(fdst->fdMaxNum + 1, &fdst->fdsr, NULL, NULL, 0);
        if (ret < 0 && errno != EINTR)
        {
            printf("select error,%d\n", ret);
            exit(-1);
        }
        else
        {
            if (ret > 0)
                break;
            if (errno == EINTR)
                continue;
            else
                return 0;
            
        }
    }
    for (i = 0; i < fdst->fdMaxNum; i++)
    {
        if (FD_ISSET(fdst->fds[i], &fdst->fdsr))
        {
            switch (clientSet[i]->client_status)
            {
                case CS_SDSTUNO_RECVNO:
                {
                    ret = recv(clientSet[i]->client_socket, &clientSet[i]->StuNo, sizeof(int), 0);
                    clientSet[i]->client_status = CS_SDSTUNO_RECVSTUNO;
                    break;
                }
                case CS_SDPID_RECVNO:
                {
                    ret = recv(clientSet[i]->client_socket, &clientSet[i]->pid, sizeof(int), 0);
                    clientSet[i]->client_status = CS_SDPID_RECVPID;
                    break;
                }
                case CS_SDTIME_RECVNO:
                {
                    ret = recv(clientSet[i]->client_socket, clientSet[i]->Time, 19, 0);
                    clientSet[i]->client_status = CS_SDTIME_RECVTIME;
                    break;
                }
                case CS_SDSTR_RECVNO:
                {
                    char buf[BUFSIZE];
                    ret = recv(clientSet[i]->client_socket, clientSet[i]->Time, 19, 0);
                    clientSet[i]->client_status = CS_SDTIME_RECVTIME;
                    dataFileWrite(*clientSet[i], buf);
                    break;
                }
                
                default:
                    break;
            }
        }    
    }
}

int dataSend(fdset *fdst, client_info *clientSet[])
{
    if (fdst->fdMaxNum == 0)
        return 0;
    int ret, i;
    while(1)
    {
        fdsetZeroSet(fdst);
        ret = select(fdst->maxfd + 1, NULL, &fdst->fdsr, NULL, 0);
        if (ret < 0 && errno != EINTR)
        {
            printf("select error,%d\n", ret);
            exit(-1);
        }
        else
        {
            if (ret > 0)
                break;
            if(errno==EINTR)
            {
                continue;
            }    
            else
                return 0;
        }
    }

    for (i = 0; i < fdst->fdMaxNum; i++)
    {
        if(!FD_ISSET(fdst->fds[i],&fdst->fdsr))
            continue;
        else
        {
            switch (clientSet[i]->client_status)
            {
                case CS_NOSDANY:
                {
                    ret = send(clientSet[i]->client_socket, "StuNo", 5, 0);
                    clientSet[i]->client_status=CS_SDSTUNO_RECVNO;
                    break;
                }
                case CS_SDSTUNO_RECVSTUNO:
                {
                    ret = send(clientSet[i]->client_socket, "pid", 3, 0);
                    clientSet[i]->client_status=CS_SDPID_RECVNO;
                    break;
                }
                case CS_SDPID_RECVPID:
                {
                    ret = send(clientSet[i]->client_socket, "TIME", 5, 0);
                    clientSet[i]->client_status=CS_SDSTUNO_RECVNO;
                    break;
                }
                case CS_SDTIME_RECVTIME:
                {
                    srand(time(0));
                    char buf[9];
                    strcat(buf,"str");
                    sprintf(&buf[3], "%d", rand() % (99999 - 32768 + 1) + 32768);
                    ret = send(clientSet[i]->client_socket, buf, 9, 0);
                    clientSet[i]->client_status=CS_SDSTR_RECVNO;
                    break;
                }
                case CS_SDSTR_RECVSTR:
                {
                    ret = send(clientSet[i]->client_socket, "end", 3, 0);
                    clientSet[i]->client_status=CS_SDSTR_RECVNO;
                    fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                    break;
                } 
                
                default:
                    break;
            }
            /* code */
        }
        
        
        
    }

}

int dataFileWrite(client_info clientSet, char *str)
{
    FILE *fp = NULL;
    char fileName[20];
    strcat(fileName,"./txt/");
    sprintf(&fileName[sizeof(fileName)],"%s",clientSet.StuNo);
    strcat(fileName, ".");
    sprintf(&fileName[sizeof(fileName)],"%s",clientSet.pid);
    strcat(fileName,".pid.txt");
    fp=fopen(fileName,"a");
    fprintf(fp,"%d",ntohl(clientSet.StuNo));
    fprintf(fp, "%d", ntohl(clientSet.pid) << 16 + clientSet.client_socket);
    fprintf(fp,"%s",clientSet.Time);
    fprintf(fp,"%s",str);
    return 0;
}

int fdsetReset(fdset *fdst)
{
    fdst->fdNum=0;
    fdst->fdMaxNum=0;
    fdst->maxfd=0;
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
    if (socketfd > fdst->maxfd)
        fdst->maxfd = socketfd;
}

int fdsetClose(fdset *fdst, client_info *clientSet[], int socketfd)
{
    close(socketfd);
    FD_CLR(socketfd, &fdst->fdsr);
    --fdst->fdNum;

    if (fdst->maxfd == socketfd)
        fdst->maxfd = 0;

    int i;
    for(i=0;i<fdst->fdMaxNum;++i)
    {
        if(fdst->fds[i]==socketfd)
        {
            free(clientSet[i]);
            fdst->fds[i] = -1;
        }
        else
            continue;

        if(fdst->fds[i]>fdst->fdMaxNum)
        {
            fdst->fdMaxNum=fdst->fds[i];
            continue;
        }
    }
    return 0;
    
}