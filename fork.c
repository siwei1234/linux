#include<stdio.h>
#include<unistd.h>
int main()
{
	printf("---create child process start---%d\n", getpid());
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		printf("this is child %d---rpid:%d\n", getpid(), pid);
	}
	else if(pid > 0)
	{
		printf("this is parents %d---rpid:%d\n", getpid(), pid);
	}
	else
	{
		printf("fork error\n");
	}
	printf("---create child process end---%d--%d\n",getpid(), pid);
	return 0;
}
