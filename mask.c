#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void sigcb(int signo)
{
	printf("signal no:%d\n", signo);
}
int main()
{
	signal(SIGINT, sigcb);
	signal(SIGRTMIN+4, sigcb);
	
	sigset_t set;
	sigemptyset(&set);//清空集合，防止位置数据造成影响
	sigfillset(&set);//向集合中添加所有信号
	sigprocmask(SIG_BLOCK, &set, NULL);//阻塞set集合中的所有信号

	printf("press enter continue");
	getchar();//等待一个回车，不按回车一直卡在这里	
	sigprocmask(SIG_UNBLOCK, &set, NULL);//接触set集合中的信号阻塞
	while(1)
	{
		sleep(1);
	}
	return 0;
}
