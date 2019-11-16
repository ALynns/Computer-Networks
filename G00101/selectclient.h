#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#include "runningopt.h"
#endif


//最大连接数
#define MAXCONNECTIONNUM 1000
#define BUFSIZE 100000

/*客户端信息状态:未发送任何需求信息，准备发送StuNo请求，
                已发送StuNo请求信息未接收数据，
                已接收StuNo数据,准备发送PID请求，
                已发送PID请求信息未接收数据，
                已接收PID数据,准备发送TIME请求，
                已发送TIME请求信息未接收数据，
                已接收Str数据,准备发送END请求，
                */
typedef enum ClientStatus
{
    CS_NOSDANY,
    CS_SDSTUNO_RECVNO,
    CS_SDSTUNO_RECVSTUNO,
    CS_SDPID_RECVNO,
    CS_SDPID_RECVPID,
    CS_SDTIME_RECVNO,
    CS_SDTIME_RECVTIME,
    CS_SDSTR_RECVNO,
    CS_SDSTR_RECVSTR,
    CS_SDEND
}ClientStatus;

/*存储accept所得的客户端信息
    int client_socket;客户端套接字
    struct sockaddr_in client_addr;客户端信息
    int addr_size;客户端长度
    ClientStatus client_status;客户端状态
    int StuNo;所需的三个数据
    int pid;
    char Time[19];
    int strLength;Str长度
*/
typedef struct client_info
{
    int client_socket;
    //struct sockaddr_in client_addr;
    //int addr_size;
    ClientStatus client_status;
    int StuNo;
    int pid;
    char Time[19];
    int strLength;
} client_info;

/*存储select所需的信息
    fd_set fdsr;描述符集
    int fdMaxnum当前最大描述符连接量
    int fdNum;描述符数量
    int maxfd;当前最大描述符
    int fds[MAXCONNECTIONNUM];描述符数组
*/
typedef struct fdset
{
    fd_set fdsr;
    int fdMaxNum;
    int fdNum;
    int maxfd;
    int fds[MAXCONNECTIONNUM];
}fdset;

int clientConnect(opt option);
int dataRecv(fdset *fdst, client_info *clientSet[]);
int dataSend(fdset *fdst, client_info *clientSet[]);
int dataFileWrite(client_info clientSet, char *str);

//fdset初始化，函数最开始时调用一次
int fdsetReset(fdset *fdst);

//每次调用select前初始化fdsr，每次调用select前调用一次
int fdsetZeroSet(fdset *fdst);

//添加一个新的描述符进入fdset中
int fdsetUpdate(fdset *fdst,int socketfd);

//关闭连接
int fdsetClose(fdset *fdst, client_info *clientSet[], int socketfd);

