#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(void)
{
    uid_t euid;
    struct passwd *stpwd; 

    euid = geteuid();
    if ((stpwd = getpwuid(euid)) == NULL) {
        perror(stpwd->pw_name); 
        exit(1);
    }

    printf("%s\n", stpwd->pw_name);

    exit(0);
}
