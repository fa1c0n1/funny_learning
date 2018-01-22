#include "errhandle.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <utime.h>

#define BUFSIZE        4096
#define CREAT_MODE     0644

int main(int argc, char *argv[])
{
    int fd1, fd2;
    char buf[BUFSIZE], yORn;
    int nbytes;
    int opt, prompt = 0, overwrite = 0;

    if (argc < 3) {
        exit_usage("Usage: %s [-i] <src-file> <dest-file>\n", argv[0]);
    }

    while ((opt = getopt(argc, argv, "i")) != -1) {
        switch(opt) {
            case 'i' :
                prompt = 1; 
                break;
            default:  /* ? */
                exit_usage("Usage: %s [-i] <src-file> <dest-file>", argv[0]);
        }
    }

    char *srcfile = argv[optind++];
    char *destfile = argv[optind++];
    char srcbuf[BUFSIZE] = {0};
    char destbuf[BUFSIZE] = {0};
    struct stat sbuf;
    struct utimbuf utbuf;

    if ((fd1 = open(srcfile, O_RDONLY)) == -1) {
        exit_err("open", srcfile);
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


    if (lstat(srcbuf, &sbuf) == -1) {
        exit_err("lstat", srcbuf); 
    }
    utbuf.actime = sbuf.st_atime;
    utbuf.modtime = sbuf.st_mtime;

    if ((fd2 = open(destfile, O_CREAT|O_RDWR|O_TRUNC, CREAT_MODE)) == -1) {
        exit_err("open", destfile);
    }

    while ((nbytes = read(fd1, buf, sizeof(buf))) > 0) {
        if (write(fd2, buf, nbytes) != nbytes) {
            exit_err("write", destfile);
        }
    }

    if (utime(destfile, &utbuf) == -1) {
        exit_err("utime", destfile); 
    }

    exit(EXIT_SUCCESS);
}

static void show_usage(void)
{
    fprintf(stderr, "Usage:  ./cp02 [-i] <src-file> <dest-file>\n");
}
