#include <stdio.h>

int main(void)
{
	// char name[40];
	// printf("Enter your name: ");
	// scanf("%3s", name);

	// printf("name=%s\n", name);

	// printf("-------------------\n");

	char name2[40];
	fgets(name2, sizeof(name2), stdin);

	printf("name2=%s\n", name2);

	return 0;
}
