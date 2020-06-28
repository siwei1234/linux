#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/select.h>


int main(int argc, char* argv[])
{
	while(1)
	{
		fd_set set;
		FD_ZERO(&set);//初始化情况集合
		FD_SET(0, &set);//将标准输入描述符加入集合
		struct timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		printf("Start monitoring\n");
		int ret = select(1, &set, NULL, NULL, &tv);
		if(ret < 0)
		{
			perror("select error");
			return -1;
		}
		else if(ret == 0)
		{
			printf("monitoring timeout\n");
			continue;
		}
		if(FD_ISSET(0, &set))
		{
			char buf[1024];
			printf("Start reading data\n");
			ret = read(0, buf, 1023);
			if(ret < 0)
			{
				perror("read error");
				return -1;
			}
			printf("buf:%s\n", buf);
		}
	}
	return 0;
}
