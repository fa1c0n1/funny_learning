#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist.h"

typedef struct linkList {
    LinkListNode header;
    int length;
}TLinkList;

LinkList *LinkList_Create(void)
{
    TLinkList *tLinkList = NULL;
    tLinkList = (TLinkList *)calloc(1, sizeof(TLinkList));
    tLinkList->length = 0;
    tLinkList->header.next = NULL;

    return tLinkList;
}

void LinkList_Destroy(LinkList *list)
{
    if (list == NULL) {
        fprintf(stderr, "Invalid param.\n");
        return ; 
    }

    free(list);
    list = NULL;
}

void LinkList_Clear(LinkList *list)
{
    TLinkList *tLinkList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid param.\n");
        return ; 
    }

    tLinkList = (TLinkList *)list;
    tLinkList->length = 0;
    tLinkList->header.next = NULL;
}

int LinkList_Length(LinkList *list)
{
    TLinkList *tLinkList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n"); 
        return -1;
    }

    tLinkList = (TLinkList *)list;
    return tLinkList->length;
}

int LinkList_Insert(LinkList *list, LinkListNode *node, int pos)
{
    int i;
    TLinkList *tLinkList = NULL;

    if (list == NULL || node == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n"); 
        return -1;
    }

    tLinkList = (TLinkList *)list;
    LinkListNode *pCurNode = (LinkListNode *)tLinkList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }
    node->next = pCurNode->next;
    pCurNode->next = node;
    tLinkList->length++;

    return 0;
}

LinkListNode *LinkList_Get(LinkList *list, int pos)
{
    int i;
    TLinkList *tLinkList = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n"); 
        return NULL;
    }

    tLinkList = (TLinkList *)list;
    LinkListNode *pCurNode = (LinkListNode *)tLinkList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }

    return pCurNode->next;
}

LinkListNode *LinkList_Delete(LinkList *list, int pos)
{
    int i;
    TLinkList *tLinkList = NULL;
    LinkListNode *delNode = NULL;
    LinkListNode *tNode = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n"); 
        return NULL;
    }

    tLinkList = (TLinkList *)list;
    if (LinkList_Length(list) == 0) {
        fprintf(stderr, "LinkList's length is zero.\n"); 
        return NULL; 
    }
    LinkListNode *pCurNode = (LinkListNode *)tLinkList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }
    delNode = pCurNode->next;
    tNode = delNode->next;
    pCurNode->next = tNode;
    tLinkList->length--;

    return delNode;
}
