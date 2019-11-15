#include "common.h"
#include "mypoll.h"
#include "savedialog.h"
#include "tools.h"
#define MAX_CLIENT_NUM 1024



int main()
{
    char t[100]="abcv";
    if (strncmp(t,"abc",3)==0)
        printf("%s",t);
    return 0;
}
