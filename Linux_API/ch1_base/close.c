#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = open("test.txt", O_RDWR);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}else {
		printf("open OK\n");
	}

	if(close(fd) == -1) {
		perror("fail to close");
		exit(1);
	}else {
		printf("close OK\n");
	}

	return 0;
}