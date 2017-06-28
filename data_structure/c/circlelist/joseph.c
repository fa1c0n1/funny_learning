#include <stdio.h>
#include <stdlib.h>
#include "circlelist.h"

typedef struct {
    CircleListNode node;
    int val;
}Value;

#define COUNT    3

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

    Value *pv = NULL;
    while (CircleList_Length(list) > 0) {
        for (i = 1; i < COUNT; i++) {
            CircleList_Next(list);
        } 
        
        pv = (Value *)CircleList_Current(list);
        if (pv != NULL) {
            printf("%d ", pv->val); 
            CircleList_DeleteNode(list, (CircleListNode *)pv);
        }
    } 
    printf("\n");

    CircleList_Destroy(list);

    return 0;
}

