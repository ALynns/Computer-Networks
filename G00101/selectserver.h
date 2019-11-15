#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#endif

#include "runningopt.h"

//最大连接数
#define MAXCONNECTIONNUM 1000

//客户端信息状态：未发送需求信息，已发送需求信息未接收需求数据，已接收需求数据
typedef enum ClientStatus
{
    CS_NOSDNORECV,
    CS_SDNORECV,
    CS_SDRECV
}ClientStatus;

/*存储accept所得的客户端信息
    int client_socket;客户端套接字
    struct sockaddr_in client_addr;客户端信息
    int addr_size;客户端长度
    int stuNo;是否发送学号：0未发送未接收
    int pid;
    int time;
    int str;
    int strLength;
*/
typedef struct client_info
{
    int client_socket;
    struct sockaddr_in client_addr;
    int addr_size;
    ClientStatus stuNo;
    ClientStatus pid;
    ClientStatus time;
    ClientStatus str;
    ClientStatus strLength;
    ClientStatus endInfo;
} client_info;

/*存储select所需的信息
    fd_set fdsr;描述符集
    int fdMaxnum当前最大描述符连接量
    int fdNum;描述符数量
    int maxfds;当前最大描述符
    int fds[MAXCONNECTIONNUM];描述符数组
*/
typedef struct fdset
{
    fd_set fdsr;
    int fdMaxNum;
    int fdNum;
    int maxfds;
    int fds[MAXCONNECTIONNUM];
}fdset;

int clientConnect(opt option);
int dataRecv(fdset *fdst, client_info *clientSet[]);
int dataSend(fdset *fdst, client_info *clientSet[]);

//fdset初始化，函数最开始时调用一次
int fdsetReset(fdset *fdst);

//每次调用select前初始化fdsr，每次调用select前调用一次
int fdsetZeroSet(fdset *fdst);

//添加一个新的描述符进入fdset中
int fdsetUpdate(fdset *fdst,int socketfd);

//关闭连接
int fdsetClose(fdset *fdst, int socketfd);



