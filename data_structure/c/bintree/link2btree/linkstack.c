#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkstack.h"
#include "linklist.h"

typedef struct {
    LinkListNode node;
    void *item;
}TStackNode;

LinkStack *LinkStack_Create()
{
    return LinkList_Create();
}

void LinkStack_Destroy(LinkStack *stack)
{
    LinkStack_Clear(stack);
    LinkList_Destroy(stack);
}

void LinkStack_Clear(LinkStack *stack)
{
    TStackNode *tStackNode = NULL;
    void *tItem = NULL;

    if (stack == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return ;  
    }

    while (LinkList_Length(stack) > 0) {
        LinkStack_Pop(stack);
    }

    LinkList_Clear(stack);
}

int LinkStack_Push(LinkStack *stack, void *item)
{
    int ret;
    TStackNode *tStackNode = (TStackNode *)calloc(1, sizeof(TStackNode));
    if (tStackNode == NULL) {
        return -1; 
    }

    tStackNode->item = item;
    ret = LinkList_Insert(stack, (LinkListNode *)tStackNode, 0);
    if (ret != 0) {
        fprintf(stderr, "LinkList_Insert error: %d\n", ret);
        if (tStackNode != NULL) {
            free(tStackNode);
        }
    }

    return ret;
}

void *LinkStack_Pop(LinkStack *stack)
{
    TStackNode *tStackNode = (TStackNode *)LinkList_Delete(stack, 0);
    void *tItem = NULL;

    if (tStackNode != NULL) {
        tItem = tStackNode->item;
        free(tStackNode);
        return tItem; 
    }
    return NULL;
}

void *LinkStack_Top(LinkStack *stack)
{
    TStackNode *tStackNode = (TStackNode *)LinkList_Get(stack, 0);

    if (tStackNode != NULL) {
        return tStackNode->item; 
    }
    return NULL;
}

int LinkStack_Size(LinkStack *stack)
{
    return LinkList_Length(stack);
}
