#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	char buf[1024];
	ssize_t n;

	fd = open("test.txt", O_RDONLY);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	} else {
		printf("open OK\n");
	}

	printf("reading\n");
	n = read(fd, buf, 1024);
	if(n == -1) {
		perror("fail to read");
		exit(1);
	}

	buf[n] = '\0';
	printf(buf);
	close(fd);

	return 0;
}