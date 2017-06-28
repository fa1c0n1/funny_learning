#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
    const char *s1 = NULL;

    int i = (int)s1;
    printf("i=%d\n", i);

    char ch = *s1;
    printf("ch=%c\n", ch);
}
