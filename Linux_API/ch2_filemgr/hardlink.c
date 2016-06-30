#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX    1024

int main(void)
{
	struct stat stat_buf;

	if (stat("test.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("test.txt: the number of links is %d\n", stat_buf.st_nlink);

	if (link("test.txt", "hard_link_test.txt") == -1) {
		perror("fail to link");
		exit(1);
	}

	if (stat("test.txt", &stat_buf) == -1) {
		strerror(errno);
		exit(1);
	}

	printf("----- after link -----\n");

	printf("test.txt: the number of links is %d\n", stat_buf.st_nlink);
	printf("hard_link_test.txt: the number of links is %d\n", stat_buf.st_nlink);

	return 0;
}
