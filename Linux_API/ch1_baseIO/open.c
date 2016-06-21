#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;

	fd = open("test5.txt", O_CREAT|O_RDWR, 00700);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}else {
		printf("open OK\n");
	}

	close(fd);

	return 0;
}