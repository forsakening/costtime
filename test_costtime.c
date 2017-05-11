#include <stdio.h>
#include "costtime.h"

int test_func(int moduleID)
{
	int i,j,sum;
	for (i = 0; i < 500; i++)
		for (j = 0; j < 500; j++)
			sum += i*j;

	//printf("ID:%d, Sum:%d \n", moduleID, sum);
	return 0;
}

int main()
{
	int i,j;
	for (i = 0; i < 10; i++)
	{
		costtime_init(i, 10000, 1, costtime_default_callback);
	}

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 30000; j++)
		{
			costtime_tag_start(i);
			test_func(i); //待统计的代码块
			costtime_tag_end(i);
		}
	}

	return 0;
}
