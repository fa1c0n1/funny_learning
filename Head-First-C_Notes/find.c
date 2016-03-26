#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NUM_ADS = 7;
char *ADS[] = {
    "William: SBM GSOH likes sports, TV, dining",
    "Matt: SWM NS likes art, movies, theater",
    "Luis: SLM ND likes books, theater, art",
    "Mike: DWM DS likes trucks, sports and bieber",
    "Peter: SAM likes chess, working out and art",
    "Josh: SJM likes sports, movies and theater",
    "Jed: DBM likes theater, books and dining"
};

// 喜欢运动但不喜欢Bieber
int sports_no_bieber(char *s)
{
    return strstr(s, "sports") && !strstr(s, "bieber");
}

//喜欢运动或锻炼身体
int sports_or_workout(char *s)
{
    return strstr(s, "sports") || strstr(s, "working out");
}

//喜欢戏剧而且不抽烟
int ns_theater(char *s)
{
    return strstr(s, "theater") && strstr(s, "NS");
}

//喜欢艺术、戏剧或美食
int arts_theater_or_dining(char *s)
{
    return strstr(s, "art") || strstr(s, "theater") || strstr(s, "dining");
}

void find(int (*match)(char *))
{
    int i;
    puts("Search results: ");
    puts("--------------------------------");

    for (i = 0; i < NUM_ADS; i++) {
        if (match(ADS[i])) {
            printf("%s\n", ADS[i]);
        }
    }
    puts("--------------------------------");
}

int main(void)
{
    find(sports_no_bieber);
    find(sports_or_workout);
    find(ns_theater);
    find(arts_theater_or_dining);

    return 0;
}
