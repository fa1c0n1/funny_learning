#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int main(void)
{
	FILE *ptr_file;

	ptr_file = fopen("test", "r");
	if(ptr_file == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	printf("fopen OK\n");

	if(fclose(ptr_file)) {
		perror("fail to fclose");
		exit(1);
	}

	printf("fclose OK\n");

	return 0;
}