#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9898);
	inet_aton("192.168.122.1", &addr.sin_addr);
	int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	if(ret == -1)
	{
		perror("connect error");
		exit(0);
	}
	char buf[1024] = {};
	while(fgets(buf, 1024, stdin) != NULL)
	{
		write(fd, buf, strlen(buf));
		memset(buf, 0x00, sizeof(buf));
		ret = read(fd, buf, 1024);
		if(ret <= 0) break;
		printf("say : %s\n", buf);
	}
	return 0;
}
