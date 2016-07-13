#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define MAX    1024

//rmdir(), remove() 都只能删除空目录

int main(void)
{
	char path[MAX];
	memset(path, 0, MAX);

	if(realpath("../", path) == NULL) {
		perror("fail to realpath");
		exit(1);
	}

	char rm_path[MAX];
	memset(rm_path, 0, MAX);
	sprintf(rm_path, "%s/tmp", path);

	if(remove(rm_path) == -1) {
		perror("fail to remove");
		exit(1);
	}

	return 0;
}