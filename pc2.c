#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<ctype.h>

#define LEN 4
#define COUNT 8

int buffer[2][LEN];
int in[2] = {0, 0};
int out[2] = {0, 0};

sema_t mutex_buffer1, mutex_buffer2;
sema_t wait_empty_buffer1, wait_empty_buffer2;
sema_t wait_full_buffer1, wait_full_buffer2;


char get_item(int num)
{
	char item;

	item = buffer[num][out[num]];
	out[num] = (out[num] + 1) % LEN;
	return item;
}

void put_item(int num, char item)
{
	buffer[num][in[num]] = item;
	in[num] = (in[num] + 1) % LEN;
}

typedef struct{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;


void sema_init(sema_t *sema, int value)
{
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	while(sema->value <= 0)
		pthread_cond_wait(&sema->cond, &sema->mutex);
	sema->value--;
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

void *consume(void *arg)
{
	int i;
	int item;

	for(i = 0; i < COUNT; i++)
	{
		sema_wait(&wait_full_buffer2);
		sema_wait(&mutex_buffer2);

		item = get_item(1);

		printf("     consume item:%c\n",item);
		sema_signal(&mutex_buffer2);
		sema_signal(&wait_empty_buffer2);
	}
	return NULL;
}

void *calculate(void *arg)
{
	int i;
	char item;

	for(i = 0; i < COUNT; i++)
	{
		sema_wait(&wait_full_buffer1);
		sema_wait(&mutex_buffer1);
		item = get_item(0);
		sema_signal(&mutex_buffer1);
		sema_signal(&wait_empty_buffer1);

		sema_wait(&wait_empty_buffer2);
		sema_wait(&mutex_buffer2);
		put_item(1,toupper(item));
		printf("  calculate item:%C\n",toupper(item));
		sema_signal(&mutex_buffer2);
		sema_signal(&wait_full_buffer2);
	}
	return NULL;
}

void *produce(void *arg)
{
	int i;
	int item;

	for(i = 0; i < COUNT; i++)
	{
		sema_wait(&wait_empty_buffer1);
		sema_wait(&mutex_buffer1);

		item = 'a' + i;
		put_item(0, item);

		printf("produce item:%c\n", item);
		sema_signal(&mutex_buffer1);
		sema_signal(&wait_full_buffer1);
	}
	return NULL;
}

int main()
{
	pthread_t tid1, tid2;
		
	sema_init(&mutex_buffer1, 1);
	sema_init(&mutex_buffer2, 1);
	sema_init(&wait_empty_buffer1, 1);
	sema_init(&wait_empty_buffer2, 1);
	sema_init(&wait_full_buffer1, 0);
	sema_init(&wait_full_buffer2, 0);

	produce(NULL);
	pthread_create(&tid1, NULL, calculate, NULL);
	pthread_create(&tid2, NULL, consume, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}