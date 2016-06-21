#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd1, fd2;
	int i;

	fd1 = open("test6.txt", O_CREAT|O_WRONLY, 00700);
	if(fd1 == -1) {
		perror("fail to open test6.txt");
		exit(1);
	}else {
		printf("open test6.txt OK\n");
	}

	fd2 = open("test7.txt", O_CREAT|O_WRONLY, 00700);
	if(fd2 == -1) {
		perror("fail to open test7.txt");
		exit(1);
	}else {
		printf("open test7.txt OK\n");
	}

	i = 0;
	while(i < 2) {
		sleep(5);

		printf("hi\n");
		if(write(fd1, "hi\n", 3) == -1) {
			perror("fail to write test6.txt");
			exit(1);
		}

		printf("hi\n");
		if(write(fd2, "hi\n", 3) == -1) {
			perror("fail to write test7.txt");
			exit(1);
		}

		sync();
		i++;
	}

	close(fd1);
	close(fd2);

	return 0;
}