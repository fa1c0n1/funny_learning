#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

int score = 0;

void end_game(int sig)
{
    printf("\nFinal score: %d\n", score);
    exit(0);
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    return sigaction(sig, &action, NULL);
}

void times_up(int sig)
{
    puts("\nTIME'S UP!");
    raise(SIGINT);
}

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(void)
{
    catch_signal(SIGALRM, times_up);
    catch_signal(SIGINT, end_game);
    srandom(time(0)); //确保每次都得到不同的随机数

    while(1) {
        int a = random() % 11; //a,b均是0到10的随机数
        int b = random() % 11;
        char txt[4];

        alarm(5);
        printf("\nWhat is %d times %d? ", a, b);
        fgets(txt, 4, stdin);
        int answer = atoi(txt);
        if (answer == a * b) {
            score++;
        } else {
            printf("\nWrong! Score: %d\n", score);
        }
    }

    return 0;
}

