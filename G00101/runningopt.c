#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "runningopt.h"


int optSet(opt *option,int optc,char *optv[],ProcType procType)
{
    option->Proc_Type = procType;
    
    optReset(option);

    int i;
    for (i = 1; i < optc; ++i)
    {
        if(!strcmp(optv[i],"--ip"))
        {
            if((option->IPAddr=inet_addr(optv[++i]))==INADDR_NONE)
            {
                if(option->Proc_Type==PT_SERVER)
                    option->IPAddr=inet_addr("0.0.0.0");
                --i;
            }    
            else
                continue;

            continue;
        }

        if(!strcmp(optv[i],"--port"))
        {
            if(strspn(optv[i+1],"1234567890")==strlen(optv[i+1]))
            {
                option->Port=htons(atoi(optv[++i]));
                continue;
            }
            else
                continue;
            
        }

        if(!strcmp(optv[i],"--num"))
        {
            if(strspn(optv[i+1],"1234567890")==strlen(optv[i+1]))
            {
                option->Num=atoi(optv[++i]);
                continue;
            }
            else
                continue;
            
        }

        if(!strcmp(optv[i],"--block"))
        {
            option->Block_Type=BT_BLOCK;
            continue;
        }

        if(!strcmp(optv[i],"--nonblock"))
        {
            option->Block_Type=BT_NONBLOCK;
            continue;
        }

        if(!strcmp(optv[i],"--fork"))
        {
            option->Fork_Type=FT_FORK;
            continue;
        }

        if(!strcmp(optv[i],"--nofork"))
        {
            option->Fork_Type=FT_NOFORK;
            continue;
        }

        if(!strcmp(optv[i],"--select"))
        {
            option->BlockFunc_Type=BFT_SELECT;
            continue;
        }

        if(!strcmp(optv[i],"--poll"))
        {
            option->BlockFunc_Type=BFT_POLL;
            continue;
        }

        if(!strcmp(optv[i],"--epoll"))
        {
            option->BlockFunc_Type=BFT_EPOLL;
            continue;
        }
    }
    if(option->Fork_Type==FT_NOFORK&&option->Block_Type==BT_BLOCK)
        option->Block_Type=BT_NONBLOCK;

    return 0;

}

int optReset(opt *option)
{
    option->IPAddr=inet_addr("0.0.0.0");
    option->Num=100;
    option->Block_Type=BT_NONBLOCK;
    option->Fork_Type=FT_NOFORK;
    option->BlockFunc_Type=BFT_SELECT;
    return 0;
}

int optSocketSet(opt *option, int socket)
{
    option->Socketfd = socket;
    return 0;
}

int printOpt(opt option)
{
    struct in_addr in;
    in.s_addr=option.IPAddr;

    printf("IP Addr:%s\n",inet_ntoa(in));
    printf("Port :%d\n",ntohs(option.Port));
    printf("Num:%d\n",option.Num);
    printf("BlockType:%d\n",option.Block_Type);
    printf("ForkType:%d\n",option.Fork_Type);
    printf("BlockFuncType:%d\n",option.BlockFunc_Type);
    return 0;
}
