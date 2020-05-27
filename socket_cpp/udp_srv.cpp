#include <iostream>
#include <string>
#include "udpsocket.hpp"

#define CHECK_RET(q) if((q)==false){return -1;}
int main (int argc, char *argv[])
{
	//argc 表示程序运行参数的个数
	//./udp_srv 192.168.2.2 9000
	if (argc != 3) 
	{
		std::cout << "Usage: ./udp_srv ip port\n";
		return -1;
	}
	uint16_t port = std::stoi(argv[2]);
	std::string ip = argv[1];

	UdpSocket srv_sock;
	//创建套接字
	CHECK_RET(srv_sock.Socket());
	//绑定地址信息
	CHECK_RET(srv_sock.Bind(ip, port));
	while(1) 
	{
		//接收数据
		std::string buf;
		std::string peer_ip;
		uint16_t peer_port;
		CHECK_RET(srv_sock.Recv(&buf, &peer_ip, &peer_port));//接收对端数据以及地址信息
		std::cout << "client[" << peer_ip << ":" << peer_port << "] say: " << buf << std::endl; 
		//发送数据
		buf.clear();
		std::cout << "server say: ";
		std::cin >> buf;
		CHECK_RET(srv_sock.Send(buf, peer_ip, peer_port));//谁发送了数据就给谁回复
	}
	//关闭套接字
	srv_sock.Close();
	return 0;
}

