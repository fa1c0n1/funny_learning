#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	int flag;

	char *p = "1st linux";
	char *q = "2nd linux";

	fd = open("test2.txt", O_WRONLY);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}

	if(write(fd, p, strlen(p)) == -1) {
		perror("fail to write");
		exit(1);
	}

	flag = fcntl(fd, F_GETFL, 0);
	printf("before: flag=%d\n", flag);
	if(flag == -1) {
		perror("fail to fcntl");
		exit(1);
	}

	flag |= O_APPEND;
	printf("O_APPEND=%d, after: flag=%d\n", O_APPEND, flag);
	if(fcntl(fd, F_SETFL, flag) == -1) {
		perror("fail to fcntl");
		exit(1);
	}

	if(write(fd, q, strlen(q)) == -1) {
		perror("fail to write");
		exit(1);
	}

	close(fd);

	return 0;
}
