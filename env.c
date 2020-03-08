//在代码中访问环境变量
#include<stdio.h>
#include<stdlib.h>
int main(int argc, char* argv[], char* env[])
{
	int i = 0;
	for(;env[i];i++)
	{
		printf("%s\n", env[i]);
	}
	printf("%s\n", getenv("PATH"));
	return 0;
}
