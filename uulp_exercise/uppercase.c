#include "errhandle.h"
#include <termios.h>
#include <unistd.h>

int main(void)
{
    struct termios tios;
    char *msg;

    if (tcgetattr(STDIN_FILENO, &tios) == -1) 
        exit_err("tcgetattr", "stdin");

    if (tios.c_oflag & OLCUC) {
        tios.c_oflag &= ~OLCUC; 
        msg = "OFF";
    } else {
        tios.c_oflag |= OLCUC; 
        msg = "ON";
    }

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tios) == -1)
        exit_err("tcsetattr", "stdin");

    printf("OLCUC is now %s.\n", msg);

    exit(EXIT_SUCCESS);
}
