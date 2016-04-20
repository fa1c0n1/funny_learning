#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

void* does_not(void *a)
{
	int i = 0;
	for(; i < 5; i++) {
		sleep(1);
		puts("Does not!");
	}
}

void* does_too(void *a)
{
	int i = 0;
	for(; i < 5; i++) {
		sleep(1);
		puts("Does too!");
	}
	return NULL;
}

int main(void)
{
	pthread_t t0;
	pthread_t t1;

	if(pthread_create(&t0, NULL, does_not, NULL) == -1) {
		error("无法创建线程t0");
	}
	if(pthread_create(&t1, NULL, does_too, NULL) == -1) {
		error("无法创建线程t1");
	}

	void *result;
	if(pthread_join(t0, &result)) {
		error("无法回收线程t0");
	}
	if(pthread_join(t1, &result)) {
		error("无法收回线程t1");
	}

	return 0;
}