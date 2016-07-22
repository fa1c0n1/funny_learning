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
	FILE *fp, *fp_bak;
	char buf[128];
	char ch;

	if ((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen test");
		exit(1);
	}

	if ((fp_bak = fopen("test_bak", "w+")) == NULL) {
		perror("fail to fopen test");
		exit(1);
	}

	while(!feof(fp)) {
		fgets(buf, sizeof(buf), fp);
		fputs(buf, fp_bak);
		fflush(fp_bak);
	}
	fclose(fp);

	fseek(fp_bak, 0, SEEK_SET);
	while(fgets(buf, sizeof(buf), fp_bak) != NULL) {
		fputs(buf, stdout);
	}
			
	fclose(fp_bak);

	return 0;
}