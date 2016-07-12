#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/**
  * 函数原型：void rewinddir(DIR *dir);
  *   
  * 函数功能详解：
  *     rewinddir函数将一个目录的当前读写位置回到目录文件的开始处，这个操作称为回卷.
  */

int main(void)
{
	DIR *dir;
	struct dirent *d_ptr;

	dir = opendir("/home/android/gitHub/funny_learning/Linux_API/ch2_filemgr");
	if(dir == NULL) {
		perror("fail to opendir");
		exit(1);
	}

	errno = 0;
	while((d_ptr = readdir(dir)) != NULL) {
		printf("dir name is: %s \n", d_ptr->d_name);
	}

	if(errno != 0) {
		perror("fail to readdir");
		exit(1);
	}


	printf("read dir again\n");
	printf("------------------------------\n");
	printf("------------------------------\n");
	printf("------------------------------\n");
	rewinddir(dir);

	errno = 0;
	while((d_ptr = readdir(dir)) != NULL) {
		printf("dir name is: %s \n", d_ptr->d_name);
	}

	if(errno != 0) {
		perror("fail to readdir again");
		exit(1);
	}

	return 0;
}