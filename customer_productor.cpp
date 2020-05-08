#include<cstdio>
#include<iostream>
#include<vector>
#include<pthread.h>
#include<semaphore.h>

class RingQueue
{
public:
	RingQueue(int maxq = 5)
	:_queue(maxq)
	,_capacity(maxq)
	,_read(0)
	,_write(0)
	{
		sem_init(&_lock, 0, 1);
		sem_init(&_sem_data, 0, 0);
		sem_init(&_sem_idle, 0, maxq);
	}
	~RingQueue()
	{
		sem_destroy(&_lock);
		sem_destroy(&_sem_data);
		sem_destroy(&_sem_idle);
	}
	bool Push(int data)
	{
		sem_wait(&_sem_idle);
		sem_wait(&_lock);
		_queue[_write] = data;
		_write = (_write + 1) % _capacity;
		sem_post(&_lock);
		sem_post(&_sem_data);
		return true;
	}
	bool Pop(int* data)
	{
		sem_wait(&_sem_data);
		sem_wait(&_lock);
		*data = _queue[_read];
		_read = (_read + 1) % _capacity;
		sem_post(&_lock);
		sem_post(&_sem_idle);
		return true;
	}

private:
	std::vector<int> _queue;
	int _capacity;
	int _read;
	int _write;
	sem_t _lock;
	sem_t _sem_idle;
	sem_t _sem_data;
};
void* thr_cus(void* arg)
{
	RingQueue* queue = (RingQueue*)arg;
	int data;
	while(1)
	{
		queue->Pop(&data);
		printf("customer data:%d\n", data);
	}
	return NULL;
}
void* thr_pro(void* arg)
{
	RingQueue* queue = (RingQueue*)arg;
	int i = 0;
	while(1)
	{
		queue->Push(i);
		printf("product data:%d\n", i++);
	}
	return NULL;
}
int main()
{
	pthread_t cus[4], pro[4];
	int ret, i;
	RingQueue queue;
	for(i = 0;i < 4;i++)
	{
		ret = pthread_create(&cus[i], NULL, thr_cus, (void*)&queue);
		if(ret != 0)
			return -1;
		ret = pthread_create(&pro[i], NULL, thr_pro, (void*)&queue);
		if(ret != 0)
			return -1;
	}
	for(i = 0;i < 4;i++)
	{
		pthread_join(cus[i], NULL);
		pthread_join(pro[i], NULL);
	}
	return 0;
}
