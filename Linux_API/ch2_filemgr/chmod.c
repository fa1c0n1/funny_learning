#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NEW_MODE    S_IRUSR|S_IRGRP|S_IROTH
#define OFF_ALL_MODE    (~S_IRWXU & ~S_IRWXG & ~S_IRWXO)

int main(void)
{
	printf("%o\n", OFF_ALL_MODE);

	mode_t mode;
	struct stat stat_buf;

	if(chmod("test.txt", NEW_MODE) == -1) {
		perror("fail to chmod test.txt");
		exit(1);
	}
	printf("chmod test.txt OK\n");

	if(stat("test2.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	mode = stat_buf.st_mode;
	mode &= OFF_ALL_MODE;
	mode |= NEW_MODE;

	if(chmod("test2.txt", mode) == -1) {
		perror("fail to chmod test2.txt");
		exit(1);
	}
	printf("chmod test2.txt OK\n");

	return 0;
}