#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define LINELEN    512

void do_more(FILE *);
int see_more(FILE *);

static int page_len;

static int get_pagelen(void)
{
    struct winsize wbuf;

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wbuf) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    return wbuf.ws_row;
}

int main(int argc, char *args[])
{
    FILE *fp;

    page_len = get_pagelen() - 1;

    if (argc == 1) {
        do_more(stdin); 
    } else {
        while (--argc) {
            if ((fp = fopen(*(++args), "r")) != NULL) {
                do_more(fp);
                fclose(fp); 
            } else {
                fprintf(stderr, "fopen %s failure: %s\n", *(--args), strerror(errno));
                exit(1);
            }
        } 
    }

    return 0;
}

void do_more(FILE *fp)
{
    int num_of_line = 0, reply;
    char line[LINELEN];
    FILE *fp_tty;

    fp_tty = fopen("/dev/tty", "r");
    if (fp_tty == NULL) {
        fprintf(stderr, "fopen %s failure: %s\n", "/dev/tty", strerror(errno)); 
        exit(1);
    }

    while (fgets(line, LINELEN, fp)) {
        if (num_of_line == page_len) {
            reply = see_more(fp_tty); 
            if (reply == 0) {
                break;
            }
            num_of_line -= reply;
        } 

        if (fputs(line, stdout) == EOF) {
            perror("fputs failure"); 
            exit(1);
        }

        num_of_line++;
    }
}

int see_more(FILE *cmd)
{
    int ch;

    printf("\033[7m--more--\033[m");
    if ((ch = getc(cmd)) != EOF) {
        if (ch == 'q')
            return 0;
        if (ch == ' ')
            return page_len;
        if (ch == '\n')
            return 1;
    }

    return 0;
}
