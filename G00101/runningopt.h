#ifndef __RUNNIGOPT_H
#define __RUNNINGOPT_H
#endif

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
    unsigned long IPAddr;�޷���������ʽ��IP��ַ
    int Port;�˿ں�
    BlockType Block_Type;�Ƿ�����
    ForkType Fork_Type;�Ƿ�����ӽ��̷�������
    BlockFuncType BlockFunc_Type;�����ĺ���
    int Num;������
*/
typedef struct opt
{
    unsigned long IPAddr;
    int Port;
    BlockType Block_Type;
    ForkType Fork_Type;
    BlockFuncType BlockFunc_Type;
    int Num;
} opt;

//����Ĭ�ϲ����ĳ�ʼ������
int resetOpt(opt *optio);

//���ò����ṹ�壬ǰ����ֱ��Ӧint argc��char* argv[]
int optSet(int optc,char *optv[],opt *option);

//��ӡ�ṹ������ݲ��Ժ���
int printOpt(opt option);



