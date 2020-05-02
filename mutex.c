#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int ticket = 100;
pthread_mutex_t mutex;
void* func(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(ticket > 0)
		{
			usleep(1000);
			printf("我抢到票了--%d\n", ticket);
			--ticket;
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
	return NULL;
}

int main()
{
	pthread_t tid[4];
	int i, ret;
	//互斥锁的初始化一定要放在线程创建之前
	pthread_mutex_init(&mutex, NULL);
	for(i = 0;i < 4;i++)
	{
		ret = pthread_create(&tid[i], NULL, func, NULL);
		if(ret != 0)
			return -1;
	}
	for(i = 0;i < 4;i++)
	{
		pthread_join(tid[i], NULL);
	}
	//互斥锁的销毁一定是不再使用这个互斥锁
	pthread_mutex_destroy(&mutex);
	return 0;
}
