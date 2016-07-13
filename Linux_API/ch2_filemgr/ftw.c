#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int fn(const char *fpath, const struct stat *sb, int typeflag)
{
	printf("%s, typeflag=%d\n", fpath, typeflag);

	if (S_ISREG(sb->st_mode)) {
		printf("regular file\n");
	} else {
		printf("other file\n");
	}
	printf("\n");

	return 0;
}

int main(int argc, char **argv)
{
	if(ftw(argv[1], fn, 500) == -1) {
		perror("fail to ftw");
		exit(1);
	}

	printf("ftw OK\n");

	return 0;
}