//这是一个进度条程序
#include<stdio.h>
#include<unistd.h>
int main()
{
	char arr[11] = {0};
	int i;
	for(i = 0;i < 11;i++)
	{
		arr[i] = '-';
		printf("\r%s",arr);
		fflush(stdout);
		usleep(100000);
	}
	return 0;
}
