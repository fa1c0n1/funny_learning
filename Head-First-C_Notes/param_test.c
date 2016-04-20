#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* do_stuff(void *param)
{
	long thread_no = (long)param;
	printf("Thread number %ld\n", thread_no);
	return (void*)(thread_no + 1);
}

int main(void)
{
	pthread_t threads[3];
	long t;
	for(t = 0; t < 3; t++) {
		pthread_create(&threads[t], NULL, do_stuff, (void *)t);
	}

	void *result;
	for(t = 0; t < 3; t++) {
		pthread_join(threads[t], &result);
		printf("Thread %ld return %ld\n", t, (long)result);
	}

	return 0;
}