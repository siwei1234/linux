#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int g_val = 1;
int main()
{
	pid_t pid = fork();
	if(pid == 0)
	{
		g_val = 100;
		printf("child %d, %p\n", g_val, &g_val);
	} 
	else if(pid > 0)
	{
		printf("parent %d, %p\n", g_val, &g_val);
	}
	else
	{
		printf("fork\n");
	}
	sleep(2);
	return 0;
}
