#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	struct stat buf;

	printf("---before lchown---\n");
	if (stat("test.txt", &buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("stat OK\n");
	printf("the owner of test.txt is: %d\n", buf.st_uid);
	printf("the group of test.txt is: %d\n", buf.st_gid);

	if (lstat("l_test.txt", &buf) == -1) {
		perror("fail to lstat");
		exit(1);
	}

	printf("lstat OK\n");
	printf("the owner of l_test.txt is: %d\n", buf.st_uid);
	printf("the group of l_test.txt is: %d\n", buf.st_gid);

	if (lchown("l_test.txt", 1020, 1001) == -1) {
		perror("fail to lchown");
		exit(1);
	}

	printf("\nlchown OK\n");
	printf("---after lchown---\n");

	if (stat("test.txt", &buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("stat OK\n");
	printf("the owner of test.txt is: %d\n", buf.st_uid);
	printf("the group of test.txt is: %d\n", buf.st_gid);

	if (lstat("l_test.txt", &buf) == -1) {
		perror("fail to lstat");
		exit(1);
	}

	printf("lstat OK\n");
	printf("the owner of l_test.txt is: %d\n", buf.st_uid);
	printf("the group of l_test.txt is: %d\n", buf.st_gid);

	return 0;
}