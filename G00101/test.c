#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "runningopt.h"

int main(int argc,char *argv[])
{
    opt option;
    optSet(argc,argv,&option);
    printOpt(option);
    return 0;
}