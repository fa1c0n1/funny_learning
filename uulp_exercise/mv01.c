#include "errhandle.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>

void mvFile2File(char *srcfile, char *destfile)
{
    if (rename(srcfile, destfile) == -1) {
        exit_err("rename", destfile);
    }
}

void mvFile2Dir(char *srcfile, char *dir)
{
    char *newfile;
    char *tdir;

    if ((newfile = (char *)calloc(PATH_MAX, sizeof(char))) == NULL) {
        exit_err("calloc", "newfile"); 
    }

    if ((tdir = (char *)calloc(strlen(dir)+1, sizeof(char))) == NULL) {
        exit_err("calloc", "tdir"); 
    }
    strcpy(tdir, dir);
    if (tdir[strlen(tdir)-1] == '/') {
        tdir[strlen(tdir)-1] = '\0'; 
    }

    sprintf(newfile, "%s/%s", tdir, basename(srcfile));
    printf("newfile=%s\n", newfile);

    if (rename(srcfile, newfile) == -1) {
        exit_err("rename", dir); 
    }

    free(newfile);
    free(tdir);
}

void do_mv(char *srcfile, char *dest)
{
    struct stat sbuf;
    char *pathbuf;

    if ((pathbuf = (char *)calloc(PATH_MAX, sizeof(char))) == NULL) {
        exit_err("calloc", ""); 
    }

    if (realpath(dest, pathbuf) == NULL) {
        if (errno == ENOENT) {
            mvFile2File(srcfile, dest); 
            return ;
        } else {
            exit_err("realpath", dest);
        } 
    }

    if (lstat(pathbuf, &sbuf) == -1) {
        free(pathbuf); 
        exit_err("lstat", dest); 
    }
    free(pathbuf); 

    if (S_ISDIR(sbuf.st_mode)) {
        mvFile2Dir(srcfile, dest); 
    } else {
        mvFile2File(srcfile, dest); 
    }
}

int main(int argc, char *argv[])
{
    const char *use = "Usage: %s <src-file> <dest-file>|<dest-dir>\n";

    if (argc != 3) {
        exit_usage(use, argv[0]);
    }  

    do_mv(argv[1], argv[2]);

    exit(EXIT_SUCCESS);
}
