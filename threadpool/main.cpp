#include "threadpool.hpp"
#include<unistd.h>
void func1(int data)
{
	int sec = (data % 3) + 1;
	printf("tid:%p--data:%d--sec:%d\n", pthread_self(), data, sec);
//	sleep(sec);
}

void func2(int data)
{
	printf("tid:%p--data:%d\n", pthread_self(), data);
//	sleep(1);
}
int main()
{
	ThreadPool p;
	for(int i = 0;i < 10;i++)
	{
		ThreadTask task;
		if(i % 2 == 0)
			task.SetTask(i, func1);
		else
			task.SetTask(i, func2);
		p.TaskPush(task);
	}
	sleep(10);
	return 0;
}
