#ifndef __RUNNIGOPT_H
#define __RUNNINGOPT_H
#endif

//��������
typedef enum ProcType
{
    PT_SERVER,
    PT_CLIENT
}ProcType;

//�Ƿ�����
typedef enum BlockType
{
    BT_BLOCK,
    BT_NONBLOCK
} BlockType;

//�Ƿ�����ӽ��̷�������
typedef enum ForkType
{
    FT_FORK,
    FT_NOFORK
} ForkType;

//�����ĺ���
typedef enum BlockFuncType
{
    BFT_SELECT,
    BFT_POLL,
    BFT_EPOLL
}BlockFuncType;

/*
��������Ľṹ�壬���ݰ�����
    ProcType Proc_Type;��ǰ��������ͣ������/�ͻ���
    unsigned long IPAddr;�޷���������ʽ��IP��ַ
    int Port;�˿ں�
    BlockType Block_Type;�Ƿ�����
    ForkType Fork_Type;�Ƿ�����ӽ��̷�������
    BlockFuncType BlockFunc_Type;�����ĺ���
    int Num;������
    int socket;�����׽���
*/
typedef struct opt
{
    ProcType Proc_Type;
    unsigned long IPAddr;
    int Port;
    BlockType Block_Type;
    ForkType Fork_Type;
    BlockFuncType BlockFunc_Type;
    int Num;
    int Socketfd;
} opt;

//����Ĭ�ϲ����ĳ�ʼ������
int optReset(opt *optio);

//���ò����ṹ�壬ǰ����ֱ��Ӧint argc��char* argv[]
int optSet(opt *option,int optc,char *optv[],ProcType procType);

//���ñ����׽���
int optSocketSet(opt *option, int socket);

//��ӡ�ṹ������ݲ��Ժ���
int printOpt(opt option);



