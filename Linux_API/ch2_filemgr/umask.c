#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void prn_mask(mode_t mask)
{
	if (mask / 10 == 0) {
		//只有1位
		printf("(oct) the original mask is 000%o\n", (unsigned int)mask);
		return ;
	}

	if (mask / 100 == 0) {
		//有两位
		printf("(oct) the original mask is 00%o\n", (unsigned int)mask);
		return ;
	}

	//有3位
	printf("(oct) the original mask is 0%o\n", (unsigned int)mask);
}

void mask_test1()
{
	mode_t pre_mask;

	pre_mask = umask(0444);
	prn_mask(pre_mask);

	pre_mask = umask(0002);
	prn_mask(pre_mask);

	pre_mask = umask(0022);
	prn_mask(pre_mask);
}

/* 屏蔽所有者、组用户和其他用户的读权限 */
#define MASK    0444

void mask_test2()
{
	int fd;
	mode_t pre_mask;

	pre_mask = umask(MASK);
	prn_mask(pre_mask);

	//创建后该文件的权限应为333
	fd = open("umask_test.txt", O_CREAT, 0777);
	if(fd == -1) {
		perror("fail to open");
		exit(1);
	}

	printf("open OK\n");
	close(fd);
}

int main(void)
{
	// mask_test1();
	mask_test2();

	return 0;
}