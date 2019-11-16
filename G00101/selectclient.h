#ifndef __SELECTSERVER_H
#define __SELECTSERVER_H
#include "runningopt.h"
#endif


//���������
#define MAXCONNECTIONNUM 1000
#define BUFSIZE 100000

/*�ͻ�����Ϣ״̬:δ�����κ�������Ϣ��׼������StuNo����
                �ѷ���StuNo������Ϣδ�������ݣ�
                �ѽ���StuNo����,׼������PID����
                �ѷ���PID������Ϣδ�������ݣ�
                �ѽ���PID����,׼������TIME����
                �ѷ���TIME������Ϣδ�������ݣ�
                �ѽ���Str����,׼������END����
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

/*�洢accept���õĿͻ�����Ϣ
    int client_socket;�ͻ����׽���
    struct sockaddr_in client_addr;�ͻ�����Ϣ
    int addr_size;�ͻ��˳���
    ClientStatus client_status;�ͻ���״̬
    int StuNo;�������������
    int pid;
    char Time[19];
    int strLength;Str����
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

/*�洢select�������Ϣ
    fd_set fdsr;��������
    int fdMaxnum��ǰ���������������
    int fdNum;����������
    int maxfd;��ǰ���������
    int fds[MAXCONNECTIONNUM];����������
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

//fdset��ʼ���������ʼʱ����һ��
int fdsetReset(fdset *fdst);

//ÿ�ε���selectǰ��ʼ��fdsr��ÿ�ε���selectǰ����һ��
int fdsetZeroSet(fdset *fdst);

//���һ���µ�����������fdset��
int fdsetUpdate(fdset *fdst,int socketfd);

//�ر�����
int fdsetClose(fdset *fdst, client_info *clientSet[], int socketfd);

