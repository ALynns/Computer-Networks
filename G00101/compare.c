#include "compare.h"

int compare_dirs(char *path1,char *path2)
{
        char str_cmd[100]="diff -r ";
        strcat(str_cmd,path1);
        strcat(str_cmd," ");
        strcat(str_cmd,path2);
    
        char buf[10]="";

        int ret=CMP_IDENTICAL;
        FILE *fp = popen(str_cmd,"r");
        if  (NULL != fgets(buf, 10, fp)) 
        {   
                ret=CMP_DIFFERENT;
        }   
        pclose(fp);
        return ret;
}
