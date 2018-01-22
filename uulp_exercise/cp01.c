#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE        4096
#define CREAT_MODE     0644

static void show_usage(void);

int main(int argc, char *args[])
{
    int fd1, fd2;
    char buf[BUFSIZE];
    int nbytes;

    if (argc != 3) {
        show_usage(); 
        exit(1);
    }

    if ((fd1 = open(args[1], O_RDONLY)) == -1) {
        perror(args[1]); 
        exit(1);
    }

    if (strcmp(args[1], args[2]) == 0) {
        fprintf(stderr, "cp: \"%s\" and \"%s\" are the same file.\n", args[1], args[2]); 
        exit(1);
    }

    if ((fd2 = open(args[2], O_CREAT|O_RDWR|O_TRUNC, CREAT_MODE)) == -1) {
        perror(args[2]);
        exit(1);
    }

    while ((nbytes = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, nbytes) != nbytes) {
            perror("write error"); 
            exit(1);
        }
    }

    exit(0);
}

static void show_usage(void)
{
    fprintf(stderr, "Usage:  ./cp <src-file> <dest-file>\n");
}
