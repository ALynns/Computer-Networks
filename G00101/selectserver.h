#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#endif

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

typedef struct fdset
{
    fd_set fdsr;
    int fdNum;
    int maxfds;
    int fds[MAXCONNECTIONNUM];
}fdset;

int clientConnect(opt option);
int dataRecv();
int dataSend();


