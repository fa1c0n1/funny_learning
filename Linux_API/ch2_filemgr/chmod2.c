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
	//rdonly.txt原本是444权限
	if(chmod("rdonly.txt", 0777) == -1) {
		perror("fail to chmod");
		exit(1);
	}

	printf("chmod OK\n");

	return 0;
}