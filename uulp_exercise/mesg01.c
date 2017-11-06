#include "errhandle.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *use_fmt = "Usage: %s [y|n]";

    struct stat stbuf;
    mode_t perm;

    if (fstat(STDIN_FILENO, &stbuf) == -1)
        exit_err("fstat", "stdin");

    perm = stbuf.st_mode & 07777;

    if (argc == 1) {
        printf("is %c\n", (perm&S_IWGRP) ?'y':'n');
        exit(EXIT_SUCCESS);
    }

    if (argc == 2) {
        if (argv[1][0] == 'n') 
            perm &= ~S_IWGRP;
        else if (argv[1][0] == 'y')
            perm |= S_IWGRP;
        else
            exit_usage(use_fmt, argv[0]);
    } else {
        exit_usage(use_fmt, argv[0]); 
    }

    if (fchmod(STDIN_FILENO, perm) == -1)
        exit_err("fchmod", "stdin");

    exit(EXIT_SUCCESS);
}
