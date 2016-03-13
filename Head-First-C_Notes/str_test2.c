#include <stdio.h>
#include <string.h>

void print_reverse(char *s) {
	size_t len = strlen(s);

	char *t = s + len - 1;

	while(t >= s) {
		printf("%c", *t);

		t = t - 1;
	}

	puts("");
}

int main(void)
{
	char str[] = "ABC";
	print_reverse(str);

	char str2[] = "exploit";

	char *position = strstr(str2, "loi");
	printf("position=%s\n", position);


	return 0;
}