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
	DIR *dir;
	struct dirent *d_ptr;

	dir = opendir("/home/suhao/github/funny_learning/Linux_API");
	if(dir == NULL) {
		perror("fail to opendir");
		exit(1);
	}

	//.和..两个目录项占24个字节
	// seekdir(dir, 24);  

	/*
     * 经过测试以及在ubuntu12.04查阅man seekdir发现, 该函数不能这么用。
     * 有可能老的版本的Linux支持这样的用法吧。。。总之测试后发现不行。
     * 
     *  man seekdir有如下描述：
     *     The  seekdir()  function sets the location in the directory stream from 
     *     which the next readdir(2) call will start. The loc argument
     *     should be a value returned by a previous call to telldir(3)
	 */

	seekdir(dir, 6716920470225049525L);

	// long loc = 0;
	while((d_ptr = readdir(dir)) != NULL) {
		// loc = telldir(dir);
		// printf("dir name is: %s, loc=%ld\n", d_ptr->d_name, loc);
		printf("dir name is: %s\n", d_ptr->d_name);
	}

	closedir(dir);

	return 0;
}
