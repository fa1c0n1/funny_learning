#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/*
	setbuf/setbuffer函数用来在读取文件之前，设置该文件的缓冲区为自定义的缓冲区
*/

#define BUFSIZE    4096

int main(void)
{
	char buffer[BUFSIZE];
	char str[BUFSIZE];
	FILE *fp;

	if((fp = fopen("test", "a+")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	memset(buffer, 0, BUFSIZE);
	memset(str, 0, BUFSIZE);

	// setbuf(fp, buffer);
	setbuffer(fp, buffer, BUFSIZE);
	fputs("\nThis is sent to a buffered stream", fp);
	fflush(fp);
	fclose(fp);

	if((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	// char ch = fgetc(fp);
	// while(ch != EOF) {
	// 	putchar(ch);
	// 	ch = fgetc(fp);
	// }

	while(!feof(fp)) {
		fgets(str, BUFSIZE, fp);
		printf("%s", str);
	}

	fclose(fp);

	return 0;
}