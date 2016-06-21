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
	char *p = "/home";
	char buf[MAX];

	printf("chdir to root\n");
	chdir("/");

	if(getcwd(buf, MAX) == NULL) {
		perror("fail to getcwd");
		exit(1);
	}
	printf("cur dir: %s\n", buf);

	printf("chroot\n");
	if(chroot(p) == -1) {
		perror("fail to chroot");
		exit(1);
	}

	

	return 0;
}