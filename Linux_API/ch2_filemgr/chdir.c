#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX    1024

int main(void)
{
	char root[MAX];
	char *p = "hello chdir\n";

	getcwd(root, MAX);
	printf("before chdir: curdir=%s\n", root);

	if(chdir("/home/suhao/tmp") == -1) {
		perror("fail to chdir");
		exit(1);
	}

	getcwd(root, MAX);
	printf("after chdir: curdir=%s\n", root);

	int fd;
	fd = open("test_chdir.txt", O_CREAT|O_RDWR, 0700);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}
	printf("open OK\n");

	if(write(fd, p, strlen(p)) == -1) {
		perror("fail to write");
		exit(1);
	}	
	printf("write OK\n");

	close(fd);

	return 0;
}