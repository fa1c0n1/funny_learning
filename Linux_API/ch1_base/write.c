#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	ssize_t n;

	fd = open("test8.txt", O_CREAT|O_WRONLY, 00700);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}

	printf("writing\n");
	const char *str = "hello\n";
	n = write(fd, str, strlen(str));
	printf("write bytes count = %d\n", n);
	if(n == -1) {
		perror("fail to write");
		exit(1);
	}

	close(fd);

	return 0;
}