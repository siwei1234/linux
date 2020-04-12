#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>


void sigcb(int signo)
{
	printf("signal no:%d\n", signo);
}
int main()
{
	//signal(SIGINT, SIG_IGN);//当前信号到来的时候，采用忽略处理方式
	signal(SIGINT, sigcb);//当前信号到来的时候调用sigcb这个函数，并且通过参数传入触发回调函数的信号值
	while(1)
	{
		printf("hello world\n");
		sleep(1);
	}
	return 0;
}
