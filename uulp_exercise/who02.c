#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include "utmplib.h"

static void show_info(struct utmp *utbufp);
static void show_time(long timeval);
static char *get_line(int fd);

void show_usage()
{
    fprintf(stderr, "Usage: \n\t \
            1) ./who02\n\t \
            2) ./who02 am i\n");
}

int main(int argc, char *args[])
{
    int fd;
    struct utmp *utbufp;
    char *myline = NULL;
    int only_myself = 0;

    if (argc != 1 && argc != 3) {
        show_usage(); 
        exit(1);
    }

    if (argc == 3) {
        if (strcmp(args[1], "am") == 0 && strcmp(args[2], "i") == 0) {
            only_myself = 1; 
            myline = get_line(STDIN_FILENO); 
        } else {
            show_usage(); 
            exit(1);
        }
    }

    if (utmp_open(O_RDONLY) == -1) {
        perror(UTMP_FILE); 
        exit(1);
    }

    while ((utbufp = utmp_next()) != (struct utmp *)NULL) {
        if (only_myself) {
           if (strcmp(utbufp->ut_line, myline) != 0) {
                continue; 
           }
        }
        show_info(utbufp); 
    }

    utmp_close();
    exit(0);
}

static char *get_line(int fd)
{
    char *tline;

    tline = ttyname(fd);
    if (strncmp(tline, "/dev/", 5) == 0) {
        tline += 5; 
    }

    return tline;
}

static void show_info(struct utmp *utbufp)
{
    if (utbufp->ut_type != USER_PROCESS) {
        return ; 
    }

    printf("%-8s", utbufp->ut_user);
    printf("    ");
    printf("%-8s", utbufp->ut_line);
    printf("    ");
    show_time(utbufp->ut_time); 
    printf("    ");

    if (utbufp->ut_host[0] != '\0') {
        printf("(%-s)", utbufp->ut_host);
    }

    printf("\n");
}

static void show_time(long timeval)
{
    struct tm *tbuf;
    char buf[128] = {0};
    char hourbuf[16] = {0};
    char minbuf[16] = {0};

    tbuf = localtime(&timeval);
    if (tbuf->tm_hour < 10) {
        sprintf(hourbuf, "0%d", tbuf->tm_hour);
    } else {
        sprintf(hourbuf, "%d", tbuf->tm_hour); 
    }

    if (tbuf->tm_min < 10) {
        sprintf(minbuf, "0%d", tbuf->tm_min); 
    } else {
        sprintf(minbuf, "%d", tbuf->tm_min);  
    }

    snprintf(buf, sizeof(buf), "%d-%d-%d %s:%s", 
            tbuf->tm_year+1900, tbuf->tm_mon+1, tbuf->tm_mday, hourbuf, minbuf); 

    printf("%s", buf);
}

