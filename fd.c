#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
	//close(1);
	/*umask(0);
	int fd = open("./test.txt", O_RDWR|O_CREAT, 0664);
	dup2(fd, 1);
	printf("fd = %d\n", fd);
	fflush(stdout);
	close(fd);
	*/
	FILE* fp = fopen("./test.txt", "r+");
	fp->_fileno = 1;
	fwrite("hello world\n", 1, 12, fp);
	fclose(fp);
	return 0;
}
