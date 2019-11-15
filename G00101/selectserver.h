#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#endif

<<<<<<< HEAD

#include "runningopt.h"

=======
>>>>>>> 4d95ba27eb9c253c696af58e240244f1462151c0
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

/*�洢select�������Ϣ
    fd_set fdsr;��������
    int fdMaxnum��ǰ���������������
    int fdNum;����������
    int maxfds;��ǰ���������
    int fds[MAXCONNECTIONNUM];����������
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

//fdset��ʼ���������ʼʱ����һ��
int fdsetReset(fdset *fdst);

//ÿ�ε���selectǰ��ʼ��fdsr��ÿ�ε���selectǰ����һ��
int fdsetZeroSet(fdset *fdst);

//���һ���µ�����������fdset��
int fdsetUpdate(fdset *fdst,int socketfd);

//
int fdsetClose(fdset *fdst, int socketfd);



