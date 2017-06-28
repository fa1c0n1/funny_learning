#include <stdio.h>
#include "linkstack.h"

int main(void)
{
    int i;
    int a[10];

    LinkStack *stack = LinkStack_Create();
    if (stack == NULL) {
        return ; 
    }

    for (i = 0; i < 5; i++) {
        a[i] = i + 1; 
        LinkStack_Push(stack, &a[i]);
    }

    printf("size: %d\n", LinkStack_Size(stack));
    printf("top: %d\n", *((int *)LinkStack_Top(stack)));
     
    while (LinkStack_Size(stack) > 0) {
        int tmp = *((int *)LinkStack_Pop(stack)); 
        printf("tmp: %d\n", tmp);
    }

    LinkStack_Destroy(stack);

    return 0;
}
