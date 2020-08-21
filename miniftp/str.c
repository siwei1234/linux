#include"str.h"

void str_trim_crlf(char* str)
{
	char* p = str + strlen(str) - 1;
	while('\r' == *p || '\n' == *p)
		*p-- = '\0';
}
void str_split(const char* str, char* left, char* right, char c)
{
	char* p = strchr(str, c);
	if(NULL == p)
		strcpy(left, str);
	else
	{
		strncpy(left, str, p - str);
		strcpy(right, p + 1);
	}
}

void str_upper(char* str)
{
	while(*str)
	{
		if(*str >= 'a' && *str <= 'z')
			*str -= 32;
		str++;
	}
}