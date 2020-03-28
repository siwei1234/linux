#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	int fd = open("./test.txt", O_RDWR|O_CREAT, 0777);
	lseek(fd, 0, SEEK_END);
	char* ptr = "hello world\n";
	int ret = write(fd, ptr, strlen(ptr));
	printf("ret = %d\n", ret);
	lseek(fd, 0, SEEK_SET);
	char buf[1024] = {0};
	ret = read(fd, buf, 1024);
	printf("ret = %d\n%s\n", ret, buf);
	close(fd);
	return 0;
}
