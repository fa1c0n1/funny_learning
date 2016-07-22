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
	char ch;

	if((fp = fopen("test", "a+")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	ch = '9';
	fputc((int)ch, fp);
	fflush(fp);
	
	fseek(fp, 0, SEEK_SET);
	ch = fgetc(fp);
	while(!feof(fp)) {
		putchar(ch);
		ch = fgetc(fp);
	}

	fclose(fp);

	return 0;
}