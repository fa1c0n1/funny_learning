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
	char *str = "\n5678\n";

	fp = fopen("test", "a");
	if(fp == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	fwrite(str, strlen(str), 1, fp);
	fflush(fp);
	printf("fwrite OK\n");
	fclose(fp);

	fp = fopen("test", "r");
	if(fp == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	char c = getc(fp);
	while(!feof(fp)) {
		putc(c, stdout);
		c = getc(fp);
	}

	fclose(fp);

	return 0;
}