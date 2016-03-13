#include <stdio.h>

int main(void)
{
	char s[] = "How big is it?";
	char *t = s;

	printf("sizeof(s)===%ld\n", sizeof(s));
	printf("sizeof(t)===%ld\n", sizeof(t));

	int does[] = {1, 3, 2, 1000};

	//does[3] == *(does + 3) == *(3 + does) == 3[does]
	printf("服用 %d 毫克的药\n", 3[does]);
	printf("服用 %d 毫克的药\n", does[3]);

	return 0;
}
