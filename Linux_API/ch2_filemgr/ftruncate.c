#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
	int fd;

	fd = open("test3.txt", O_RDWR|O_APPEND);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}
	printf("open OK\n");

	if(ftruncate(fd, 32) == -1) {
		perror("fail to ftruncate");
		exit(1);
	}
	printf("ftruncate OK\n");


	close(fd);

	return 0;	
}