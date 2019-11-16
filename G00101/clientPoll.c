#include "common.h"
#include "tools.h"
#include "savedialog.h"
#include "clientinfo.h"
#include "selectserver.h"
#include "runningopt.h"
#include "mypoll.h"

//cmd like this
//./clientPoll  --ip 192.168.80.200 --port 4000 --nonblock --nofork --num 2 --poll

#define div_piece 10
int main(int argc, char *argv[])
{
	opt option;
   	optSet(&option,argc,argv,PT_CLIENT);//获取命令参数

    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=option.IPAddr;
    server_addr.sin_port=option.Port;
	
	int small_aim;
	int asc=0;
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
		int i,timeout=0;
		int serverfd[MAXCONNECTIONNUM];
		int nrd=0;
		struct pollfd rd_poll[MAXCONNECTIONNUM];
		int respond_list[MAXCONNECTIONNUM];
		
		int nstage[MAXCONNECTIONNUM];
		
		dialog* ppdia[MAXCONNECTIONNUM];
		int ret;
		memset(nstage,0,sizeof(int)*MAXCONNECTIONNUM);
		for (i=0;i<small_aim;i++)
		{
			serverfd[i] = socket(AF_INET, SOCK_STREAM, 0);
			if (serverfd[i]<0)
				error_exit("create error");
			
			int flags = fcntl(serverfd[i], F_GETFL, 0);  //获取文件的flags值
			fcntl(serverfd[i], F_SETFL, flags | O_NONBLOCK);//设置成非阻塞模式；

			ret = connect(serverfd[i], (struct sockaddr *)&server_addr, sizeof(server_addr));
			if (ret<0)
			{
				if (errno=!EINPROGRESS)
					error_exit("connect error");

				//if (connect(serverfd[i], (struct sockaddr *)&server_addr, sizeof(server_addr))==0||errno==EALREADY)
					//printf("%d connect to server successfully!\n",i);
			}

			ppdia[nrd]=(dialog*)malloc(sizeof(dialog));
			ppdia[nrd]->StuNo=htonl(STUDENT_NO);
			ppdia[nrd]->pid=htonl(getpid()+(asc++));
			if (ppdia[nrd]->pid==0)
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%d,%d\n",getpid(),serverfd[i]);
			get_datatime(ppdia[nrd]->TIME);
			add_rdpoll(serverfd[i],rd_poll,&nrd);
			
		}


		char recv_buf[MAX_SEND_LEN];
		int goodend_for_dialog=0;
		while (goodend_for_dialog<small_aim)
		{
			if (poll(rd_poll,nrd,timeout))//有事件响应了
			{
				
				filter_respond_fd(rd_poll,nrd,respond_list);
				i=0;
				while (respond_list[i++]!=-1)
				{
					
					int fd_no=respond_list[i-1];
					int fd=serverfd[fd_no];
					dialog *pdia=ppdia[fd_no];
					
					memset(recv_buf,0,MAX_SEND_LEN);
					recv(fd,recv_buf,MAX_SEND_LEN,0);
					switch (nstage[fd_no])
					{
						case 0:
						{
							if (strcmp(recv_buf,"StuNo")==0)
							{
								
								if (send(fd,&(pdia->StuNo),sizeof(int),0)<0)
									error_exit("send");
							}
							
							break;
						}   
						case 1:
						{
							if (strcmp(recv_buf,"pid")==0)
							{
								if (send(fd,&(pdia->pid),sizeof(int),0)<0)
									error_exit("send");
							}
							
							break;
						}
						case 2:
						{
							if (strcmp(recv_buf,"TIME")==0)
							{
								if (send(fd,pdia->TIME,TIME_LEN,0)<0)
									error_exit("send");
							}
							
							break;
						}
						case 3:
						{
							if (strncmp(recv_buf,"str",3)==0)
							{
								pdia->Randstr_len=atoi(recv_buf+3);
								int i;
								for (i=0;i<pdia->Randstr_len;i++)
									pdia->Randstr[i]=rand()%256;
								
								int temp;
								ret=0;
								while (ret<pdia->Randstr_len)
								{
									while ((temp=send(fd,pdia->Randstr+ret,pdia->Randstr_len-ret,0))<0)
									{
										//printf("send block\n");
										//sleep(1);
									}	
									if (temp>0)
										ret+=temp;
								
									//printf("send len %d for reqest %d for %d\n",ret,pdia->Randstr_len,pdia->pid);
								}
							}
							
							break;
							
						}
						case 4:
						{
							if (strcmp(recv_buf,"end")==0)
							{
								close(fd);
								savedialog(pdia);
								free(pdia); 
								goodend_for_dialog++;
							}
							
							break;
						}
						default:
							break;
					}//end of switch
					++nstage[fd_no];
				}
			}
		}
	}
	/*
	char buf[100] ="1751763";
    
    int timeout=0;
	struct pollfd wr_poll;
	int npoll=0;
	add_wrpoll(serverfd,&wr_poll,&npoll);
	int tmp;
    while (1) 
    {
	
        if ((tmp=poll(&wr_poll,npoll,timeout))>0)
        {
            int wrsize = write(serverfd, buf, 100);
            printf("write size %d\n", wrsize);
        }
                   
    }
	*/
}
