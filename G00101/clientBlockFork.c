#include "common.h"
#include "tools.h"
#include "savedialog.h"
#include "clientinfo.h"
#include "selectserver.h"
#include "runningopt.h"

//cmd like this
//./clientBlockFork  --ip 192.168.80.200 --port 4000 --block --fork --num 2 --select
#define div_piece 50

#define mysig_err 36
#define mysig_finish 37
static int goodend_for_dialog,badend_for_dialog;
void sig_handler_err(int signo)
{
   signal(mysig_err,sig_handler_err);
   //printf("client break down\n");
   badend_for_dialog++;
}

void sig_handler_finish(int signo)
{
   signal(mysig_err,sig_handler_err);
   //printf("client finish\n");
   goodend_for_dialog++;
}

int main(int argc,char *argv[])
{

   opt option;
   optSet(&option,argc,argv,PT_CLIENT);//获取命令参数


   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr=option.IPAddr;
   svraddr.sin_port=option.Port;

   int small_aim;
   while (option.Num)
   {
      if (option.Num>div_piece)
      {
         small_aim=div_piece;
         option.Num-=div_piece;
      }   
      else
      {
         small_aim=option.Num;
         option.Num=0;
      }
      
      //char buf[1024]={0};
      int nfork=0;
      int pid;
      int father_pid=getpid();
      goodend_for_dialog=0;
      badend_for_dialog=0;
      signal(mysig_err,sig_handler_err);
      signal(mysig_finish,sig_handler_finish);
      while (goodend_for_dialog<small_aim)
      {
         nfork-=badend_for_dialog;
         badend_for_dialog=0;
         while (nfork<small_aim) 
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
               //printf("connect to server successfully!\n");
               int stage=0;
               dialog dia;
               dia.StuNo=htonl(STUDENT_NO);
               dia.pid=htonl(getpid());
               get_datatime(dia.TIME);
               char recv_buf[MAX_SEND_LEN];
               while (stage<DIALOG_STAGE_NUM)
               {
                  memset(recv_buf,0,MAX_SEND_LEN);
                  if (recv(serverfd,recv_buf,MAX_SEND_LEN,0)<0)
                     error_exit("recv");;
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
                     srand(getpid()+time(0));
                     for (i=0;i<dia.Randstr_len;i++)
                        dia.Randstr[i]=rand()%256;
                     if (send(serverfd,dia.Randstr,dia.Randstr_len,0)<0)
                        error_exit("send");
                  }
                  else if (strcmp(recv_buf,"end")==0)
                  {
                     close(serverfd);
                     savedialog(&dia);
                     kill(father_pid,mysig_finish);
                  }
                  else
                  {
                     close(serverfd);
                     kill(father_pid,mysig_err);
                     //向父进程发送出错信号，
                     exit(0);
                  }
                  
                  stage++;
               }
               exit(0);
            }
         }
      }
   }
   printf("finish all connections\n");
   return 0;
}

