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

int get_file_count(char *root)
{
	int total = 0;
	DIR *dir;
	struct dirent *ptr;
	char path[MAX];

	dir = opendir(root);
	if (dir == NULL) {
		perror("fail to opendir");
		exit(1);
	}

	errno = 0;

	while ((ptr = readdir(dir)) != NULL) {
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) continue;
		if (ptr->d_type == DT_DIR) {
			sprintf(path, "%s/%s", root, ptr->d_name);
			total += get_file_count(path);
		} else if (ptr->d_type == DT_REG) {
			total++;
		}

	}

	if (errno != 0) {
		perror("fail to readdir");
		exit(1);
	}

	closedir(dir);

	return total;
}

int main(int argc, char **argv)
{
	int total = 0;

	if(argc != 2) {
		printf("Wrong Usage!\n");
		exit(1);
	}

	total = get_file_count(argv[1]);
	printf("%s has %d files\n", argv[1], total);

	return 0;
}