#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/ioctl.h>

#define COUNT        4096 
#define ITEMNAME_MAX    255 

static int flagAll = 0;
static int flagDetail = 0;
static int flagReverse = 0;
static int flagRecursion = 0;
static int flagAtime = 0;
static int flagIno = 0;
static int mIndex;
static int scrWidth;
static int scrHeight;

static void measure_screen(void)
{
    struct winsize wbuf;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wbuf) == -1) {
        perror("ioctl");
        return ;
    }

    scrWidth = wbuf.ws_col;
    scrHeight = wbuf.ws_row;
}

static char *dir_item_names[COUNT] = {0};

static void exit_usage(char *cmd)
{
    fprintf(stderr, "Usage: %s [-alrRr] [dir1] [dir2]...\n", cmd);
    exit(EXIT_FAILURE);
}

static void exit_err(char *cmd, char *msg)
{
    fprintf(stderr, "%s \"%s\": %s\n", cmd, msg, strerror(errno));
    exit(EXIT_FAILURE);
}

static void *sh_calloc(size_t nmemb, size_t size)
{
    void *newptr;

    if ((newptr = calloc(nmemb, size)) == NULL) {
        exit_err("calloc", ""); 
    }

    return newptr;
}

static void *sh_realloc(void *ptr, size_t size)
{
    void *newptr;

    if ((newptr = realloc(ptr, size)) == NULL) {
        exit_err("realloc", "");
    }

    return newptr;
}

static int compare_by_filename(const void *p1, const void *p2)
{
    if (flagReverse) {
        return strcmp(*(char **)p2, *(char **)p1); 
    }

    return strcmp(*(char **)p1, *(char **)p2);    
}

static int compare_by_atime(const void *p1, const void *p2, void *parentDir)
{
    char *fullPath1 = (char *)sh_calloc(PATH_MAX, sizeof(char));
    char *fullPath2 = (char *)sh_calloc(PATH_MAX, sizeof(char));
    struct stat sbuf;
    time_t atime1, atime2;

    sprintf(fullPath1, "%s/%s", (char *)parentDir, *(char **)p1);
    sprintf(fullPath2, "%s/%s", (char *)parentDir, *(char **)p2);

    if (lstat(fullPath1, &sbuf) == -1) {
        exit_err("lstat", fullPath1); 
    }
    atime1 = sbuf.st_atime;

    if (lstat(fullPath2, &sbuf) == -1) {
        exit_err("lstat", fullPath2); 
    } 
    atime2 = sbuf.st_atime;

    free(fullPath1);
    free(fullPath2);

    if (flagReverse) {
        return atime1 < atime2; 
    }

    return atime1 > atime2;
}

static void mode_to_letter(mode_t mode, char str[])
{
    strncpy(str, "----------", 10);

    if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISFIFO(mode)) str[0] = 'p';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISSOCK(mode)) str[0] = 's';

    if (mode & S_IRUSR) str[1] = 'r'; 
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) {
        if (mode & S_ISUID) {
             str[3] = 's';
        } else {
            str[3] = 'x';
        }
    } else {
        if (mode & S_ISUID) {
            str[3] = 'S'; 
        } 
    }

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) {
        if (mode & S_ISGID) {
            str[6] = 's';
        } else {
            str[6] = 'x'; 
        }
    } else {
        if (mode & S_ISGID) {
            str[6] = 'S'; 
        } 
    }

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) {
        if (mode & S_ISVTX) {
            str[9] = 't';
        } else {
            str[9] = 'x';
        }
    } else {
        if (mode & S_ISVTX) {
            str[9] = 'T'; 
        } 
    }
}

static char *uid_to_name(uid_t uid)
{
    struct passwd *pwdp; 
    char invalidUser[32] = {0};

    if ((pwdp = getpwuid(uid)) == NULL) {
        sprintf(invalidUser, "%d", uid);
        fprintf(stderr, "getpwuid %d: %s\n", uid, strerror(errno));
        return invalidUser;
    }

    return pwdp->pw_name;
}

static char *gid_to_name(gid_t gid)
{
    struct group *grpp; 
    char invalidGroup[32] = {0};

    if ((grpp = getgrgid(gid)) == NULL) {
        sprintf(invalidGroup, "%d", gid); 
        fprintf(stderr, "getgrgid %d: %s\n", gid, strerror(errno));
        return invalidGroup;
    }

    return grpp->gr_name;
}

static void show_item_detail(char **itemPath, char *itemName)
{
    struct stat stbuf;
    struct tm *sttm;
    char modeStr[11] = {0};
    char timebuf[128] = {0};
    char hourbuf[8] = {0};
    char minbuf[8] = {0};

    if (lstat(*itemPath, &stbuf) == -1) {
        fprintf(stderr, "lstat '%s': %s\n", *itemPath, strerror(errno));
        return ;
    } 

    mode_to_letter(stbuf.st_mode, modeStr); 
    sttm = flagAtime ? localtime(&stbuf.st_atime) : localtime(&stbuf.st_mtime);

    if (sttm->tm_hour < 10) {
        sprintf(hourbuf, "0%d", sttm->tm_hour);
    } else {
        sprintf(hourbuf, "%d", sttm->tm_hour); 
    }

    if (sttm->tm_min < 10) {
        sprintf(minbuf, "0%d", sttm->tm_min); 
    } else {
        sprintf(minbuf, "%d", sttm->tm_min);  
    }

    snprintf(timebuf, sizeof(timebuf), "%2d月 %2d %s:%s", 
            sttm->tm_mon+1, sttm->tm_mday, hourbuf, minbuf); 

    if (flagIno) {
        printf("%d ", stbuf.st_ino); 
    }

    printf("%s", modeStr);
    printf("%4d ", stbuf.st_nlink);
    printf("%-8s", uid_to_name(stbuf.st_uid));
    printf("%-8s", gid_to_name(stbuf.st_gid));
    printf("%8ld ", stbuf.st_size);
    printf("%12s ", timebuf);
    printf("%s\n", itemName);
}

static void show_item_separate(int count)
{
    int col, maxLen, colLen;
    int i, tLen;

    if (dir_item_names[0] == NULL) {
        return ; 
    }

    maxLen = strlen(dir_item_names[0]);
    for (i = 1; i < count; i++) {
        if ((tLen = strlen(dir_item_names[i])) > maxLen) {
            maxLen = tLen; 
        } 
    } 
    colLen = maxLen + 1; 

    measure_screen();
    col = scrWidth / colLen;

    for (i = 0; i < count; i++) {
        printf("%-*s ",maxLen, dir_item_names[i]);

        if ((i+1) % col == 0) {
            printf("\n"); 
        } 
    }
}

static void show_dir_items(char **dirRealPath, int count)
{
    int i;
    char *fullPath = NULL;

    if (flagAtime) {
        if (flagDetail) {
            //'ls -lu'
            qsort(dir_item_names, count, sizeof(char *), compare_by_filename);
        } else {
            qsort_r(dir_item_names, count, sizeof(char *), compare_by_atime, *dirRealPath);
        }
    } else {
        qsort(dir_item_names, count, sizeof(char *), compare_by_filename);
    }

    if (flagDetail) {
        fullPath = (char *)sh_calloc(PATH_MAX, sizeof(char));
        for (i = 0; i < count; i++) {
            sprintf(fullPath, "%s/%s", *dirRealPath, dir_item_names[i]);
            show_item_detail(&fullPath, dir_item_names[i]); 
            memset(fullPath, 0, strlen(fullPath));
        }
        free(fullPath);
    } else {
        show_item_separate(count);     
    }
    printf("\n");
}

static void do_ls(char *dir)
{
    printf("%s:\n", dir);

    DIR *dp;
    struct dirent *dentp;
    char *dirRealPath = NULL;
    char *itemRealPath = NULL;
    char **subDirItems = NULL;
    int dirItemCount = 0;
    
    dirRealPath = (char *) sh_calloc(PATH_MAX, sizeof(char));
    if (realpath(dir, dirRealPath) == NULL) {
        exit_err("realpath", dir);
    }

    if ((dp = opendir(dir)) == NULL) {
        exit_err("opendir", dir);
    }

    itemRealPath = (char *)sh_calloc(PATH_MAX, sizeof(char));
    mIndex = 0; 
    while ((dentp = readdir(dp)) != NULL) {
        if (!flagAll) {
            if (strncmp(".", dentp->d_name, 1) == 0) {
                continue; 
            }     
        }

        if (flagRecursion) {
            if (dentp->d_type == DT_DIR && strcmp(".", dentp->d_name) != 0 && strcmp("..", dentp->d_name) != 0) {
                subDirItems = (char **)sh_realloc(subDirItems, (dirItemCount+1)*sizeof(char *));
                subDirItems[dirItemCount] = (char *) sh_calloc(PATH_MAX, sizeof(char));
                sprintf(itemRealPath, "%s/%s", dirRealPath, dentp->d_name);
                strncpy(subDirItems[dirItemCount], itemRealPath, strlen(itemRealPath));
                memset(itemRealPath, 0, strlen(itemRealPath));
                dirItemCount++;
            }
        }

        if ((dir_item_names[mIndex] = (char *)calloc(ITEMNAME_MAX+1, sizeof(char))) == NULL) {
            exit_err("calloc", "");
        }

        strncpy(dir_item_names[mIndex], dentp->d_name, strlen(dentp->d_name));
        mIndex++;
    }
    free(itemRealPath);

    show_dir_items(&dirRealPath, mIndex);
    free(dirRealPath);

    int i;
    for (i = 0; i < mIndex; i++) {
        free(dir_item_names[i]); 
        dir_item_names[i] = NULL;
    }

    closedir(dp);

    if (flagRecursion) {
        for (i = 0; i < dirItemCount; i++) {
            if (!flagDetail) {
                printf("\n"); 
            }
            do_ls(subDirItems[i]);
            free(subDirItems[i]);
            subDirItems[i] = NULL;
        }

        free(subDirItems);
        subDirItems = NULL;
    }
}

int main(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "alrRui")) != -1) {
        switch(opt) {
            case 'a':
                flagAll = 1;
                break;
            case 'l':
                flagDetail = 1; 
                break;
            case 'r':
                flagReverse = 1;
                break;
            case 'R':
                flagRecursion = 1;
                break;
            case 'u':
                flagAtime = 1;
                break;
            case 'i':
                flagIno = 1;
                break;
            default: /* ? */
                exit_usage(argv[0]);
                exit(EXIT_FAILURE);
        } 
    }

    char *dir;
    if ((dir = argv[optind]) == NULL) {
        //e.g `ls` , `ls -l` 
        dir = ".";
        do_ls(".");
    } else {
        while ((dir = argv[optind++]) != NULL) {
            do_ls(dir);
        }
    }

    exit(EXIT_SUCCESS);
}
