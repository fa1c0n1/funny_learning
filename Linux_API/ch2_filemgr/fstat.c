#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	int i, fd;
	struct stat stat_buf;

	for(i = 1; i < argc; i++) {
		fd = open(argv[i], O_RDWR);
		if(fd == -1) {
			perror("fail to open");
			exit(1);
		}

		printf("open %s OK\n", argv[i]);
		if(fstat(fd, &stat_buf) == -1) {
			perror("fail to fstat");
			exit(1);
		}

		if(S_ISREG(stat_buf.st_mode))
			printf("%s is regular file\n", argv[i]);
		else if(S_ISDIR(stat_buf.st_mode))
			printf("%s is directory\n", argv[i]);
		else if(S_ISCHR(stat_buf.st_mode))
			printf("%s is character device\n", argv[i]);
		else if(S_ISBLK(stat_buf.st_mode))
			printf("%s is block device\n", argv[i]);
		else if(S_ISFIFO(stat_buf.st_mode))
			printf("%s is FIFO\n", argv[i]);
		else if(S_ISLNK(stat_buf.st_mode))
			printf("%s is symbolic link\n", argv[i]);
		else if(S_ISSOCK(stat_buf.st_mode))
			printf("%s is socket\n", argv[i]);
		else 
			printf("unkown file\n");

		close(fd);
	}

	return 0;
}