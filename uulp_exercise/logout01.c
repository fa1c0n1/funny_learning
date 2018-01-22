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

#define UTMP_SIZE    (sizeof(struct utmp))

static char *get_line(int fd);
static int logout_tty(char *line);

int main(void)
{
    int ret; 
    char *cur_line; 

    if ((cur_line = get_line(STDIN_FILENO)) == NULL) {
        fprintf(stderr, "ttyname error: %s\n", strerror(errno)); 
        exit(1);
    }

    ret = logout_tty(cur_line);

    exit(ret);
}

static int logout_tty(char *line)
{
    int fd; 
    struct utmp utbuf;
    int retval = -1;

    if ((fd = open(UTMP_FILE, O_RDWR)) == -1) {
        perror(UTMP_FILE); 
        return -1;
    }

    while (read(fd, &utbuf, UTMP_SIZE) == UTMP_SIZE) {
        if (utbuf.ut_type == USER_PROCESS && strcmp(utbuf.ut_line, line) == 0) {
            utbuf.ut_type = DEAD_PROCESS;
            utbuf.ut_time = time((time_t *)NULL);

            if (lseek(fd, -UTMP_SIZE, SEEK_CUR) != -1) {
                if (write(fd, &utbuf, UTMP_SIZE) == UTMP_SIZE) {
                    retval = 0; 
                } else {
                    perror("write");
                }
            } else {
                perror("lseek"); 
            }

            break;
        } 
    } 

    close(fd);
    return retval;
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
