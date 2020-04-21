#include<stdio.h>
#include<unistd.h>
#include<pthread.h>



void* func(void* arg)
{
	while(1)
	{
		printf("thread\n");
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, func, NULL);
	if(ret != 0)
		return -1;
	pthread_cancel(tid);
	while(1)
	{

		printf("main\n");
		sleep(1);
		//pthread_exit(NULL);
	}
	return 0;
}
