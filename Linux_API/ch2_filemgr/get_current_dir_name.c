#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(void)
{

	char *dir;

	dir = get_current_dir_name();
	if(dir == NULL) {
		perror("fail to get_current_dir_name");
		printf("get_current_dir_name errno=%d\n", errno);
		exit(1);
	}

	printf("curr_dir_name=%s\n", dir);
	free((void*)dir);

	if(chdir("/home/suhao/tmp") == -1) {
		perror("fail to chdir");
		exit(1);
	}	

	printf("chdir OK\n");
	dir = get_current_dir_name();
	if(dir == NULL) {
		perror("fail to get_current_dir_name");
		printf("get_current_dir_name errno=%d\n", errno);
		exit(1);
	}

	printf("curr_dir_name=%s\n", dir);
	free((void*)dir);

	return 0;
}