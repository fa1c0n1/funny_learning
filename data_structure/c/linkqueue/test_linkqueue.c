#include <stdio.h>
#include "linkqueue.h"

int main(void)
{
    int i, a[10];

    LinkQueue *queue = LinkQueue_Create();
    if (queue == NULL) {
        return -1; 
    }

    for (i = 0; i < 5; i++) {
        a[i] = 60 + i;
        LinkQueue_Append(queue, &a[i]);
    }

    printf("queue len: %d\n", LinkQueue_Length(queue));
    printf("queue header: %d\n", *((int *)LinkQueue_Header(queue)));

    while (LinkQueue_Length(queue) > 0) {
        int tmp = *((int *)LinkQueue_Retrieve(queue)); 
        printf("tmp: %d\n", tmp);
    }

    LinkQueue_Destroy(queue);

    return 0;
}
