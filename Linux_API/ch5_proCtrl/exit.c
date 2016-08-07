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
	函数：void exit(int status);

	功能详解：
	     exit()函数结束进程的运行，并且进入内核做一些清理工作。

	参数：
	    status表示进程的退出状态。这是一个整数，
	    可以在shell中检测到这个整数的值。
        进程退出状态的整数值是一个8位的无符号整数，其取值范围是0~255.
        如果超过这个数值，系统会自动将其截短为8位。

    PS: 在进程调用exit()函数退出运行后，进程会关闭文件，并且将内存缓冲区中的
        文件内容回写到磁盘文件上去。这也是进程所做的清理工作之一。

 */

int main(void)
{
	FILE *fp;

	printf("open file\n");
	fp = fopen("test.txt", "w+");
	if(fp == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	printf("write file\n");
	if(fwrite("hello world", sizeof(char), strlen("hello world"), fp) < 0) {
		perror("fail to fwrite");
		exit(1);
	}

	fclose(fp);

	printf("exit\n");
	exit(255);

	return 0;
}