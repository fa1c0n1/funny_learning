#include <stdio.h>
#include <gnu/libc-version.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    printf("%d, %d\n", __GLIBC__, __GLIBC_MINOR__);
    printf("glibc version: %s\n", gnu_get_libc_version());
    
    char buf[128];
    memset(buf, 0, sizeof(buf));
    confstr(_CS_GNU_LIBC_VERSION, buf, sizeof(buf));
    printf("glibc version: %s\n", buf);

    return 0;
}
