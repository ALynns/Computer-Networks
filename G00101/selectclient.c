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

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(0);

    client_info *client[MAXCONNECTIONNUM];
    fdset fds;
    fdsetReset(&fds);

    
    srand(time(0));

    for (i = 0; i < option.Num; ++i)
    {
        dataRecv(&fds, client);
        dataSend(&fds, client);
        client[i] = (client_info *)malloc(sizeof(client_info));
        client[i]->client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        fcntl(client[i]->client_socket, F_SETFL, fcntl(client[i]->client_socket, F_GETFL, 0) | O_NONBLOCK);
        client[i]->str = NULL;
        client[i]->client_status = CS_NOSDANY;
        fdsetUpdate(&fds, client[i]->client_socket);
        fdsetZeroSet(&fds);

        ret = bind(client[i]->client_socket, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        if (ret < 0)
        {
            printf("bind error\n");
            exit(-1);
        }

        

        ret = connect(client[i]->client_socket, (struct sockaddr *)&serviceAddr, sizeof(serviceAddr));
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
    if(ret==0)
    {
        printf("Connection error\n");
        exit(-1);
    }
    while(1)
    {
        dataRecv(&fds, client);
        dataSend(&fds, client);
        if(fds.fdNum==0&&fds.fdMaxNum==option.Num)
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
        if (clientSet[i] && clientSet[i]->client_status == CS_SDSTR_RECVSTR && (time(0) - clientSet[i]->startTime > 3))
        {
            clientSet[i]->client_status=CS_SDSTR_RECVNO;
            printf("Connect %d resend\n", i);
        }    
        if (fdst->fds[i] != -1 && FD_ISSET(fdst->fds[i], &fdst->fdsr))
        {
            char buf[9];
            switch (clientSet[i]->client_status)
            {
                printf("%d\n", clientSet[i]->client_status);
                case CS_NOSDANY:
                {
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while(1)
                    {
                        if (temp >= 5)
                            break;
                        ret = recv(clientSet[i]->client_socket, buf, 5, 0);
                        buf[5] = 0;
                        if (ret > 0)
                            temp += ret;
                        if (strcmp(buf, "StuNo")&&(time(0) - clientSet[i]->startTime > 0))
                        {
                            temp = 0;
                            printf("Connect%d ERROR StuNo\n", i);
                            fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                            break;
                        }
                    }
                    if(temp)
                        clientSet[i]->client_status = CS_SDSTUNO_RECVNO;
                    break;
                }
                case CS_SDSTUNO_RECVSTUNO:
                {
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while(1)
                    {
                        if (temp >= 3)
                            break;
                        ret = recv(clientSet[i]->client_socket, buf, 3, 0);
                        buf[3] = 0;
                        if (ret > 0)
                            temp += ret;
                        if (strcmp(buf, "pid")&&(time(0) - clientSet[i]->startTime > 0))
                        {
                            temp = 0;
                            printf("Connect%d ERROR pid\n", i);
                            fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                            break;
                        }
                        if(temp)
                            clientSet[i]->client_status = CS_SDPID_RECVNO;
                    }
                    break;
                }
                case CS_SDPID_RECVPID:
                {
                    int temp = 0;
                    clientSet[i]->startTime = time(0);
                    while(1)
                    {
                        if (temp >= 4)
                            break;
                        ret = recv(clientSet[i]->client_socket, buf, 5, 0);
                        
                        if (ret > 0)
                            temp += ret;
                        if (strcmp(buf, "TIME")&&(time(0) - clientSet[i]->startTime > 0))
                        {
                            temp = 0;
                            printf("Connect%d ERROR TIME\n", i);
                            fdsetClose(fdst, clientSet, clientSet[i]->client_socket);
                            break;
                        }
                    }
                    if(temp)
                        clientSet[i]->client_status = CS_SDTIME_RECVNO;
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
                    memcpy(buf, "\0\0\0\0", 4);

                    ret = recv(clientSet[i]->client_socket, buf, 3, 0);
                    if(ret==3)
                    {
                        clientSet[i]->client_status = CS_SDEND;
                    }
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
                case CS_SDSTUNO_RECVNO:
                {
                    clientSet[i]->StuNo = htonl(1753935);
                    ret = send(clientSet[i]->client_socket, &clientSet[i]->StuNo, 4, 0);
                    clientSet[i]->client_status=CS_SDSTUNO_RECVSTUNO;
                    break;
                }
                case CS_SDPID_RECVNO:
                {
                    clientSet[i]->pid = htonl((getpid() << 16) + clientSet[i]->client_socket);
                    ret = send(clientSet[i]->client_socket, &clientSet[i]->pid, 4, 0);
                    clientSet[i]->client_status=CS_SDPID_RECVPID;
                    break;
                }
                case CS_SDTIME_RECVNO:
                {
                    getTime(clientSet[i]->Time);
                    ret = send(clientSet[i]->client_socket, clientSet[i]->Time, 19, 0);
                    clientSet[i]->client_status=CS_SDTIME_RECVTIME;
                    break;
                }
                case CS_SDSTR_RECVNO:
                {
                    if (clientSet[i]->str == NULL)
                    {
                        clientSet[i]->str = (char *)malloc(clientSet[i]->strLength + 1);
                        if (clientSet[i]->str == NULL)
                        {
                            printf("str ERROR\n");
                            exit(-1);
                        }
                        clientSet[i]->str[clientSet[i]->strLength] = 0;
                        int j;
                        for (j = 0; j < clientSet[i]->strLength; ++j)
                            clientSet[i]->str[j] = rand() % 254 + 1;
                    }

                    ret = 1;
                    int temp = 0;
                    while (ret > 0)
                    {
                        ret = send(clientSet[i]->client_socket, clientSet[i]->str, clientSet[i]->strLength - temp, 0);
                        temp += ret;

                    }
                    clientSet[i]->startTime = time(0);
                    clientSet[i]->client_status = CS_SDSTR_RECVSTR;
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
    
    sprintf(date,"%04d-%02d-%02d %02d:%02d:%02d",pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,pTM->tm_hour,pTM->tm_min,pTM->tm_sec);
    return 0;
}