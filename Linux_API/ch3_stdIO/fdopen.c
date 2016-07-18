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
	FILE *fp;

	fp = fdopen(STDOUT_FILENO, "w");
	if(fp == NULL) {
		perror("fail to fdopen");
		exit(1);
	}

	fprintf(fp, "%s\n", "hello! ");
	printf("successful write the data: hello\n");

	fclose(fp);

	return 0;
}