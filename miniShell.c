#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	while(1)
	{
		printf("[sw@minishell]$ ");
		fflush(stdout);//刷新标准输出缓冲区i
		char buf[1024] = {0};
		fgets(buf, 1023, stdin);
		buf[strlen(buf) - 1] = '\0';
		char* argv[32] = {NULL};
		int argc = 0;
		char* ptr = buf;
		while(*ptr != '\0')
		{
			if(*ptr != ' ')
			{
				argv[argc] = ptr;
				argc++;
				while(*ptr !=' ' && *ptr != '\0')
				{
					ptr++;
				}
				*ptr = '\0';
			}
			ptr++;
		}
		argv[argc] = NULL;
		pid_t pid = fork();
		if(pid == 0)
		{
			execvp(argv[0], argv);
			exit(0);
		}
		wait(NULL);
	}
	return 0;
}
