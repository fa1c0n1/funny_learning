#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <dirent.h>

#define BUFSIZE        4096
#define CREAT_MODE     0644
#define DIR_MODE       0775

static int prompt = 0;


static void show_usage(void)
{
    fprintf(stderr, "Usage:  ./cp02 [-i] <src-file> <dest-file>\n");
}

static void cp_file2file(char *srcfile, char *destfile)
{
    int fd1, fd2, nbytes;
    char yORn;
    char buf[BUFSIZE] = {0};

    if (access(destfile, F_OK) == 0) {
        if (prompt) {
            printf("cp: Do you overwrite \"%s\"? ", destfile);
            scanf("%c", &yORn);
            if (yORn != 'y' && yORn != 'Y') {
                exit(EXIT_SUCCESS); 
            } 
        }
    }

    if ((fd1 = open(srcfile, O_RDONLY)) == -1) {
        printf("-----------------------------\n");
        perror(srcfile); 
        exit(EXIT_FAILURE);
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

    close(fd1);
    close(fd2);
}

static void cp_file2dir(char *srcfile, char *destdir)
{
    char newfile[BUFSIZE] = {0};

    char *fname = basename(srcfile);
    sprintf(newfile, "%s/%s", destdir, fname);
    cp_file2file(srcfile, newfile);
}

static void cp_dir2dir(char *srcdir, char *destdir)
{
    char newdir[BUFSIZE] = {0};     
    char newfile[BUFSIZE] = {0};
    char oldfile[BUFSIZE] = {0};
    DIR *dp;
    struct dirent *dentp;
    struct stat sbuf;
    char *dname;

    if (access(destdir, F_OK) == -1) {
        sprintf(newdir, "%s", destdir);
    } else {
        dname = basename(srcdir);
        sprintf(newdir, "%s/%s", destdir, dname);
    }

    if (mkdir(newdir, DIR_MODE) == -1) {
        fprintf(stderr, "mkdir \"%s\": %s\n", newdir, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((dp = opendir(srcdir)) == NULL) {
        fprintf(stderr, "opendir \"%s\": %s\n", srcdir, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((dentp = readdir(dp)) != NULL) {
        if (strcmp(".", dentp->d_name) == 0 || strcmp("..", dentp->d_name) == 0) {
            continue; 
        }
        sprintf(newfile, "%s/%s", newdir, dentp->d_name);
        sprintf(oldfile, "%s/%s", srcdir, dentp->d_name);
    
        if (lstat(oldfile, &sbuf) != -1 && S_ISDIR(sbuf.st_mode)) {
            printf("Skipping directory: %s\n", oldfile); 
            continue;
        }

        cp_file2file(oldfile, newfile);         
        memset(newfile, 0, strlen(newfile));
        memset(oldfile, 0, strlen(oldfile));
    }

    if (closedir(dp) == -1) {
        perror("closedir"); 
        exit(EXIT_FAILURE);
    }
}

static void do_cp(char *src, char *dest)
{
    struct stat sbuf1;
    struct stat sbuf2;
    char srcbuf[BUFSIZE] = {0};
    char destbuf[BUFSIZE] = {0};

    realpath(src, srcbuf);
    realpath(dest, destbuf);

    if (lstat(srcbuf, &sbuf1) == -1) {
        fprintf(stderr, "lstat \'%s\': %s\n", src, strerror(errno));
        exit(EXIT_FAILURE);
    } 


    if (strcmp(srcbuf, destbuf) == 0) {
        fprintf(stderr, "cp: \"%s\" and \"%s\" are the same file.\n", src, dest); 
        exit(EXIT_FAILURE);
    }

    if (access(dest, F_OK) == 0) {
        if (lstat(destbuf, &sbuf2) == -1) {
            fprintf(stderr, "lstat \'%s\': %s\n", dest, strerror(errno));
            exit(EXIT_FAILURE);
        } 
    } else {
        //case: cp f1.txt ~/<not_exist_dir>/
        if (dest[strlen(dest)-1] == '/') {
            fprintf(stderr, "The directory \"%s\" does not exist.\n", dest); 
            exit(EXIT_FAILURE);
        }
    }

    if (S_ISDIR(sbuf1.st_mode)) {
        cp_dir2dir(srcbuf, destbuf); 
    } else {
        if (S_ISDIR(sbuf2.st_mode)) {
            cp_file2dir(src, destbuf); 
        } else {
            cp_file2file(src, dest); 
        }
    }
}

int main(int argc, char *args[])
{
    int fd1, fd2;
    char buf[BUFSIZE], yORn;
    int nbytes;
    int opt, overwrite = 0;

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

    char *src = args[optind++];
    char *dest = args[optind++];

    if (access(src, F_OK) == -1) {
        perror(src); 
        exit(EXIT_FAILURE);
    }

    do_cp(src, dest);

    exit(EXIT_SUCCESS);
}

