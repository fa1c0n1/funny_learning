#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    srand((int)time(0));
    int i, j;
//    j = 0 + (int)(4.0 * rand() / (RAND_MAX+ 1.0)); //[0-3]随机
    j = 1 + (int)(4.0 * rand() / (RAND_MAX+ 1.0)); //[1-4]随机
    printf("rand=%d\n", j);

    return 0;
}
