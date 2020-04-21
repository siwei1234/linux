#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
void* func(void* arg)
{
	while(1)
	{
		printf("%s\n", (char*)arg);
		sleep(1);
	}
	return NULL;
}



int main()
{
	pthread_t tid;
	char* ptr = "hello";
	int ret = pthread_create(&tid, NULL, func, (void*)ptr);
	if(ret != 0)
		return -1;
	while(1)
	{
		printf("main pthread\n");
		sleep(1);
	}
	return 0;
}
