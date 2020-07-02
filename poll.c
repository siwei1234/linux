#include<poll.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
	struct pollfd poll_fd[10];
	poll_fd[0].fd = 0;
	poll_fd[0].events = POLLIN;
	while(1)
	{
		int ret = poll(&poll_fd[0], 10, 3000);
		if(ret < 0)
		{
			perror("poll error");
			continue;
		}
		else if(ret == 0)
		{
			printf("poll timeout\n");
			continue;
		}
		int i;
		for(i = 0;i < 10;i++)
		{
			if(poll_fd[i].events == POLLIN)
			{
				char buf[1024];
				int ret = read(poll_fd[i].fd, buf, sizeof(buf)-1);
				printf("stdin:%s\n", buf);
			}
		}
	}
	return 0;
}
