#include "errhandle.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DIR_MODE    0775

static int flagParents = 0;

static void do_mkdir(char *dpath)
{
    if (mkdir(dpath, DIR_MODE) == -1) {
        if (errno != ENOENT || !flagParents) {
            if (errno == EEXIST) {
                return ; 
            }
            exit_err("mkdir", dpath); 
        }
    } else {
        return ; 
    }

    char *dir;
    char *pdir;     //parent dir
    char *sdir;     //sub dir

    if ((dir = (char *)calloc(1, strlen(dpath)+1)) == NULL) {
        exit_err("calloc", "dir"); 
    }
    strcpy(dir, dpath);
    printf("dir=%s\n", dir);

    if (dir[strlen(dir)-1] == '/') {
        dir[strlen(dir)-1] = '\0';
    }

    sdir = strrchr(dir, '/');
    if ((pdir = (char *)calloc(1, strlen(dir)+1)) == NULL) {
        exit_err("calloc", "pdir"); 
    }
    strncpy(pdir, dir, sdir-dir);
    printf("pdir=%s\n", pdir);

    do_mkdir(pdir);
    free(pdir);
    pdir = NULL;

    do_mkdir(dir);
    free(dir);
    dir = NULL;
}

int main(int argc, char *argv[])
{
    const char *use = "Usage: %s [-p] [DIR1] [DIR2] ...\n";
    int opt;

    if (argc < 2) {
        exit_usage(use, argv[0]); 
    }

    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch(opt) {
            case 'p': 
                flagParents = 1;                
                break;
            default: /* ? */
                exit_usage(use, argv[0]);
        } 
    }

    if (flagParents) {
        if (argv[optind] == NULL) {
            exit_msg(argv[0], "-p 缺少操作参数."); 
        }
    }

    char *tdir = NULL;
    while ((tdir = argv[optind++]) != NULL) {
        printf("------ dir:%s, start ---------\n", tdir); 
        do_mkdir(tdir);
        printf("------ dir:%s, end ---------\n", tdir); 
    }

    exit(EXIT_SUCCESS);
}
