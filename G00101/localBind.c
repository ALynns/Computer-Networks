#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <time.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ifaddrs.h>
#include <stddef.h>
#include <sys/errno.h>

#include "runningopt.h"
#include "localBind.h"


int locanBind(opt *option)
{
    switch (option->Proc_Type)
    {
        case PT_SERVER:
        {
            struct sockaddr_in serviceAddr;
            int ret;
            memset(&serviceAddr, 0, sizeof(serviceAddr)); //���ڸýṹ���д��ڱ���λ����Ҫ��0
            serviceAddr.sin_family = AF_INET;             //�����ַ�����ͣ�AF_INETΪIPv4��AF_INET6ΪIPv6
            serviceAddr.sin_port = option->Port;
            serviceAddr.sin_addr.s_addr = option->IPAddr;                 //inet_addr("127.0.0.1");
            option->Socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Linux�ж���socketΪһ���ļ�������ֵΪһ���ļ�������
            if (option->Socketfd < 0)
            {
                printf("socket error\n");
                exit(-1);
            }
            if(option->Block_Type==BT_NONBLOCK)
                fcntl(option->Socketfd, F_SETFL, fcntl(option->Socketfd, F_GETFL, 0) | O_NONBLOCK);

            int reuse = 1;
            setsockopt(option->Socketfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(int));

            ret = bind(option->Socketfd, (struct sockaddr *)&serviceAddr, sizeof(serviceAddr)); //�󶨶˿�
            if (ret < 0)
            {
                printf("bind error\n");
                exit(-1);
            }
            ret = listen(option->Socketfd, SOMAXCONN);
            if (ret < 0) //listen()����ʹ�׽��ֽ��뱻������״̬��SOMAXCONNΪ�������������
            {
                printf("listen error\n");
                exit(-1);
            }
            break;
        }
        
        case PT_CLIENT:
        {
            break;
        }
    }
    return 0;
}