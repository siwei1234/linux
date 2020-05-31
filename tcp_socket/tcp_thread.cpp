#include<iostream>
#include<signal.h>
#include<pthread.h>
#include "tcpsocket.hpp"


void* thr_start(void* arg)
{
	TcpSocket new_sock;
	long fd = (long)arg;
	new_sock.SetFd(fd);
	while(1)
	{
		std::string buf;
		new_sock.Recv(&buf);
		std::cout << "client say: " << buf << std::endl;
		buf.clear();
		std::cout << "server say: ";
		std::cin >> buf;
		new_sock.Send(buf);
	}
	new_sock.Close();
	return NULL;
}
void sigcb(int no)
{
        std::cout << "send no: " << no << std::endl;
}
int main(int argc, char* argv[])
{
	
	if(argc != 3)
	{
		std::cout << "ip port error" << std::endl;
		return -1;
	}
	signal(SIGPIPE, sigcb);
	std::string ip = argv[1];
	uint16_t port = std::stoi(argv[2]);
	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));
	CHECK_RET(lst_sock.Listen());
	while(1)
	{
		TcpSocket new_sock;
		bool ret = lst_sock.Accept(&new_sock);
		if(ret == false)
			continue;
		pthread_t tid;
		int res = pthread_create(&tid, NULL, thr_start, (void*)new_sock.GetFd());
		if(res != 0)
		{
			std::cout << "pthread create error" << std::endl;
			continue;
		}
		pthread_detach(tid);
	}
	lst_sock.Close();
	return 0;
}
