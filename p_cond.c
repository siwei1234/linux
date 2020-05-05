#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

int bowl = 0;
pthread_cond_t customer;
pthread_cond_t cook;
pthread_mutex_t mutex;


void* thr_cook(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(bowl != 0)
		{
			pthread_cond_wait(&cook, &mutex);
		}
		bowl = 1;
		printf("我做了一碗饭\n");
		pthread_cond_signal(&customer);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
void* thr_customer(void* arg)
{
	while(1)
	{
		while(bowl != 1)
		{
			pthread_cond_wait(&customer, &mutex);
		}
		bowl = 0;
		printf("我吃了一碗饭\n");
		pthread_cond_signal(&cook);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
int main()
{
	pthread_t cook_tid[4], customer_tid[4];
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cook, NULL);
	pthread_cond_init(&customer, NULL);
	int ret;
	int i;
	for(i = 0;i < 4;i++)
	{
		ret = pthread_create(&cook_tid[i], NULL, thr_cook, NULL);
		if(ret != 0)
			return -1;
		ret = pthread_create(&customer_tid[i], NULL, thr_customer, NULL);
		if(ret != 0)
			return -1;
	}
	pthread_join(cook_tid[0], NULL);
	pthread_join(customer_tid[0], NULL);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cook);
	pthread_cond_destroy(&customer);
	return 0;
}
