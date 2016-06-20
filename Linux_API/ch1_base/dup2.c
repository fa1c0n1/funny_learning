#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = open("test2.txt", O_RDWR);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}else {
		printf("open OK\n");
	}

	if(dup2(fd, 1) == -1) {
		perror("fail to dup2");
		exit(1);
	}else {
		printf("dup2 STDOUT OK\n"); 
	}

	printf("hello world\n");

	return 0;
}