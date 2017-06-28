#include <stdio.h>
#include "seqstack.h"

int main(void)
{
    int i;
    int a[10];
    SeqStack *stack = NULL;

    stack = SeqStack_Create(10);
    if (stack == NULL) {
        return -1; 
    }

    for (i = 0; i < 5; i++) {
        a[i] = i+101;
        SeqStack_Push(stack, (void *)&a[i]); 
    }

    printf("Capacity: %d\n", SeqStack_Capacity(stack));
    printf("Size: %d\n", SeqStack_Size(stack));
    printf("Top: %d\n", *((int *)SeqStack_Top(stack)));

    while (SeqStack_Size(stack) > 0) {
        int tmp = *((int *)SeqStack_Pop(stack));
        printf("tmp: %d\n", tmp);
    }

    printf("Size: %d\n", SeqStack_Size(stack));

    return 0;
}
