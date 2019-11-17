#include"savedialog.h"


int savedialog(dialog *dia)
{
    char path[30]="";
    sprintf(path,"./txt/%d.%d.pid.txt",ntohl(dia->StuNo),ntohl(dia->pid));
    printf("save dialog to %s\n",path);
    FILE * fp=fopen(path,"w+");
    if (NULL==fp)
        perror("fopen");  
    fprintf(fp,"%d\n%d\n",dia->StuNo,dia->pid);
    fwrite(dia->TIME,sizeof(char),19,fp);
    fwrite("\n",sizeof(char),1,fp);
    fwrite(dia->Randstr,sizeof(char),dia->Randstr_len,fp);
    fclose(fp);
}