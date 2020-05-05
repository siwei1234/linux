#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void* function()
{
	char* ptr = "返回值";
	pthread_exit((void*)ptr);
}

void* func(void* arg)
{
	pthread_detach(pthread_self());
	while(1)
	{
		printf("thread\n");
		sleep(5);
		function();
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, func, NULL);
	if(ret != 0)
		return -1;
	//pthread_cancel(tid);
	//char* ptr;
	//pthread_join(tid, (void**)&ptr);
	//printf("%s\n", ptr);
	while(1)
	{

		printf("main\n");
		sleep(1);
		//pthread_exit(NULL);
	}
	return 0;
}
