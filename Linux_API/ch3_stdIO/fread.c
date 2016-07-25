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

	if(fread(str, sizeof(str), STRINF_SIZE, fp) != STRINF_SIZE) {
		if(feof(fp)) {
			printf("Premature end the file. ");
		} else {
			printf("File read error. ");
		}
	}

	fwrite(str, sizeof(char), STRINF_SIZE, fp);
	fclose(fp);

	return 0;
}