#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int pipefd[2] = {-1};
	int ret = pipe(pipefd);
	pid_t pid = fork();
	if(pid == 0)
	{
		char buf[1024] = {0};
		read(pipefd[0], buf, 1023);
		printf("%s", buf);
	}
	else
	{
		char* ptr = "hello world\n";
		write(pipefd[1], ptr, strlen(ptr));
	}
	return 0;
}

