#include "errhandle.h"
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
    struct passwd *pwdp;
    char *username;
    uid_t uid;

    if (argc < 3) {
        exit_usage("%s <username> [FILE1] [FILE2]...\n", argv[0]);
    }

    username = *(++argv);
    
    if ((pwdp = getpwnam(username)) == NULL) {
        exit_err("getpwnam", username); 
    }
    uid = pwdp->pw_uid;

    while (*(++argv) != NULL) {
        if (chown(*argv, uid, -1) == -1) {
            exit_err("chown", *argv); 
        } 
    }
    
    exit(EXIT_SUCCESS);
}
