#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define LEN 10

int array[LEN + 1] = {100, 70, 3, 20, 80, 60, 41, 90, 20, 16};

// 子进程对后半部分进行选择排序 
void *sub_thread()
{
	int i, j, temp;
	for(i = LEN / 2; i < LEN; i++)
	{
		for(j = i; j < LEN; j++)
		{
			if(array[j] < array[i])
			{
				temp = array[j];
				array[j] = array[i];
				array[i] = temp;
			}
		}
	}
}

// 对前后部分进行归并排序 
void merge()
{
	int array1[LEN + 1];
	int i, j, s, t;
	s = 0;
	i = 0;
	j = LEN / 2;
	while(i < (LEN / 2)&&j < LEN)
	{
		if(array[i] < array[j])
			array1[s++] = array[i++];
		else
			array1[s++] = array[j++];
	}
	if(i == (LEN / 2))
	{
		for(t = j; t < LEN; t++)
			array1[s++] = array[t++];
	}
	if(j == LEN)
	{
		for(t = i; t < (LEN / 2); t++)
			array1[s++] = array[t++];
	}
	for(i = 0; i < LEN; i++)
		array[i] = array1[i];
}

void show(int start, int end)
{
	int i;
	for(i = start; i < end; i++)
		printf("%d ",array[i]);
	printf("\n");
}

int main()
{
	pthread_t tid;
	int i, j, temp;

	printf("before sort:");
	show(0, LEN);

	pthread_create(&tid, NULL, sub_thread, NULL);

	// 主进程对前半部分进行选择排序 
	for( i = 0; i < (LEN / 2); i++)
	{
		for(j = i; j < (LEN / 2); j++)
		{
			if(array[j] < array[i])
			{
				temp = array[j];
				array[j] = array[i];
				array[i] = temp;
			}
		}
	}
	pthread_join(tid, NULL);

	printf("main_thread sort:");
	show(0, LEN / 2);
	printf("sub_thread sort:");
	show(LEN / 2, LEN);
	merge();

	printf("after sort:");
	show(0, LEN);

	return 0;
}