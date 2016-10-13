#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define STRINF_SIZE    128

int main(void)
{
	FILE *fp;
	char str[STRINF_SIZE];

	if((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	memset(str, 0, sizeof(str));
	if(fread(str, sizeof(char), STRINF_SIZE, fp) != STRINF_SIZE) {
		if(!feof(fp)) {
			printf("File read error. ");
			exit(1);
		}
	}

	fwrite(str, sizeof(char), strlen(str), stdout);
	fclose(fp);

	return 0;
}