#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#endif

#include "runningopt.h"

//���������
#define MAXCONNECTIONNUM 1000

//�ͻ�����Ϣ״̬��δ����������Ϣ���ѷ���������Ϣδ�����������ݣ��ѽ�����������
typedef enum ClientStatus
{
    CS_NOSDNORECV,
    CS_SDNORECV,
    CS_SDRECV
}ClientStatus;

/*�洢accept���õĿͻ�����Ϣ
    int client_socket;�ͻ����׽���
    struct sockaddr_in client_addr;�ͻ�����Ϣ
    int addr_size;�ͻ��˳���
    int stuNo;�Ƿ���ѧ�ţ�0δ����δ����
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
int dataRecv(fdset *fdst, client_info *clientSet[]);
int dataSend(fdset *fdst, client_info *clientSet[]);

//fdset��ʼ���������ʼʱ����һ��
int fdsetReset(fdset *fdst);

//ÿ�ε���selectǰ��ʼ��fdsr��ÿ�ε���selectǰ����һ��
int fdsetZeroSet(fdset *fdst);

//���һ���µ�����������fdset��
int fdsetUpdate(fdset *fdst,int socketfd);

//�ر�����
int fdsetClose(fdset *fdst, int socketfd);



