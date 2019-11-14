#ifndef __RUNNIGOPT_H
#define __RUNNINGOPT_H
#endif

//是否阻塞
typedef enum BlockType
{
    BT_BLOCK,
    BT_NONBLOCK
} BlockType;

//是否分裂子进程方法连接
typedef enum ForkType
{
    FT_FORK,
    FT_NOFORK
} ForkType;

//阻塞的函数
typedef enum BlockFuncType
{
    BFT_SELECT,
    BFT_POLL,
    BFT_EPOLL
}BlockFuncType;

/*
储存参数的结构体，内容包括：
    unsigned long IPAddr;无符号整形形式的IP地址
    int Port;端口号
    BlockType Block_Type;是否阻塞
    ForkType Fork_Type;是否分裂子进程方法连接
    BlockFuncType BlockFunc_Type;阻塞的函数
    int Num;连接数
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

//设置默认参数的初始化函数
int resetOpt(opt *optio);

//设置参数结构体，前两项分别对应int argc和char* argv[]
int optSet(int optc,char *optv[],opt *option);

//打印结构体的内容测试函数
int printOpt(opt option);



