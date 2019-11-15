#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#endif

<<<<<<< HEAD

#include "runningopt.h"

=======
>>>>>>> 4d95ba27eb9c253c696af58e240244f1462151c0
//最大连接数
#define MAXCONNECTIONNUM 1000

/*存储accept所得的客户端信息
    int client_socket;客户端套接字
    struct sockaddr_in client_addr;客户端信息
    int addr_size;客户端长度
*/
typedef struct client_info
{
    int client_socket;
    struct sockaddr_in client_addr;
    int addr_size;
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
int dataRecv(fdset *fdst);
int dataSend(fdset *fdst);

//fdset初始化，函数最开始时调用一次
int fdsetReset(fdset *fdst);

//每次调用select前初始化fdsr，每次调用select前调用一次
int fdsetZeroSet(fdset *fdst);

//添加一个新的描述符进入fdset中
int fdsetUpdate(fdset *fdst,int socketfd);

//
int fdsetClose(fdset *fdst, int socketfd);



