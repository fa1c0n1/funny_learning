#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	off_t off;

	fd = open("test4.txt", O_WRONLY);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}

	printf("locating\n");
	off = lseek(fd, 5, SEEK_CUR);
	if(off == -1) {
		perror("fail to lseek");
		exit(1);
	}

	printf("writing\n");
	if(write(fd, "hello\n", 6) == -1) {
		perror("fail to write");
		exit(1);
	}	

	close(fd);

	return 0;
}