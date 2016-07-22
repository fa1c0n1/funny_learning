#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/* 

1) 位于: /usr/include/stdio.h

typedef _G_fpos_t fpos_t;

2) 位于：/usr/include/_G_config.h (Ubuntu14.04)

typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;

*/

int main(void)
{
	FILE *fp;
	long offset = 5;
	fpos_t pos;

	if ((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	fseek(fp, offset, SEEK_SET);
	printf("offset=%ld\n", ftell(fp));

	rewind(fp);
	fgetpos(fp, &pos);
	printf("offset=%ld\n", pos.__pos);

	pos.__pos = 10;
	if (fsetpos(fp, &pos) == -1) {
		perror("fail to fsetpos");
		exit(1);
	}

	printf("offset=%ld\n", ftell(fp));

	fclose(fp);

	return 0;
}
