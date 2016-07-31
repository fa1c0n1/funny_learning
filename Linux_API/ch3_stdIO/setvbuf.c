#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define BUFSIZE    4096

/*
	setvbuf(FILE *stream, char *buf, int mode, size_t size); 

	参数stream为流文件指针。
	参数buf为字符串数组指针。
	参数mode为行为参数，有如下值：
		_IOFBF (满缓冲)：当缓冲区为空时，从流读入数据。或当缓冲区满时，向流写入数据。
		_IOLBF (行缓冲)：每次从流中读入一行数据或向流中写入—行数据。
		_IONBF (无缓冲)：直接从流中读入数据或直接向流中写入数据，而没有缓冲区。

        PS: 如果参数buf为NULL指针，则为无缓冲
	参数size定义了数组buf的长度

*/

int main(void)
{
	char buf[BUFSIZE];
	char str[BUFSIZE];
	FILE *fp;

	if((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	setvbuf(fp, buf, _IONBF, BUFSIZE);
	while(!feof(fp)) {
		fgets(str, BUFSIZE, fp);
		fputs(str, stdout);
	}

	return 0;
}