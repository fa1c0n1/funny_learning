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
	struct dirent *ptr[2];
	char root[MAX];

	if(getcwd(root, MAX) == NULL) {
		perror("fail to getcwd");
		exit(1);
	}

	dir = opendir(root);
	if(dir == NULL) {
		perror("fail to opendir");
		exit(1);
	}

	int i = 0;
	while((ptr[i] = readdir(dir)) != NULL) {
		if(strcmp(ptr[i]->d_name, ".") == 0 || strcmp(ptr[i]->d_name, "..") == 0)
			continue;

		printf("%s\n", ptr[i]->d_name);
		i++;
	}
	printf("errno=%d, i=%d, %d\n", errno, i, ptr[i] == NULL);

	//if errno is 0, that meaning finish read this dir.
	if(errno != 0) {
		perror("fail to read dir");
		exit(1);
	}

	closedir(dir);

	printf("-------------------------\n");

	int n;
	struct dirent **namelist;

	getcwd(root, MAX);
	n = scandir(root, &namelist, 0, alphasort);
	for(i = 0; i < n; i++) {
		if(strcmp(namelist[i]->d_name, ".") == 0
			|| strcmp(namelist[i]->d_name, "..") == 0) {
			continue;
		}

		printf("%s\n", namelist[i]->d_name);
	}

	return 0;
}