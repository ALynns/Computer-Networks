#include "common.h"
#include "tools.h"
#include "savedialog.h"
#include "clientinfo.h"
#include "selectserver.h"
#include "runningopt.h"
int main(int argc,char *argv[])
{

   opt option;
   optSet(&option,argc,argv,PT_CLIENT);//获取命令参数


   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr=option.IPAddr;
   svraddr.sin_port=option.Port;

   //char buf[1024]={0};
   int nfork=0;
   int pid;

   while (nfork<option.Num) 
   {

      pid=fork();     
      if (pid>0)
         ++nfork;
      else if (pid==0)
      {
         int serverfd=socket(AF_INET,SOCK_STREAM,0);
         if(serverfd<0)
         {
            error_exit("create error");
         }

         int ret =connect(serverfd,(struct sockaddr *)&svraddr,sizeof(svraddr));
         if(ret<0)
         {
            error_exit("connect error");
         }
         printf("connect to server successfully!\n");
         int stage=0;
         dialog dia;
         dia.StuNo=htonl(STUDENT_NO);
         dia.pid=htonl(getpid());
         get_datatime(dia.TIME);
         char recv_buf[MAX_SEND_LEN];
         while (stage<DIALOG_STAGE_NUM)
         {
            memset(recv_buf,0,MAX_SEND_LEN);
            recv(serverfd,recv_buf,MAX_SEND_LEN,0);
            if (strcmp(recv_buf,"StuNo")==0)
            {
               if (send(serverfd,&(dia.StuNo),sizeof(int),0)<0)
                     error_exit("send");
            }
            else if (strcmp(recv_buf,"pid")==0)
            {
               if (send(serverfd,&(dia.pid),sizeof(int),0)<0)
                     error_exit("send");
            }
            else if (strcmp(recv_buf,"TIME")==0)
            {
               if (send(serverfd,dia.TIME,TIME_LEN,0)<0)
                     error_exit("send");
            }
            else if (strncmp(recv_buf,"str",3)==0)
            {
               dia.Randstr_len=atoi(recv_buf+3);
               int i;
               for (i=0;i<dia.Randstr_len;i++)
                  dia.Randstr[i]=rand()%256;
               if (send(serverfd,dia.Randstr,dia.Randstr_len,0)<0)
                  error_exit("send");
            }
            else if (strcmp(recv_buf,"end")==0)
            {
               close(serverfd);
               savedialog(&dia);
            }
            stage++;
         }
         exit(0);
      }
   }
   getchar();
   return 0;
}

