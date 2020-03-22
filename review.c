#include<stdio.h>
#include<string.h>
int main()
{
	FILE* fp = fopen("./test.txt", "r+");
	//fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	char* ptr = "hello siwei\n";
	int ret = fwrite(ptr, strlen(ptr), 1, fp);
	printf("%d\n",ret);
	fseek(fp, 0, SEEK_SET);
	char buf[1024] = {0};
	ret = fread(buf, 1, 1023, fp);
	printf("%d\n",ret);
	printf("%s\n", buf);
	fclose(fp);
	return 0;
}
