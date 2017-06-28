#include <stdio.h>
#include "seqlist.h"

typedef struct {
    int age;
    char name[64];
}Teacher;

int main(void)
{
    int i;
    SeqList* list = NULL;

    Teacher t1, t2, t3, t4, t5;
    t1.age = 31;
    t2.age = 32;
    t3.age = 33;
    t4.age = 34;
    t5.age = 35;

    list = SeqList_Create(10);
    if (list == NULL) {
        fprintf(stderr, "SeqList_Create error.\n");
        return -1;
    }

    SeqList_Insert(list, (SeqListNode *)&t1, 0);
    SeqList_Insert(list, (SeqListNode *)&t2, 0);
    SeqList_Insert(list, (SeqListNode *)&t3, 0);
    SeqList_Insert(list, (SeqListNode *)&t4, 0);
    SeqList_Insert(list, (SeqListNode *)&t5, 0);

    for (i = 0; i < SeqList_Length(list); i++) {
        Teacher *tmp = (Teacher *)SeqList_Get(list, i); 
        if (tmp == NULL) {
            return -1; 
        }
        printf("Teacher-%d age: %d\n", i, tmp->age);
    }

    while (SeqList_Length(list) > 0) {
        SeqList_Delete(list, 0); 
    }

    return 0;
}
