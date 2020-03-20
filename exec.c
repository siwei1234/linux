//演示程序替换接口的基本使用与认识
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char* argv[], char* env[])
{
	printf("hehe\n");
	//execl("./test", "test", "-l", NULL);
	char* new_argv[] = {"test", "-L", "-P", NULL};
	//execvp("test", new_argv);
	char* new_env[] = {"MYVAL=10","TESTVAL=20",NULL};
	execve("./test", new_argv, new_env);
	return 0;
}
