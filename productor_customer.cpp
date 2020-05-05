#include<cstdio>
#include<iostream>
#include<queue>
#include<pthread.h>

class BlockQueue
{
	public:
		BlockQueue(int maxq = 5)
			:_capacity(maxq)
		{
			pthread_mutex_init(&_mutex, NULL);
			pthread_cond_init(&_pro_cond, NULL);
			pthread_cond_init(&_cus_cond, NULL);
		}
		~BlockQueue()
		{
			pthread_mutex_destroy(&_mutex);
			pthread_cond_destroy(&_pro_cond);
			pthread_cond_destroy(&_cus_cond);
		}
		bool Push(const int data)
		{
			pthread_mutex_lock(&_mutex);
			while(_queue.size() == _capacity)
			{
				pthread_cond_wait(&_pro_cond, &_mutex);
			}
			_queue.push(data);
			pthread_mutex_unlock(&_mutex);
			pthread_cond_signal(&_cus_cond);
			return true;
		}
		bool Pop(int* data)
		{
			pthread_mutex_lock(&_mutex);
			while(_queue.empty())
			{
				pthread_cond_wait(&_cus_cond, &_mutex);
			}
			*data = _queue.front();
			_queue.pop();
			pthread_mutex_unlock(&_mutex);
			pthread_cond_signal(&_pro_cond);
			return true;
		}
	private:
		std::queue<int> _queue;
		int _capacity;
		pthread_mutex_t _mutex;
		pthread_cond_t _pro_cond;
		pthread_cond_t _cus_cond;
};

void* thr_pro(void* arg)
{
	BlockQueue* queue = (BlockQueue*)arg;
	int i = 0;
	while(1)
	{
		queue->Push(i);
		printf("product data:%d\n", i++);
	}
	return NULL;
}
void* thr_cus(void* arg)
{
	BlockQueue* queue = (BlockQueue*)arg;
	int data;
	while(1)
	{
		queue->Pop(&data);
		printf("customer data:%d\n", data);
	}
	return NULL;
}

int main()
{
	pthread_t pro[4], cus[4];
	int ret, i;
	BlockQueue queue;
	for(i = 0;i < 4;i++)
	{
		ret = pthread_create(&pro[i], NULL, thr_pro, (void*)&queue);
		if(ret != 0)
			return -1;
		ret = pthread_create(&cus[i], NULL, thr_cus, (void*)&queue);
		if(ret != 0)
			return -1;
	}
	for(i = 0;i < 4;i++)
	{
		pthread_join(pro[i], NULL);
		pthread_join(cus[i], NULL);
	}
	return 0;
}
