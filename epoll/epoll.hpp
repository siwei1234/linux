#include<cstdio>
#include<unistd.h>
#include<vector>
#include<sys/epoll.h>
#include "tcpsocket.hpp"

class Epoll
{
public:
	Epoll()
	{
		_epfd = epoll_create(1);
		if(_epfd < 0)
		{
			perror("epoll_create error");
			_exit(0);
		}
	}
	bool Add(TcpSocket& sock)
	{
		int fd = sock.GetFd();
		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = EPOLLIN | EPOLLET;
		int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
		if(ret < 0)
		{
			perror("epoll_ctl_add error");
			return false;
		}
		return true;
	}
	bool Del(TcpSocket& sock)
	{
		int fd = sock.GetFd();
		int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
		if(ret < 0)
		{
			perror("epoll_ctl_del error");
			return false;
		}
		return true;
	}
	bool Wait(std::vector<TcpSocket>* list, int timeout = 3000)
	{
		struct epoll_event evs[10];
		int ret = epoll_wait(_epfd, evs, 10, timeout);
		if(ret < 0)
		{
			perror("epoll_wait error");
			return false;
		}
		else if(ret == 0)
		{
			printf("epoll timeout\n");
			return true;
		}
		for(int i = 0;i < ret;i++)
		{
			if(evs[i].events & EPOLLIN)
			{
				TcpSocket sock;
				sock.SetFd(evs[i].data.fd);
				list->push_back(sock);
			}
		}
		return true;
	}
private:
	int _epfd;
};
