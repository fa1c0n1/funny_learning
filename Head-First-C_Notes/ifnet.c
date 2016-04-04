#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(void)
{
   if(execl("/sbin/ifconfig", "/sbin/ifconfig", NULL) == -1) {
        if(execlp("ipconfig", "ipconfig", NULL) == -1) {
            fprintf(stderr, "Cannot run ipconfig: %s\n", strerror(errno));
            return 1;
        }
   }

   return 0;
}
