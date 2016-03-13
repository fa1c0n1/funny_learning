#include <stdio.h>
#include <string.h>

void fortune_cookie(char msg[]) {
	printf("Message reads: %s\n", msg);
	printf("msg occupies %ld bytes\n", sizeof(msg)); //得到的是字符数组指针的大小
}

int main(void) 
{	
	char quote[] = "Cookies make you fat.";
	fortune_cookie(quote);

	return 0;
}
