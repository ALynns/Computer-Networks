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
#include <sys/errno.h>

#include "selectclient.h"
#include "runningopt.h"

int clientConnect(opt option)
{
    int i, ret;
    struct sockaddr_in serviceAddr;
    memset(&serviceAddr,0,sizeof(serviceAddr));
    serviceAddr.sin_family=AF_INET;
    serviceAddr.sin_addr.s_addr=option.IPAddr;
    serviceAddr.sin_port=option.Port;

    client_info *client[MAXCONNECTIONNUM];
    fdset fds;
    fdsetReset(&fds);

    for (i = 0; i < option.Num; ++i)
    {
        dataRecv(&fds, client);
        dataSend(&fds, client);
        client[i] = (client_info *)malloc(sizeof(client_info));
        client[i]->client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        client[i]->client_status = CS_NOSDANY;
        fcntl(client[i]->client_socket, F_SETFL, fcntl(client[i]->client_socket, F_GETFL, 0) | O_NONBLOCK);
        fdsetUpdate(&fds, client[i]->client_socket);
        ret = connect(client[fds.fdMaxNum]->client_socket, (struct sockaddr *)&serviceAddr, sizeof(struct sockaddr_in));
        if (ret == -1)
        {
            if (errno != EINPROGRESS) //
            {
                printf("Connection error\n");
                exit(-1);
            }
            else
            {
                fdsetZeroSet(&fds);
                ret = select(fds.maxfd + 1, NULL, &fds.fdsr, NULL, NULL);

                if (ret < 0)
                {
                    printf("select error\n");
                    exit(-1);
                }
                else
                {
                    int error = -1, slen = sizeof(int);
                    getsockopt(client[i]->client_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&slen);
                    if (error == 0)
                    {
                        printf("Connection%d successful\n", i);
                        ret = 1;
                    }
                    else
                        ret = 0;
                }
            }
        }
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
        ret = select(fdst->maxfd + 1, &fdst->fdsr, NULL, NULL, 0);
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
            char buf[9];
            switch (clientSet[i]->client_status)
            {
                case CS_NOSDANY:
                {
                    ret = recv(clientSet[i]->client_socket, buf, 5, 0);
                    clientSet[i]->client_status = CS_SDSTUNO_RECVNO;
                    buf[5] = 0;
                    if(strcmp(buf,"StuNo"))
                        exit(-1);
                    break;
                }
                case CS_SDSTUNO_RECVSTUNO:
                {
                    ret = recv(clientSet[i]->client_socket, buf, 3, 0);
                    clientSet[i]->client_status = CS_SDPID_RECVNO;
                    buf[3] = 0;
                    if(strcmp(buf,"pid"))
                        exit(-1);
                    break;
                }
                case CS_SDPID_RECVPID:
                {
                    ret = recv(clientSet[i]->client_socket, buf, 5, 0);
                    clientSet[i]->client_status = CS_SDTIME_RECVNO;
                    if(strcmp(buf,"TIME"))
                        exit(-1);
                    break;
                }
                case CS_SDTIME_RECVTIME:
                {
                    ret = recv(clientSet[i]->client_socket, buf, 9, 0);
                    clientSet[i]->client_status = CS_SDSTR_RECVNO;
                    clientSet[i]->strLength = atoi(&buf[3]);
                    break;
                }
                case CS_SDSTR_RECVSTR:
                {
                    ret = recv(clientSet[i]->client_socket, buf, 3, 0);
                    clientSet[i]->client_status = CS_SDEND;
                    if(strcmp(buf,"end"))
                        exit(-1);
                    
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
                case CS_SDSTUNO_RECVNO:
                {
                    clientSet[i]->StuNo = htonl(1753935);
                    ret = send(clientSet[i]->client_socket, &clientSet[i]->StuNo, 4, 0);
                    clientSet[i]->client_status=CS_SDPID_RECVNO;
                    break;
                }
                case CS_SDPID_RECVNO:
                {
                    clientSet[i]->pid = getpid() << 16 + clientSet[i]->client_socket;
                    ret = send(clientSet[i]->client_socket, &clientSet[i]->pid, 4, 0);
                    clientSet[i]->client_status=CS_SDSTUNO_RECVNO;
                    break;
                }
                case CS_SDTIME_RECVNO:
                {
                    getTime(clientSet[i]->Time);
                    ret = send(clientSet[i]->client_socket, clientSet[i]->Time, 19, 0);
                    clientSet[i]->client_status=CS_SDSTR_RECVNO;
                    break;
                }
                case CS_SDSTR_RECVNO:
                {
                    clientSet[i]->str = (char *)malloc(clientSet[i]->strLength);
                    ret = send(clientSet[i]->client_socket, clientSet[i]->str, clientSet[i]->strLength, 0);
                    clientSet[i]->client_status=CS_SDSTR_RECVNO;
                    fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                    break;
                }
                case CS_SDEND:
                {
                    dataFileWrite(*clientSet[i], clientSet[i]->str);
                    free(clientSet[i]->str);
                    fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
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
    strcat(fileName, ".pid.txt");
    fp = fopen(fileName, "a");
    fprintf(fp, "%d\n", ntohl(clientSet.StuNo));
    fprintf(fp, "%d\n", ntohl(clientSet.pid));
    fprintf(fp, "%s\n", clientSet.Time);
    fprintf(fp, "%s", str);
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

int getTime(char *date)
{
    time_t time0;
    struct tm *pTM;
    
    time(&time0); 
    pTM = localtime(&time0);
    
    sprintf(date,"%04d-%02d-%02d %02d-%02d-%02d",pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,pTM->tm_hour,pTM->tm_min,pTM->tm_sec);
    return 0;
}