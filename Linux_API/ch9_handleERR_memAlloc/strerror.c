#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void)
{
    char *err_str;

    err_str = strerror(EACCES);
    printf("err_str: %s\n", err_str);

    printf("EACCES: %s\n", strerror(EACCES));
    printf("EINVAL: %s\n", strerror(EINVAL));

    printf("err_str: %s\n", err_str);

    return 0;
}
