#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define MAX    1024

int main(void)
{
	char buf[MAX];
	int fd;

	memset(buf, 0, MAX);
	if (symlink("test.txt", "sl_test.txt") == -1) {
		perror("fail to symlink");
		exit(1);
	}

	if (symlink("sl_test.txt", "sl2_test.txt")) {
		perror("fail to symlink");
		exit(1);
	}

	if (readlink("sl_test.txt", buf, MAX) == -1) {
		perror("fail to readlink");
		exit(1);
	}

	printf("%s\n", buf);

	if(readlink("sl2_test.txt", buf, MAX) == -1) {
		perror("fail to readlink");
		exit(1);
	}

	printf("%s\n", buf);

	return 0;
}