#include<cstdio>
/*
class temp
{
public:
	int* get_data()
	{
		return &_data;
	}
private:
	static int _data;
};
*/

class temp
{
public:
	int* get_data()
	{
		if(_data == NULL)
		{
			_data = new int;
			*_data = 10;
		}
		return _data;
	}
private:
	static int* _data;
};
int* temp::_data = NULL;

//int temp::_data = 1;

int main()
{
	temp a;
	temp b;
	printf("a:%d-%p\n", *(a.get_data()), a.get_data());
	printf("b:%d-%p\n", *(b.get_data()), b.get_data());	
	return 0;
}
