#include<iostream>
#include "epoll.hpp"
int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		std::cout << "ip port error" << std::endl;
		return -1;
	}
	std::string ip = argv[1];
	uint16_t port = std::stoi(argv[2]);
	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));
	CHECK_RET(lst_sock.Listen());
	Epoll s;
	s.Add(lst_sock);
	while(1)
	{
		std::vector<TcpSocket> list;
		bool ret = s.Wait(&list);
		if(ret == false)
		{
			continue;
		}
		for(auto sock : list)
		{
			if(sock.GetFd() == lst_sock.GetFd())
			{
				TcpSocket new_sock;
				bool ret = lst_sock.Accept(&new_sock);
				if(ret == false)
					continue;
				s.Add(new_sock);
			}
			else
			{
				std::string buf;
				ret = sock.Recv(&buf);
				if(ret == false)
				{
					sock.Close();
					s.Del(sock);
					continue;
				}
				std::cout << "client say:" << buf << std::endl;
				std::cout << "server say:";
				buf.clear();
				std::cin >> buf;
				ret = sock.Send(buf);
				if(ret == false)
				{
					sock.Close();
					s.Del(sock);
				}
			}
		}
	}
	lst_sock.Close();
	return 0;
}
