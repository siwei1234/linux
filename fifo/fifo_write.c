//mkfifo创建命名管道
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>

int main()
{
	umask(0);
	int ret = mkfifo("./test.fifo", 0664);
	if(ret < 0 && errno != EEXIST) 
	{
		return -1;
	}
	int fd = open("./test.fifo", O_WRONLY);
	int i = 0;
	while(1)
	{
		char buf[1024] = {-1};
		sprintf(buf, "hello world+%d", i++);
		write(fd, buf, strlen(buf));
		sleep(1);
	}
	close(fd);
	return 0;
}

