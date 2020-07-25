#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "coroutine.h"


int tcp_init() 
{
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( lfd == -1 ) perror("socket"),exit(1);

	int op = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9898);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int r = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	if ( r == -1 ) perror("bind"),exit(1);

	listen(lfd, SOMAXCONN);

	return lfd;
}

void set_nonblock(int fd) 
{
	int flgs = fcntl(fd, F_GETFL, 0);
	flgs |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flgs);
}

void accept_conn(int lfd, schedule_t *s, int co_ids[], void *(*call_back)(schedule_t *s, void *args) ) 
{
	while ( 1 ) 
	{
		int cfd = accept(lfd, NULL, NULL);
		if ( cfd > 0 ) 
		{
			set_nonblock(cfd);
			int args[] = {lfd, cfd};
			int id = coroutine_create(s, call_back, args);
			int i;
			for (i=0; i<CORSZ; i++) 
			{
				if ( co_ids[i] == -1 ) 
				{
					co_ids[i] = id;
					break;
				}
			}
			if ( i == CORSZ ) 
			{
				printf("连接太多\n");
			}
			coroutine_running(s, id);
		} 
		else 
		{
			int i;
			for (i=0; i<CORSZ; i++) 
			{
				int cid = co_ids[i];
				if ( cid == -1 ) continue;
				coroutine_resume(s, cid);
			}
		}
	}
}



void *handle(schedule_t *s, void *args) 
{
	int *arr = (int*)args;
	int cfd = arr[1];

	char buf[1024] = {};
	while ( 1 ) 
	{
		memset(buf, 0x00, sizeof(buf));
		int r = read(cfd, buf, 1024);
		if ( r == -1 ) 
		{
			coroutine_yield(s);
		} 
		else if ( r == 0 ) 
		{
			break;
		} 
		else 
		{
			printf("recv:%s\n", buf);
			if ( strncasecmp(buf, "exit", 4) == 0 ) 
			{
				break;
			}
			write(cfd, buf, r);
		}
	}
}


int main( void ) 
{
	int lfd = tcp_init();
	set_nonblock(lfd);

	schedule_t *s = schedule_create();
	int co_ids[CORSZ];
	int i;
	for (i=0; i<CORSZ; i++)
		co_ids[i] = -1;

	accept_conn(lfd, s, co_ids, handle);

	schedule_destroy(s);
}
