#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int main(void)
{
	FILE *fp;
	int fd;
	char buf[128];

	if ((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	if ((fd = fileno(fp)) == -1) {
		perror("fail to fileno");
		exit(1);
	}

	printf("fd=%d\n", fd);

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		printf("%s", buf);
	}

	fclose(fp);

	return 0;
}
