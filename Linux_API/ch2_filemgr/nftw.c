// #define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define __USE_XOPEN_EXTENDED
#include <ftw.h>


int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	printf("%s\n", fpath);
	if(typeflag == FTW_F) { 
		printf("regular file\n");
	} else {
		printf("other file\n");
	}

	printf("the level:%d, the base:%d\n", ftwbuf->level, ftwbuf->base);
	printf("\n");

	return 0;
}

int main(int argc, char **argv)
{
	if(nftw(argv[1], fn, 500, FTW_PHYS) == -1) {
		perror("fail to nftw");
		exit(1);
	}

	printf("nftw OK\n");

	return 0;
}