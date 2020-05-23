#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
	//创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
		return -1;
	//绑定地址信息（不推荐）
	//发送数据
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(9000);
	srv_addr.sin_addr.s_addr = inet_addr("192.168.122.1");
	socklen_t len = sizeof(struct sockaddr_in);
	char tmp[1024] = {0};
	fgets(tmp, 1024, stdin);
	sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr*)&srv_addr, len);
	//接收数据
	char buf[1024];
	recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&srv_addr, &len);
	printf("server:%s\n", buf);
	//关闭套接字
	close(sockfd);
	return 0;
}
