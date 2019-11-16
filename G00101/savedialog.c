#include"savedialog.h"
#include"tools.h"

int savedialog(dialog *dia)
{
    char path[30]="";
    char str_buf[20];
    int hStuNo=ntohl(dia->StuNo),hpid=ntohl(dia->pid);

    sprintf(path,"./txt/%d.%d.pid.txt",hStuNo,hpid);
    printf("save dialog to %s\n",path);
    FILE * fp=fopen(path,"w+");
    if (NULL==fp)
        perror("fopen");  
    fprintf(fp,"%s\n",myitoa(hStuNo,str_buf));  
    fprintf(fp,"%s\n",myitoa(hpid,str_buf));
    fwrite(dia->TIME,sizeof(char),TIME_LEN,fp);
    fwrite("\n",sizeof(char),1,fp);
    fwrite(dia->Randstr,sizeof(char),dia->Randstr_len,fp);
    fclose(fp);
}