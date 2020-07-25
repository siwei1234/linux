#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fcntl.h>

#include "coroutine.hpp"

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
void accept_conn(int lfd, schedule_t *s, void *(*call_back)(schedule_t *s, void *args) ) 
{
        while ( 1 ) 
	{
                int cfd = accept(lfd, NULL, NULL);
                if ( cfd > 0 ) 
		{
                        set_nonblock(cfd);
                        int args[] = {lfd, cfd};
			s->create(call_back, cfd);
                        s->running(cfd);
                } 
		else 
		{
			std::vector<int> fds = s->getfds();
			int i;
			for(i = 0;i < fds.size();i++)
			{
				s->resume(fds[i]);
			}
                }
        }
}



void *handle(schedule_t *s, void *args) 
{
	int cfd = s->getfd();
        char buf[1024] = {};
        while ( 1 ) 
	{
                memset(buf, 0x00, sizeof(buf));
                int r = read(cfd, buf, 1024);
                if ( r == -1 ) 
		{
                       	s->yield();
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

        schedule_t s;
        int co_ids[CORSZ];

        accept_conn(lfd, &s, handle);

}
                  
