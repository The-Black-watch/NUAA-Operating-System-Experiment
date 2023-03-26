#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define LEN 100000000		// 假设使用LEN个数计算PI

double array[LEN + 1];
double output = 0;

struct param{
	int start;
	int end;
};

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

void *sub_thread(void *arg)
{
	int i;
	struct param *param;
	param = (struct param*)arg;
	for(i = param->start; i <= param->end; i++)
		output += array[i];
}

int main()
{
	pthread_t tid[1000];
	int i, n, step;

	array_init();
	printf("input threads number:");
	scanf("%d", &n);
	step = LEN / n;	// 每个进程中处理的数组步长 

	for(i = 1; i <= n; i++)
	{
		struct param *param;
		param = (struct param*)malloc(sizeof(struct param));
		if(i != n)
		{
			param->start = (i - 1) * step +1;
			param->end = i * step;
		}
		else
		{
			param->start = (i - 1) * step + 1;
			param->end = LEN;
		}
		pthread_create(&tid[i], NULL, sub_thread, param);	// 创建n个子进程 
	}

	for( i = 1; i <= n; i++)
		pthread_join(tid[i], NULL);		// 等待每个子进程 

	printf("PI = %.10lf\n", output * 4);
	return 0;
}