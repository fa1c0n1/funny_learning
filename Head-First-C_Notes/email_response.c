#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum response_type {
    DUMP, SECOND_CHANCE, MARRIAGE
};

typedef struct {
    char *name;
    enum response_type type;
} response;

void dump(response r)
{
    printf("Dear %s,\n", r.name);
    puts("Unfortunately your last date contacted us to");
    puts("say that they will not be seeing you again");
}

void second_chance(response r)
{
    printf("Dear %s,\n", r.name);
    puts("Good news: your last date has asked us to");
    puts("arrange another meeting. Please call ASAP.");
}

void marriage(response r)
{
    printf("Dear %s,\n", r.name);
    puts("Congratulations! Your last date has contacted");
    puts("us with a proposal of marriage.");
}

int main(void)
{
    response r[] = {
        {"Mike", DUMP}, {"Luis", SECOND_CHANCE},
        {"Matt", SECOND_CHANCE}, {"William", MARRIAGE}
    };

    int i;
    //for (i = 0; i < 4; i++) {
    //    switch(r[i].type) {
    //    case DUMP:
    //        dump(r[i]);
    //        break;       
    //    case SECOND_CHANCE:
    //        second_chance(r[i]);
    //        break;       
    //    default:
    //        marriage(r[i]);

    //    }
    //}


    //创建函数指针数组，且函数指针在数组中的顺序与上面的枚举相同
    void (*replies[])(response) = {dump, second_chance, marriage};

    for (i = 0; i < sizeof(r) / sizeof(response); i++) {
        replies[r[i].type](r[i]);
    }

    return 0;
}
