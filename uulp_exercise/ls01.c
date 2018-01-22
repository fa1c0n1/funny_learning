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

static void show_usage(char *name)
{
    fprintf(stderr, "Usage: %s [-al] [dir1] [dir2]...\n", name);
}

static int compare(const void *p1, const void *p2)
{
    return strcmp(*(char **)p1, *(char **)p2);    
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

static void show_item_detail(char *itemPath, char *itemName)
{
    struct stat stbuf;
    struct tm *sttm;
    char modeStr[11] = {0};
    char timebuf[128] = {0};
    char hourbuf[8] = {0};
    char minbuf[8] = {0};

    if (lstat(itemPath, &stbuf) == -1) {
        fprintf(stderr, "lstat '%s': %s\n", itemPath, strerror(errno));
        return ;
    } 

    mode_to_letter(stbuf.st_mode, modeStr); 
    sttm = localtime(&stbuf.st_mtime);

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
    printf("\n");
}

static void show_dir_items(char *dirRealPath, int count)
{
    int i;
    char fullPath[PATH_MAX] = {0};

    qsort(dir_item_names, count, sizeof(char *), compare);

    if (flagDetail) {
        for (i = 0; i < count; i++) {
            sprintf(fullPath, "%s/%s", dirRealPath, dir_item_names[i]);
            show_item_detail(fullPath, dir_item_names[i]); 
        }
    } else {
        show_item_separate(count);     
    }
}

static int do_ls(char *dir)
{
    DIR *dp;
    struct dirent *dentp;
    char dirRealPath[PATH_MAX] = {0};

    if (realpath(dir, dirRealPath) == NULL) {
        fprintf(stderr, "realpath '%s': %s\n", dir, strerror(errno)); 
        return -1;
    }

    if ((dp = opendir(dir)) == NULL) {
        perror(dir); 
        return -1;
    }

    mIndex = 0; 
    while ((dentp = readdir(dp)) != NULL) {
        if (!flagAll) {
            if (strncmp(".", dentp->d_name, 1) == 0) {
                continue; 
            }     
        }

        if ((dir_item_names[mIndex] = (char *)calloc(ITEMNAME_MAX+1, sizeof(char))) == NULL) {
            perror("calloc error");                
            return -1;
        }

        strncpy(dir_item_names[mIndex], dentp->d_name, strlen(dentp->d_name));
        mIndex++;
    }

    show_dir_items(dirRealPath, mIndex);

    int i;
    for (i = 0; i < mIndex; i++) {
        free(dir_item_names[i]); 
        dir_item_names[i] = NULL;
    }

    closedir(dp);
    return 0;
}

int main(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "al")) != -1) {
        switch(opt) {
            case 'a':
                flagAll = 1;
                break;
            case 'l':
                flagDetail = 1; 
                break;
            default: /* ? */
                show_usage(argv[0]);
                exit(EXIT_FAILURE);
        } 
    }

    char *dir;
    if ((dir = argv[optind]) == NULL) {
        //e.g `ls` , `ls -l` 
        dir = ".";
        printf("%s:\n", dir);
        if(do_ls(".") == -1) {
            exit(EXIT_FAILURE); 
        } 
    } else {
        while ((dir = argv[optind++]) != NULL) {
            printf("%s:\n", dir);
            if (do_ls(dir) == -1) {
                exit(EXIT_FAILURE);             
            }         

            printf("\n");
        }
    }

    exit(EXIT_SUCCESS);
}
