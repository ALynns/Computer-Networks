#include "common.h"
#include "savedialog.h"
#include "tools.h"
#include "runningopt.h"
#include "selectserver.h"
#include "serverbase.h"
#include "mypoll.h"
//cmd like this
//./serverPoll  --ip 192.168.80.200 --port 4000 --nonblock --nofork --poll
int main(int argc, char *argv[])
{

	opt option;
	optSet(&option, argc, argv, PT_SERVER); //获取命令参数

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = option.IPAddr;
	server_addr.sin_port = option.Port;

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0)
		error_exit("create error");

	int reuse = 1;
	int ok = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	int flags = fcntl(listenfd, F_GETFL, 0);
	fcntl(listenfd, F_SETFL, flags | O_NONBLOCK);

	if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		error_exit("bind");

	if (listen(listenfd, MAXCONNECTIONNUM) < 0)
		error_exit("listen error");

	struct sockaddr_in client_addr;
	int addr_len = sizeof(client_addr);

	char buf[100] = {0};
	int clientfd[MAXCONNECTIONNUM], nclientfd = 0;
	int timeout = 0;
	int i, rdsize, nrd = 0;

	//listenfd->rd_poll[0] clientfd[0]->rd_poll[1] clientfd[1]->rd_poll[2]
	struct pollfd rd_poll[MAXCONNECTIONNUM];

	/*respond_list用来存rd_poll中到底哪一个响应了
	e.g
		respond_list[n]=0  -> listenfd respond
		respond_list[n]=9  -> clientfd[8] respond
	*/
	int respond_list[MAXCONNECTIONNUM];

	add_rdpoll(listenfd, rd_poll, &nrd); //添加描述

	int nstage[MAXCONNECTIONNUM];
	memset(nstage, 0, sizeof(int) * (MAXCONNECTIONNUM));
	dialog *ppdia[MAXCONNECTIONNUM];

	int ret;
	char temp_buff[10];
	int goodend_for_dialog = 0;

	while (1)
	{

		if (poll(rd_poll, nrd, timeout)) //有事件响应了
		{

			filter_respond_fd(rd_poll, nrd, respond_list);
			i = 0;

			if (respond_list[i] == 0) //listenfd 有响应了,添加新的clientfd
			{

				clientfd[nclientfd] = accept(listenfd, (struct sockaddr *)&client_addr, &addr_len);
				if (clientfd[nclientfd] <= 0)
					printf("accept error\n");
				else
				{
					//printf("a client connect in\n");
					flags = fcntl(clientfd[nclientfd], F_GETFL, 0);			 //获取文件的flags值。
					fcntl(clientfd[nclientfd], F_SETFL, flags | O_NONBLOCK); //设置成非阻塞模式；

				
					while (send(clientfd[nclientfd], "StuNo", 5, 0) < 0)
						;
					add_rdpoll(clientfd[nclientfd], rd_poll, &nrd);
					ppdia[nclientfd] = (dialog *)malloc(sizeof(dialog));

					nclientfd++;
					++i;
				}
			}

			while (respond_list[i++] != -1)
			{

				int fd_no = respond_list[i - 1] - 1;
				dialog *pdia = ppdia[fd_no];
				int pollerr = rd_poll[respond_list[i - 1]].revents;
				int fd = clientfd[fd_no];
				//printf("clientfd %d respond:in stage %d\n",fd,nstage[fd_no]);
				switch (nstage[fd_no])
				{
				case 0:
				{
					if (recv(fd, &(pdia->StuNo), sizeof(int), 0) < 0)
					{
						close(fd);
						free(pdia);
					}
					while (send(fd, "pid", 3, 0) < 0) //send next stage msg
						;
					break;
				}
				case 1:
				{
					if (recv(fd, &(pdia->pid), sizeof(int), 0) < 0)
					{
						close(fd);
						free(pdia);
					}
					while (send(fd, "TIME", 4, 0) < 0)
						;
					break;
				}
				case 2:
				{
					if (recv(fd, pdia->TIME, TIME_LEN, 0) < 0)
					{
						close(fd);
						free(pdia);
					}
					char s_rand[10], toSend[10] = "str";
					srand(pdia->pid);
					int i_rand = rangerand(32768, 99999);

					pdia->Randstr_len = i_rand;
					myitoa(i_rand, s_rand);
					strcat(toSend, s_rand);
					while (send(fd, toSend, 9, 0) < 0)
						;
					break;
				}
				case 3:
				{
					ret = 0;
					int temp;
					while (ret < pdia->Randstr_len)
					{
						while ((temp = recv(fd, pdia->Randstr + ret, pdia->Randstr_len - ret, 0)) < 0)
							;
						ret += temp;
						//printf("read %d bytes and require %d bytes\n",ret,pdia->Randstr_len);
					}
					while (send(fd, "end", 3, 0) < 0)
						;

					break;
				}
				case 4:
				{ 
					goodend_for_dialog++;
					printf("%dth dialog finished\n", goodend_for_dialog);
					savedialog(pdia);
					close(fd);
					free(pdia);
					break;
				}
				default:
					break;
				} //end of switch
				++nstage[fd_no];

			} //end of while (respond_list[i++]!=-1)
		}
	}
	return 0;
}
