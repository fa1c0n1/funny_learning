#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *file;
	char c;

	if (argc != 2) {
		printf("Usage: clearerr filename.\n");
		exit(1);
	}

	if ((file = fopen(argv[1], "rb")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	c = getc(file);
	while (!feof(file)) {
		if (ferror(file)) {
			printf("Read Error\n");
			clearerr(file);  /* 清除错误标志 */
			break;
		} else {
			printf("%d\n", c);
			putc(c, stdout);
			c = getc(file);
		}

	}

	fclose(file);

	return 0;
}