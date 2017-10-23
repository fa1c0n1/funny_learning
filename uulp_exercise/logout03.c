#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmp.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

static char *get_line(int fd);

int main(void)
{
    int ret; 
    char *cur_line; 

    if ((cur_line = get_line(STDIN_FILENO)) == NULL) {
        fprintf(stderr, "ttyname error: %s\n", strerror(errno)); 
        exit(1);
    }

    ret = logout(cur_line);
    if (ret == 0) {
        perror("logout"); 
    }

    exit(ret);
}

static char *get_line(int fd)
{
    char *tline;

    tline = ttyname(fd);
    if (strncmp(tline, "/dev/", 5) == 0) {
        tline += 5; 
    }

    return tline;
}
