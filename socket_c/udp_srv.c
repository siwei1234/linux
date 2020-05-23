#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>//地址结构体定义的头文件
#include<arpa/inet.h>//字节序转换接口的头文件
#include<sys/socket.h>//套接字接口头文件

int main()
{
	uint16_t port = 9000;
	char* ip = "192.168.122.1";
	//创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
		return -1;
	//绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(struct sockaddr_in);
	int ret = bind(sockfd, (struct sockaddr*)&addr, len);
	if(ret < 0)
		return -1;
	//接收数据
	char tmp[4096] = {0};
	struct sockaddr_in cli_addr;
	char cli_ip[24] = {0};
	uint16_t cli_port = 0;
	ret = recvfrom(sockfd, tmp, 4096, 0, (struct sockaddr*)&cli_addr, &len);
	if(ret < 0)
		return -1;
	strcpy(cli_ip, inet_ntoa(cli_addr.sin_addr));
	cli_port = ntohs(cli_addr.sin_port);
	//发送数据
	ret = sendto(sockfd, tmp, ret, 0, (struct sockaddr*)&cli_addr, len);
	if(ret < 0)
		return -1;
	//关闭套接字
	close(sockfd);
	return 0;
}
