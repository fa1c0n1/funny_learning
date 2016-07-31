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
	int ungetc(int c, FILE *stream);

	参数c为一个int型的变量，它的低8位为要写回文件的字符。
	参数stream为要写入该字符的文件流指针

	函数执行成功则返回参数c的值，失败则返回EOF, 并设置全局错误变量errno

	特别说明：该字符会实时地显示在文件中，所以在下一次读文件时，会读出该字符.
*/

int main(void)
{
	char buf[80];
	FILE *fp;

	int i;
	char ch;

	if((fp = fopen("test2", "w+")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	fputs("ABCefgQWC", fp);
	fflush(fp);
	fclose(fp);

	if((fp = fopen("test2", "r+")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}


	printf("the ori data is : ABCefgQWC\n");

	while((ch = fgetc(fp)) != EOF) {
		if(ch >= 'A' && ch <= 'Z') {
			i = 0;
			while(ch != EOF && ch >= 'A' && ch <= 'Z') {
				buf[i++] = ch;
				ch = fgetc(fp);
			}
			buf[i] = '\0';
			printf("%s\n", buf);
			if(ch == EOF)
				break;
			else
				ungetc((int)ch, fp);
		} else {
			i = 0;
			while(ch != EOF && ch >= 'a' && ch <= 'z') {
				buf[i++] = ch;
				ch = fgetc(fp);
			}
			buf[i] = '\0';
			printf("%s\n", buf);
			if(ch == EOF)
				break;
			else 
				ungetc((int)ch, fp);
		}
	}

	fclose(fp);

	return 0;
}