#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define LEN 100000000		// 假设使用LEN个数计算PI

double array[LEN + 1];
double fir_output = 0;
double sec_output = 0;
double output = 0;

// 初始化数组
void array_init()
{
	int i;
	for(i = 1; i <= LEN; i++)
	{
		if(i % 2 == 1)
			array[i] = 1.0 / (2 * (i - 1) + 1);
		else
			array[i] = 1.0 / (-2 * (i - 1) - 1);
	}
}

// 子进程计算数组后一半的和
void *sub_thread()
{
	int i;
	for(i = LEN / 2 + 1; i <= LEN; i++)
		sec_output += array[i];
}

int main()
{
	pthread_t tid;
	int i;

	array_init();
	pthread_create(&tid, NULL, sub_thread, NULL);

	// 主进程计算数组前一半的和
	for( i = 1; i <= LEN / 2; i++)
		fir_output +=array[i];
	pthread_join(tid, NULL);	// 等待子进程
	output = fir_output +sec_output;
	printf("PI = %.10lf\n", output * 4);
	return 0;
}
