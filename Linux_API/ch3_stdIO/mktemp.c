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
附加说明1：参数 template 所指的文件名称字符串必须声明为数组,如:
    char template[] = "template-XXXXXX";
不可用
    char * template = "template-XXXXXX";

    可能是因为第二种，template是一个指向字符串常量的指针，常量是无法改变的,
    而该函数会改变template的值，而数组是可以修改的，所以这里只能传数组.


附加说明2：
    mktemp和mkdtemp函数的区别，使用前者，程序在编译会提示该函数危险。
    且,mktemp不会生成目录，而mkdtemp会生成目录.
*/

int main(void)
{
	char template[] = "tmp-XXXXXX";
	char *ptr;

	if((ptr = mktemp(template)) == NULL) {
		perror("failed mktemp");
		exit(1);
	}

	printf("temp file name is : %s\n", ptr);

	return 0;
}