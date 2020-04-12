#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
int main()
{
	//kill(进程id， 信号值)
	//kill(getpid(), SIGHUP);//给指定进程发送指定的信号
	//raise(SIGINT);//给进程自己发送指定的信号
	//abort();//给自己发送SIGABRT信号-通常用于异常通知
	alarm(3);//三秒钟之后给进程自己发送SIGALRM信号
	while(1)
	{
		printf("hello world\n");
		sleep(1);
	}
	return 0;
}
