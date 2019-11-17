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

#include "selectserver.h"
#include "runningopt.h"
#include "localBind.h"

int main(int argc ,char *argv[])
{
    opt option;
    optSet(&option, argc, argv, PT_SERVER); //获取命令参数
    locanBind(&option);
    serverConnect(option);
    return 0;
}