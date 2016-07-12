#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
	printf("change name of a regular file\n");
	if(rename("rn.txt", "rn_test.txt") == -1) {
		perror("fail to rename");
		exit(1);
	}

	printf("change name of a regular file\n");
	if(rename("rn2.txt", "../rn2_test.txt") == -1) {
		perror("fail to rename");
		exit(1);
	}

	return 0;
}