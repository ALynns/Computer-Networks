#ifndef __MYPOLL_H
#define __MYPOLL_H

#include <poll.h>
#endif




/*添加监听可读的文件描述符
入口参数 fd：要添加监听的文件描述符 rd_poll[]：用于监听可读的pollfd数组 npoll:ptr->现在监听的文件描述符数量
出口参数 npoll:把fd添加进rd_poll后npoll++
*/

int add_rdpoll(int fd,struct pollfd rd_poll[],int *npoll);

/*添加监听可写的文件描述符
入口参数 fd：要添加监听的文件描述符 wr_poll[]：用于监听可写的pollfd数组 npoll:ptr->现在监听的文件描述符数量
出口参数 npoll:把fd添加进wr_poll后npoll++
*/

int add_wrpoll(int fd,struct pollfd wr_poll[],int *npoll);

/*
进行监听

入口参数 fds:要监听的文件描述符形成的pollfd数组（不是文件描述符数组），nfds ：要监听的文件描述符数量  
timeout：赋值为负数是阻塞，赋值为0时立即返回，赋值为正数时表示等待的时间

返回值：正数时表示多少个fd相应了事件；0时表示超时；-1表示出错了

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

*/


/*
如果poll返回值大于0
应该要从poll数组中找到是哪个文件描述符响应了可读，可写事件
入口参数 arr_poll：要监听的文件描述符形成的pollfd数组（不是文件描述符数组），npoll:要监听的文件描述符数量 
respond_list:用于传出响应的文件描述符下标（规定以-1结束）。
*/
int filter_respond_fd(struct pollfd arr_poll[],int npoll,int *respond_list);


