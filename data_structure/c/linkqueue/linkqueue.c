#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkqueue.h"
#include "linklist.h"

typedef struct {
    LinkListNode header;    
    void *item;
}TQueueNode;

LinkQueue *LinkQueue_Create()
{
    return LinkList_Create();
}

void LinkQueue_Destroy(LinkQueue *queue)
{
    LinkQueue_Clear(queue);
    LinkList_Destroy(queue);
}

void LinkQueue_Clear(LinkQueue *queue)
{
    while (LinkQueue_Length(queue) > 0) {
        LinkQueue_Retrieve(queue); 
    }
    
    LinkList_Clear(queue);
}

int LinkQueue_Append(LinkQueue *queue, void *item)
{
    int ret;
    TQueueNode *tNode = (TQueueNode *)calloc(1, sizeof(TQueueNode));
    if (tNode == NULL) {
        fprintf(stderr, "calloc error.\n"); 
        return -1;
    }

    tNode->item = item;
    ret = LinkList_Insert(queue, (LinkListNode *)tNode, LinkList_Length(queue));
    if (ret != 0) {
        fprintf(stderr, "LinkList_Insert error.\n"); 
        if (tNode != NULL) {
            free(tNode);    
        }
    }

    return ret;
}

void *LinkQueue_Retrieve(LinkQueue *queue)
{
    TQueueNode *tNode = (TQueueNode *)LinkList_Delete(queue, 0);
    void *tItem = NULL;

    if (tNode != NULL) {
        tItem = tNode->item; 
        free(tNode);
        return tItem;
    }

    return NULL;
}

void *LinkQueue_Header(LinkQueue *queue)
{
    TQueueNode *tNode = (TQueueNode *)LinkList_Get(queue, 0);
    void *tItem = NULL;

    if (tNode != NULL) {
        tItem = tNode->item; 
        return tItem;
    }

    return NULL;
}

int LinkQueue_Length(LinkQueue *queue)
{
    return LinkList_Length(queue);
}
