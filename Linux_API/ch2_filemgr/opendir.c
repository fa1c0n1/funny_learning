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

int main(void)
{
	DIR *dir;
	struct dirent *ptr;
	char root[MAX];

	if (getcwd(root, MAX) == NULL) {
		perror("fail to getcwd");
		exit(1);
	}

	dir = opendir(root);
	if (dir == NULL) {
		perror("fail to opendir");
		exit(1);
	}

	errno = 0;
	while ((ptr = readdir(dir)) != NULL) {
		printf("type=%d, %s\n", ptr->d_type, ptr->d_name);
	}

	if(errno != 0) {
		perror("fail to readdir");
		exit(1);
	}

	return 0;
}