#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define BUFSIZE        4096
#define CREAT_MODE     0644

static void show_usage(void);

int main(int argc, char *args[])
{
    int fd1, fd2;
    char buf[BUFSIZE], yORn;
    int nbytes;
    int opt, prompt = 0, overwrite = 0;

    if (argc < 3) {
        show_usage(); 
        exit(1);
    }

    while ((opt = getopt(argc, args, "i")) != -1) {
        switch(opt) {
            case 'i' :
                prompt = 1; 
                break;
            default:  /* ? */
                show_usage();
                exit(EXIT_FAILURE);
        }
    }

    char *srcfile = args[optind++];
    char *destfile = args[optind++];
    char srcbuf[BUFSIZE] = {0};
    char destbuf[BUFSIZE] = {0};

    if ((fd1 = open(srcfile, O_RDONLY)) == -1) {
        perror(srcfile); 
        exit(EXIT_FAILURE);
    }

    realpath(srcfile, srcbuf);
    realpath(destfile, destbuf);
    if (strcmp(srcbuf, destbuf) == 0) { //need to consider the case --> cp f1.txt ./f1.txt
        fprintf(stderr, "cp: \"%s\" and \"%s\" are the same file.\n", srcfile, destfile); 
        exit(EXIT_FAILURE);
    }

    if (access(destfile, F_OK) == 0) {
        if (prompt) {
            printf("cp: Do you overwrite \"%s\"? ", destfile);
            scanf("%c", &yORn);
            if (yORn != 'y' && yORn != 'Y') {
                exit(EXIT_SUCCESS); 
            } 
        }
    }

    if ((fd2 = open(destfile, O_CREAT|O_RDWR|O_TRUNC, CREAT_MODE)) == -1) {
        perror(destfile);
        exit(EXIT_FAILURE);
    }

    while ((nbytes = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, nbytes) != nbytes) {
            perror("write error"); 
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

static void show_usage(void)
{
    fprintf(stderr, "Usage:  ./cp02 [-i] <src-file> <dest-file>\n");
}
