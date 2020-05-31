#include<iostream>
#include<signal.h>
#include<sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int no)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
int main(int argc, char* argv[])
{
	
	if(argc != 3)
	{
		std::cout << "ip port error" << std::endl;
		return -1;
	}
	signal(SIGCHLD, sigcb);
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
		int pid = fork();
		if(pid == 0)
		{
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
			exit(0);
		}
		new_sock.Close();
	}
	lst_sock.Close();
	return 0;
}
