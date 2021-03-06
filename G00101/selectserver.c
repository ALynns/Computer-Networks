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

int serverConnect(opt option)
{
    int ret;

    fd_set serverfdsr;

    client_info *client[MAXCONNECTIONNUM];
    fdset fds;
    fdsetReset(&fds);
    
    srand(time(0));
    while(1)
    {
        getchar();
        printf("start\n");
        dataSend(&fds, client);
        dataRecv(&fds, client);
        getchar();
        printf("mid\n");
        FD_ZERO(&serverfdsr);
        FD_SET(option.Socketfd, &serverfdsr);
        struct timeval tv = {0, 0};
        ret = select(option.Socketfd + 1, &serverfdsr, NULL, NULL, &tv);
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
            client[fds.fdMaxNum] = (client_info *)malloc(sizeof(client_info));
            client[fds.fdMaxNum]->str = NULL;

            client[fds.fdMaxNum]->client_socket=accept(option.Socketfd, NULL, NULL);
            if (client[fds.fdMaxNum]->client_socket > 0)
            {
                printf("Client %d connected\n", fds.fdMaxNum);
                fcntl(client[fds.fdMaxNum]->client_socket, F_SETFL, fcntl(client[fds.fdMaxNum]->client_socket, F_GETFL, 0) | O_NONBLOCK);
                client[fds.fdMaxNum]->client_status = CS_NOSDANY;
                fdsetUpdate(&fds, client[fds.fdMaxNum]->client_socket);
            }
        }
        if (fds.fdMaxNum > 0 && fds.fdNum == 0)
            return 0;
    }
}

int dataRecv(fdset *fdst, client_info *clientSet[])
{
    if (fdst->fdMaxNum == 0)
        return 0;
    int i, ret;
    
    struct timeval tv = {0, 0};
    while(1)
    {
        fdsetZeroSet(fdst);
        
        ret = select(fdst->maxfd + 1, &fdst->fdsr, NULL, NULL, &tv);
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
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while(1)
                    {
                        if (temp >= sizeof(int))
                            break;
                        ret = recv(clientSet[i]->client_socket, &clientSet[i]->StuNo + temp, sizeof(int) - temp, 0);
                        if(ret>0)
                            temp += ret;
                        if (time(0) - clientSet[i]->startTime > 0)
                        {
                            fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                            printf("Connection%d StuNo Error\n",i);
                            break;
                        }    
                    }
                    clientSet[i]->client_status = CS_SDSTUNO_RECVSTUNO;
                    break;
                }
                case CS_SDPID_RECVNO:
                {
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while(1)
                    {
                        if (temp >= sizeof(int))
                            break;
                        ret = recv(clientSet[i]->client_socket, &clientSet[i]->pid + temp, sizeof(int) - temp, 0);
                        if (ret > 0)
                            temp += ret;
                        if (time(0) - clientSet[i]->startTime > 0)
                        {
                            fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                            printf("Connection%d pid Error\n",i);
                            break;
                        }    
                    }
                    clientSet[i]->client_status = CS_SDPID_RECVPID;
                    break;
                }
                case CS_SDTIME_RECVNO:
                {
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while(1)
                    {
                        if (temp >= 19)
                            break;
                        ret = recv(clientSet[i]->client_socket, clientSet[i]->Time + temp, 19 - temp, 0);
                        if (ret > 0)
                            temp += ret;
                        if (time(0) - clientSet[i]->startTime > 0)
                        {
                            fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                            printf("Connection%d TIME Error\n",i);
                            break;
                        }  
                    }
                    clientSet[i]->client_status = CS_SDTIME_RECVTIME;
                    break;
                }
                case CS_SDSTR_RECVNO:
                {
                    if(clientSet[i]->str==NULL)
                    {
                        clientSet[i]->str = (char *)malloc(clientSet[i]->strLength + 1);
                        clientSet[i]->str[clientSet[i]->strLength] = 0;
                    }
                    ret = 1;
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while (1)
                    {
                        if (temp >= clientSet[i]->strLength)
                            break;
                        ret = recv(clientSet[i]->client_socket, clientSet[i]->str + temp, clientSet[i]->strLength - temp, 0);
                        if (ret > 0)
                            temp += ret;
                        if (time(0) - clientSet[i]->startTime > 3)
                        {
                            printf("Connect %d overtime\n", i);
                            temp = 0;
                            clientSet[i]->startTime = time(0);
                            break;
                        }    
                    }
                    if(temp)
                        clientSet[i]->client_status = CS_SDSTR_RECVSTR;
                    else
                        break;

                    dataFileWrite(*clientSet[i], clientSet[i]->str);
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
    struct timeval tv = {0, 0};
    while(1)
    {
        fdsetZeroSet(fdst);
        ret = select(fdst->maxfd + 1, NULL, &fdst->fdsr, NULL, &tv);
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
                printf("%d\n", clientSet[i]->client_status);
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
                    clientSet[i]->client_status=CS_SDTIME_RECVNO;
                    break;
                }
                case CS_SDTIME_RECVTIME:
                {
                    char buf[9];
                    strcat(buf,"str");
                    clientSet[i]->strLength = rand() % (99999 - 32768 + 1) + 32768;
                    sprintf(&buf[3], "%d", clientSet[i]->strLength);
                    ret = send(clientSet[i]->client_socket, buf, 9, 0);
                    clientSet[i]->client_status=CS_SDSTR_RECVNO;
                    break;
                }
                case CS_SDSTR_RECVSTR:
                {
                    ret = send(clientSet[i]->client_socket, "end", 3, 0);
                    clientSet[i]->client_status=CS_SDSTR_RECVNO;
                    if(clientSet[i]->str)
                        free(clientSet[i]->str);
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
    char fileName[50] = {0};
    strcat(fileName,"./txt/");

    sprintf(&fileName[strlen(fileName)], "%d", ntohl(clientSet.StuNo));
    strcat(fileName, ".");

    sprintf(&fileName[strlen(fileName)], "%d", ntohl(clientSet.pid));

    strcat(fileName, ".pid.txt");
    
    fp = fopen(fileName, "w+");
    fclose(fp);
    fp = fopen(fileName, "a");
    fprintf(fp, "%d\n", ntohl(clientSet.StuNo));
    fprintf(fp, "%d\n", ntohl(clientSet.pid));
    fprintf(fp, "%s\n", clientSet.Time);
    fprintf(fp, "%s", str);
    fclose(fp);
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
            if(clientSet[i])
                free(clientSet[i]);
            clientSet[i] = NULL;
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