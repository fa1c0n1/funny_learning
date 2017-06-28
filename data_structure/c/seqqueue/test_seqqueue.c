#include <stdio.h>
#include "seqqueue.h"

int main(void)
{
    int i;
    int a[10];

    SeqQueue *queue = SeqQueue_Create(10);
    if (queue == NULL) {
        return -1; 
    }

    for (i = 0; i < 5; i++) {
        a[i] = i + 1; 
        SeqQueue_Append(queue, (void *)&a[i]);
    }

    printf("queue len: %d\n", SeqQueue_Length(queue));
    printf("queue header: %d\n", *((int *)SeqQueue_Header(queue)));
    printf("queue capacity: %d\n", SeqQueue_Capacity(queue));

    while (SeqQueue_Length(queue) > 0) {
        int tmp = *((int *)SeqQueue_Retrieve(queue)); 
        printf("tmp: %d\n", tmp);
    }
    
    printf("After retrieving, queue len: %d\n", SeqQueue_Length(queue));

    SeqQueue_Destroy(queue);

    return 0;
}
