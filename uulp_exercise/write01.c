#include "errhandle.h"
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "utmplib.h"
#include <fcntl.h>
#include <time.h>

#define BUFLEN    512

static int log_times = 0;

void getterm(char *param, char *term)
{
    struct stat stbuf;
    struct passwd *stpwd;
    struct utmp *utbufp;

    if (strncmp(param, "/dev/", 5) == 0) {
        if (stat(param, &stbuf) == -1)
            exit_err("stat", param); 

        if (S_ISCHR(stbuf.st_mode)) {
            strncpy(term, param, strlen(param)); 
            return ;
        }        
    } else {
        sprintf(term, "/dev/%s", param);
        if (stat(term, &stbuf) == 0) {
            if (S_ISCHR(stbuf.st_mode))
                return ;
        } else {
             memset(term, 0, strlen(term));
            if ((stpwd = getpwnam(param)) == NULL)
                exit_err("getpwnam", param);

            if (utmp_open(O_RDONLY) == -1) {
                exit_err("utmp_open", ""); 
            }

            while ((utbufp = utmp_next()) != NULL) {
                if (utbufp->ut_type == USER_PROCESS 
                    && strcmp(utbufp->ut_user, stpwd->pw_name) == 0 
                    && strncmp(utbufp->ut_line, ":", 1) != 0) {
                     
                    log_times++;
                    if (log_times == 1)
                        sprintf(term, "/dev/%s", utbufp->ut_line);
                }
            }
            
            if (log_times > 1)
                printf("Warning: %s is logged in %d times, using %s\n", param, log_times, term);

            utmp_close();
        } 
    }
}

void sayHi(int fd, char *target)
{
    char helloBuf[BUFLEN] = {0};
    char host[BUFLEN/2] = {0};
    struct passwd *pwdp;
    time_t now;

    if ((pwdp = getpwuid(getuid())) == NULL)
        exit_err("getpwuid", "");

    if (gethostname(host, sizeof(host)) == -1)
        exit_err("gethostname", "");

    now = time(NULL);
    sprintf(helloBuf, "Message from %s@%s on %s, at %s", 
            pwdp->pw_name, host, ttyname(0), ctime(&now)); 

    if (write(fd, helloBuf, strlen(helloBuf)) == -1)
        exit_err("write", target);
}

int main(int argc, char *argv[])
{
    const char *use_fmt = 
        "Usage: %s <dev-name>|<username>.\n \
        e.g  1) %s /dev/pts/10\n \
             2) %s pts/10\n \
             3) %s user1\n";

    if (argc != 2) {
        exit_usage(use_fmt, argv[0], argv[0], argv[0], argv[0]); 
    }

    int fd;
    char term[BUFLEN] = {0};
    char msgBuf[BUFSIZ] = {0};

    getterm(argv[1], term);

    if ((fd = open(term, O_WRONLY)) == -1)
        exit_err("open", term);

    sayHi(fd, term);
        
    while (fgets(msgBuf, sizeof(msgBuf), stdin) != NULL)
        if (write(fd, msgBuf, strlen(msgBuf)) == -1)
            exit_err("write", "");

    close(fd);
    exit(EXIT_SUCCESS);
}
