#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd;
	int i;

	fd = open("test3.txt", O_CREAT|O_WRONLY);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}

	i = 0;
	while(i < 3) {
		sleep(5);

		printf("hi\n");
		if(write(fd, "hi\n", 3) == -1) {
			perror("fail to write");
			exit(1);
		}

		if(fsync(fd) == -1) {
			perror("fail to fsync");
			exit(1);
		}

		i++;
	}

	close(fd);

	return 0;
}