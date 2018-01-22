#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void exit_usage(char *cmd)
{
    fprintf(stderr, "Usage: %s <file>\n", cmd);
    exit(EXIT_FAILURE);
}

void exit_err(char *call, char *msg)
{
    fprintf(stderr, "%s \"%s\": %s\n", call, msg, strerror(errno));
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        exit_usage(argv[0]);
    }

    int fd, nbytes;
    char buf[256] = {0};

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        exit_err("open", argv[1]); 
    }

    if ((nbytes = read(fd, buf, 10)) > 0) {
        printf("1st:Read %d bytes from %s: \"%s\"\n", nbytes, argv[1], buf); 
    } else {
        exit_err("read", argv[1]); 
    }

    printf("Now, type 'chmod a-r %s; exit' \n", argv[1]);
    system("bash");

    if ((nbytes = read(fd, buf, 10)) > 0) {
        printf("2nd:Read %d bytes from %s: \"%s\"\n", nbytes, argv[1], buf); 
    } else {
        exit_err("read", argv[1]); 
    }



    exit(EXIT_SUCCESS);
}
