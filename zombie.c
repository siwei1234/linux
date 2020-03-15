#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	pid_t pid = fork();
	if(pid == 0)
	{
		sleep(5);
		exit(66);
	}
	//wait(NULL);
	//waitpid(-1, NULL, 0);
	int ret, status;
	while((ret = waitpid(pid, &status, WNOHANG)) == 0)
	{
		printf("等待\n");
		sleep(1);
	}
	printf("status = %d\n",(status >> 8) & 0xff);
	while(1)
	{
		printf("忙碌\n");
		sleep(1);
	}
	return 0;
}
