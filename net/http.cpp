#include<iostream>
#include<sstream>
#include "tcpsocket.hpp"


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
	while(1)
	{
		TcpSocket new_sock;
		bool ret = lst_sock.Accept(&new_sock);
		if(ret == false)
			continue;
		std::string req;
		if(new_sock.Recv(&req) == false)
		{
			new_sock.Close();
			continue;
		}
		std::cout << "req:" << req << std::endl;
		std::string body = "<html><body><h1>Hello world</h1></body></html>";
		std::stringstream header;
		header << "Content-Length: " << body.size() << "\r\n";
		header << "Connection: close\r\n";
		header << "Location: http://www.baidu.com\r\n";
		std::string blank = "\r\n";
		std::string first_line = "HTTP/1.1 302 Found\r\n";
		new_sock.Send(first_line);
		new_sock.Send(header.str());
		new_sock.Send(blank);
		new_sock.Send(body);
		new_sock.Close();
	}
	lst_sock.Close();
	return 0;
}
