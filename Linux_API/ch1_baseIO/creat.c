#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = creat("test2.txt", 0755);
	fchmod(fd, 0755);
	if(fd == -1) {
		perror("fail to creat");
		exit(1);
	}else {
		printf("creat OK\n");
	}

	if(close(fd) == -1) {
		perror("fail to close");
		exit(1);
	}else {
		printf("close OK\n");
	}

	return 0;
}