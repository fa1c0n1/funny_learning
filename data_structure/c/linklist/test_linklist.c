#include <stdio.h>
#include "linklist.h"

typedef struct {
    LinkListNode node;
    int age;
    char name[64];
}Teacher;

int main(void)
{
    int i, len = 0, ret = 0; 

    LinkList *list = NULL;
    Teacher t1, t2, t3, t4, t5;
    t1.age = 31;
    t2.age = 32;
    t3.age = 33;
    t4.age = 34;
    t5.age = 35;

    list = LinkList_Create();
    if (list == NULL) {
        return ; 
    }

    ret = LinkList_Insert(list, (LinkListNode *)&t1, 0);
    ret = LinkList_Insert(list, (LinkListNode *)&t2, 0);
    ret = LinkList_Insert(list, (LinkListNode *)&t3, 0);
    ret = LinkList_Insert(list, (LinkListNode *)&t4, 0);
    ret = LinkList_Insert(list, (LinkListNode *)&t5, 0);

    for (i = 0; i < LinkList_Length(list); i++) {
        Teacher *tmp = (Teacher *)LinkList_Get(list, i); 
        if (tmp == NULL) {
            return -1; 
        }
        printf("LinkList_Get--->  tmp->age: %d\n", tmp->age);
    }

    while (LinkList_Length(list) > 0) {
        Teacher *tmp = (Teacher *)LinkList_Delete(list, 0); 
        if (tmp == NULL) {
            return -1; 
        }
        printf("LinkList_Delete--->  tmp->age: %d\n", tmp->age);
    }
    printf("After deleting, linklist len = %d\n", len);
    
    LinkList_Destroy(list);

    return 0;
}

