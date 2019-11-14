#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#endif

//���������
#define MAXCONNECTIONNUM 1000

/*�洢accept���õĿͻ�����Ϣ
    int client_socket;�ͻ����׽���
    struct sockaddr_in client_addr;�ͻ�����Ϣ
    int addr_size;�ͻ��˳���
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


