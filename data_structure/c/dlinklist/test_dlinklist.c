#include <stdio.h>
#include <stdlib.h>
#include "dlinklist.h"

typedef struct {
    DLinkListNode node;
    int val;
}Value;

int main(void)
{
    int i;
    Value v1, v2, v3, v4, v5, v6, v7, v8;
    Value *pv = NULL;

    v1.val = 1;
    v2.val = 2;
    v3.val = 3;
    v4.val = 4;
    v5.val = 5;
    v6.val = 6;
    v7.val = 7;
    v8.val = 8;

    DLinkList *list = DLinkList_Create();
    DLinkList_Insert(list, (DLinkListNode *)&v1, DLinkList_Length(list));
    DLinkList_Insert(list, (DLinkListNode *)&v2, DLinkList_Length(list));
    DLinkList_Insert(list, (DLinkListNode *)&v3, DLinkList_Length(list));
    DLinkList_Insert(list, (DLinkListNode *)&v4, DLinkList_Length(list));
    DLinkList_Insert(list, (DLinkListNode *)&v5, DLinkList_Length(list));
    //DLinkList_Insert(list, (DLinkListNode *)&v6, DLinkList_Length(list));
    //DLinkList_Insert(list, (DLinkListNode *)&v7, DLinkList_Length(list));
    //DLinkList_Insert(list, (DLinkListNode *)&v8, DLinkList_Length(list));

    for (i = 0; i < DLinkList_Length(list); i++) {
        Value *pv = (Value *)DLinkList_Get(list, i); 
        printf("%d\n", pv->val);
    }
    printf("\n");

    DLinkList_Delete(list, DLinkList_Length(list)-1); 
    DLinkList_Delete(list, 0);
    //DLinkList_Delete(list, 3);

    printf("After deleting, dlinklist len = %d\n", DLinkList_Length(list));
    for (i = 0; i < DLinkList_Length(list); i++) {
        pv = (Value *)DLinkList_Get(list, i); 
        printf("%d\n", pv->val);
    }
    printf("\n");

    DLinkList_Reset(list);
    printf("after slider reset ...\n");
    DLinkList_Next(list);
    printf("after slider next ...\n");

    pv = (Value *)DLinkList_Current(list);
    printf("current: %d\n", pv->val);

    DLinkList_DeleteNode(list, (DLinkListNode *)pv);
    printf("after deleting  ....\n");

    pv = (Value *)DLinkList_Current(list);
    printf("slider current: %d\n", pv->val);

    DLinkList_Pre(list);
    printf("after slider pre ...\n");

    pv = (Value *)DLinkList_Current(list);
    printf("slider current: %d\n", pv->val);

    printf("now, dlinklist length: %d\n", DLinkList_Length(list));

    DLinkList_Destroy(list);

    return 0;
}

