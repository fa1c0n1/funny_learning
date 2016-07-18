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
	char c;

	fp = fopen("test", "r");
	if (fp == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	while((c = fgetc(fp)) != EOF) {
		putc(c, stdout);
	}

	fclose(fp);

	return 0;
}