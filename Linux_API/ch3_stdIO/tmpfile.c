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
	FILE *tmpfile(void);  函数构建一个临时的二进制文件用于程序的读写操作，
	并为这个文件构造一个具有唯一性的名字

	The tmpfile() function opens a unique temporary file in binary 
	read/write (w+b) mode. The file will be automatically deleted when
    it is closed or the program terminates.
*/

int main(void)
{
	FILE *fp_tmp;
	char str[80];

	if((fp_tmp = tmpfile()) == NULL) {
		perror("fail to tmpfile");
		exit(1);
	}

	fputs("this is the temp file\n", fp_tmp);
	rewind(fp_tmp);

	fgets(str, 80, fp_tmp);
	fputs(str, stdout);

	fclose(fp_tmp);

	return 0;
}