#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	umask(0);
	while(1)
	{
		printf("[sw@minishell]$ ");
		fflush(stdout);//刷新标准输出缓冲区i
		char buf[1024] = {0};
		fgets(buf, 1023, stdin);
		buf[strlen(buf) - 1] = '\0';
		char* ptr = buf;
		int flag = 0;
		char* file = NULL;
		while(*ptr != '\0')
		{
			if(*ptr == '>')
			{
				flag = 1;
				*ptr = '\0';
				ptr++;
				if(*ptr == '>')
				{
					flag = 2;
					ptr++;
				}
				while(*ptr == ' ' && *ptr != '\0')ptr++;
				file = ptr;
				while(*ptr != ' ' && *ptr != '\0')ptr++;
				*ptr = '\0';
			}
			ptr++;
		}	
		char* argv[32] = {NULL};
		int argc = 0;
		ptr = buf;
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
			if(flag == 1)
			{
				int fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0777);
				dup2(fd, 1);
			}
			else if(flag == 2)
			{
				int fd = open(file, O_WRONLY|O_CREAT|O_APPEND, 0777);
				dup2(fd, 1);
				
			}
			execvp(argv[0], argv);
			exit(0);
		}
		wait(NULL);
	}
	return 0;
}
