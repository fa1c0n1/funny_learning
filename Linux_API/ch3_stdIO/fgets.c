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

    char *fgets(char *s, int size, FILE *stream);

fgets() reads in at most one less than size characters from stream and stores them into the buffer pointed to by s.  
Reading stops after an EOF or a newline. If a newline is read, it is stored into the buffer.  
A terminating null byte ('\0') is stored after the last character in the buffer.

大意是：
    fgets()函数最多读取 size-1 个字符【为了预留后面会自动加上的结束符'\0'的位置】，
    存放在s指向的数组中。函数遇到文件结束符或回车换行符则结束，
    并在s的相应位置加上字符串结束符 '\0'
*/

int main(void)
{
	FILE *fp;
	char buf[128];

	if((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}


	if(fgets(buf, 128, fp) == NULL) {
		perror("fail to fgets");
		exit(1);
	}

	printf("%s", buf);

	fclose(fp);

	return 0;
}