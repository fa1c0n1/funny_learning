#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 升序排列整型得分
int compare_scores(const void *score_a, const void *score_b)
{
    int a = *(int *)score_a;
    int b = *(int *)score_b;

    return a - b;
}

// 降序排列整型得分
int compare_scores_desc(const void *score_a, const void *score_b)
{
    int a = *(int *)score_a;
    int b = *(int *)score_b;

    return b - a;
}

// 按面积从小到大排列矩形
typedef struct {
    int width;
    int height;
} rectangle;

int compare_areas(const void *a, const void *b)
{
    rectangle *arec = (rectangle *)a;
    rectangle *brec = (rectangle *)b;

    return arec->width * arec->height - brec->width * brec->height;
}

// 按字母序排列名字, 区分大小写
int compare_names(const void *a, const void *b)
{
    char **name_a = (char **)a;
    char **name_b = (char **)b;

    return strcmp(*name_a, *name_b);
}

// 按面积从大到小排列矩形
int compare_areas_desc(const void *a, const void *b)
{
    return -compare_areas(a, b);
}

// 按逆字母序排列名字，区分大小写
int compare_names_desc(const void *a, const void *b)
{
    return -compare_names(a, b);
}

int main(void)
{
    int scores[] = {543, 323, 32, 554, 11, 3, 112};
    int i;

    // 对得分进行排序
    qsort(scores, 7, sizeof(int), compare_scores_desc);
    puts("These are the scores in order:");
    for (i = 0; i < 7; i++) {
        printf("Score[%d] = %d\n", i,  scores[i]);
    }

    char *names[] = {"Karen", "Mark", "Brett", "Molly"};
    qsort(names, 4, sizeof(char *), compare_names);
    puts("These are the names in order:");
    for (i = 0; i < 4; i++) {
        printf("%s\n", names[i]);
    }

    return 0;
}
