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
	setlinebuf(FILE *stream) 函数设置参数stream指定的流文件为行缓冲的类型，
	一般文件的默认缓冲模式为页缓冲，设置为行缓冲后，对文件的读写都是以行为单位
	进行
*/

int main(void)
{
	FILE *fp;
	char str[80];

	if((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	setlinebuf(fp);
	while(!feof(fp)) {
		fgets(str, sizeof(str), fp);
		fputs(str, stdout);
	}

	fclose(fp);

	return 0;
}