#include "common.h"
#include "savedialog.h"
#include "tools.h"
#include "runningopt.h"
#include "selectserver.h"
#include "serverbase.h"

/*cmd like this
 * ./serverBlockFork  --ip 192.168.80.200 --port 4000 --block --fork --select
 * */
int main(int argc,char *argv[])
{
   opt option;
   optSet(&option,argc,argv,PT_SERVER);//获取命令参数



   int listenfd=socket(AF_INET,SOCK_STREAM,0);
   if(listenfd<0)
   {
        error_exit("create error");
   }

   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr=option.IPAddr;
   svraddr.sin_port=option.Port;

   
   int reuse = 1;
   int ok=setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));//set port REUSEADDR


   int ret=bind(listenfd,(struct sockaddr*)&svraddr,sizeof(svraddr));

   if(ret<0)
   {
       error_exit("bind error");
   }

   
   ret=listen(listenfd,MAXCONNECTIONNUM);
   if(ret<0)
   {
      error_exit("listen error");
   }
   struct sockaddr_in removeaddr;
   int addr_len;
   int clientfd,nclient=0;

   signal(SIGCHLD,SIG_IGN);

   while (1)
   {
      addr_len=sizeof(removeaddr);
      clientfd=accept(listenfd,(struct sockaddr*)&removeaddr,&addr_len);
      ++nclient;
      /*每当有client连进来就fork一个新的进程来处理（没有上限）
 *       子进程出错了就直接退出
 *             */
      pid_t pid=fork();
      if (pid==0)
      {        
         printf("%d connected\n",nclient);       
         dialog dia;
         char toSend[DIALOG_STAGE_NUM][MAX_SEND_LEN];
         gen_send_info(toSend,&dia);
         
         int SendLen[DIALOG_STAGE_NUM]=
         {strlen(toSend[0]),strlen(toSend[1]),strlen(toSend[2])+1,strlen(toSend[3])+1,strlen(toSend[4])};

         int stage=0;
         while (stage<DIALOG_STAGE_NUM)
         {
            if (send(clientfd,toSend[stage],SendLen[stage],0)<0)
               error_exit("send");
            switch (stage)
            {
               case 0:
               {
                  if (recv(clientfd,&(dia.StuNo),sizeof(int),MSG_WAITALL)<0)
                     error_exit("recv");
                  break;
               }   
               case 1:
               {
                  if (recv(clientfd,&(dia.pid),sizeof(int),MSG_WAITALL)<0)
                     error_exit("recv");
                  break;
               }
               case 2:
               {
                  if (recv(clientfd,dia.TIME,TIME_LEN,MSG_WAITALL)<0)
                     error_exit("recv");
                  break;
               }
               case 3:
               {
                  if (recv(clientfd,dia.Randstr,dia.Randstr_len,MSG_WAITALL)<0)
                     error_exit("recv");
                  break;
               }
               case 4:
               {
                  
                     savedialog(&dia);
                  
                  close(clientfd);       
                  break;
               }
               default:
                  break;
            }
            ++stage;
         }
         exit(0);
      }
   }
   close(listenfd);
   return 0;
}


