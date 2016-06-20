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

	if(close(1) == -1) {
		perror("fail to close");
		exit(1);
	}

	if(dup(fd) == -1) {
		perror("fail to dup");
		exit(1);
	}else {
		printf("dup OK\n"); 
	}

	printf("hello world\n");

	return 0;
}