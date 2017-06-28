#include <stdio.h>
#include <stdlib.h>
#include "circlelist.h"

typedef struct {
    CircleListNode node;
    int val;
}Value;

int main(void)
{
    int i;
    Value v1, v2, v3, v4, v5, v6, v7, v8;

    v1.val = 1;
    v2.val = 2;
    v3.val = 3;
    v4.val = 4;
    v5.val = 5;
    v6.val = 6;
    v7.val = 7;
    v8.val = 8;

    CircleList *list = CircleList_Create();
    CircleList_Insert(list, (CircleListNode *)&v1, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v2, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v3, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v4, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v5, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v6, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v7, CircleList_Length(list));
    CircleList_Insert(list, (CircleListNode *)&v8, CircleList_Length(list));

    for (i = 0; i < 2*CircleList_Length(list); i++) {
        Value *pv = (Value *)CircleList_Get(list, i); 
        printf("%d\n", pv->val);
    }

    while (CircleList_Length(list) > 3) {
        CircleList_Delete(list, 0); 
    }
    printf("After deleting, circlelist len = %d\n", CircleList_Length(list));
    for (i = 0; i < 2*CircleList_Length(list); i++) {
        Value *pv = (Value *)CircleList_Get(list, i); 
        printf("%d\n", pv->val);
    }

    CircleList_Destroy(list);

    return 0;
}

