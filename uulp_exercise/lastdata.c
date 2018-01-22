#include "errhandle.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include "utmplib.h"

static void show_info(struct utmp *utbufp);
static void show_time(long timeval);

int main(void)
{
    int fd;
    struct utmp *utbufp;
    char *myline = NULL;
    int only_myself = 0;

    if (utmp_open(O_RDONLY) == -1) {
        exit_err("utmp_open", UTMP_FILE);
    }

    while ((utbufp = utmp_next()) != (struct utmp *)NULL) {
        show_info(utbufp); 
    }

    utmp_close();
    exit(EXIT_SUCCESS);
}

static void show_info(struct utmp *utbufp)
{
    if (utbufp->ut_type != USER_PROCESS) {
        return ; 
    }

    char filebuf[512] = {0};
    struct stat sbuf;

    sprintf(filebuf, "/dev/%s", utbufp->ut_line);
    if (lstat(filebuf, &sbuf) == -1) {
        exit_err("lstat", filebuf); 
    }

    printf("%-8s", utbufp->ut_user);
    printf("    ");
    printf("%-8s", utbufp->ut_line);
    printf("    ");
    show_time(sbuf.st_mtime); 
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

