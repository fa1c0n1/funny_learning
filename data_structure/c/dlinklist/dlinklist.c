#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlinklist.h"

typedef struct {
    DLinkListNode header;
    DLinkListNode *slider;
    int length;
}TDLinkList;

DLinkList *DLinkList_Create(void)
{
    TDLinkList *tDLinkList = NULL;

    tDLinkList = (TDLinkList *)calloc(1, sizeof(TDLinkList));
    if (tDLinkList == NULL) {
        fprintf(stderr, "calloc error.\n");
        return NULL;/////
    }

    tDLinkList->header.pre = NULL;
    tDLinkList->header.next = NULL;
    tDLinkList->slider = NULL;
    tDLinkList->length = 0;
}

void DLinkList_Destroy(DLinkList *list)
{
    if (list != NULL) {
        free(list);
        list = NULL;
    }
}

void DLinkList_Clear(DLinkList *list)
{
    TDLinkList *tDLinkList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return ;
    }

    tDLinkList = (TDLinkList *)list;
    tDLinkList->header.pre = NULL;
    tDLinkList->header.next = NULL;
    tDLinkList->slider = NULL;
    tDLinkList->length = 0;
}

int DLinkList_Length(DLinkList *list)
{
    TDLinkList *tDLinkList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return ;
    }

    tDLinkList = (TDLinkList *)list;
    return tDLinkList->length;
}

int DLinkList_Insert(DLinkList *list, DLinkListNode *node, int pos)
{
    int i;
    TDLinkList *tDLinkList = NULL;
    DLinkListNode *pCurNode = NULL;
    DLinkListNode *pNextNode = NULL;

    if (list == NULL || node == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n");
        return -1;
    }

    tDLinkList = (TDLinkList *)list;
    pCurNode = (DLinkListNode *)tDLinkList; 
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }
    pNextNode = pCurNode->next;
    node->next = pNextNode;
    pCurNode->next = node;

    node->pre = pCurNode;
    if (pNextNode != NULL) {
        pNextNode->pre = node;
    }

    if (tDLinkList->length == 0) {
        tDLinkList->slider = node; 
    }

    tDLinkList->length++;

    ////////////insert node in position 0///////////////
    if (pCurNode == (DLinkListNode *)tDLinkList) {
        node->pre = NULL;
    }
    ////////////////////////////////

    return 0;
}

DLinkListNode *DLinkList_Get(DLinkList *list, int pos)
{
    int i;
    TDLinkList *tDLinkList = NULL;
    DLinkListNode *pCurNode = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    } 

    tDLinkList = (TDLinkList *)list;
    pCurNode = (DLinkListNode *)tDLinkList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }

    return pCurNode->next;
}

DLinkListNode *DLinkList_Delete(DLinkList *list, int pos)
{
    int i;
    TDLinkList *tDLinkList = NULL;
    DLinkListNode *pCurNode = NULL;
    DLinkListNode *pNextNode = NULL;
    DLinkListNode *pDelNode = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    } 

    tDLinkList = (TDLinkList *)list;
    if (tDLinkList->length == 0) {
        fprintf(stderr, "The dlinklist's length is zero.\n");
        return NULL; 
    }

    if (pos > tDLinkList->length-1) {
        fprintf(stderr, "The position is out of bound.\n");
        return NULL;
    }

    pCurNode = (DLinkListNode *)tDLinkList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }
    pDelNode = pCurNode->next;  
    pCurNode->next = pDelNode->next;
    pNextNode = pCurNode->next;
    if (pNextNode != NULL) {
        pNextNode->pre = pCurNode;
    }
    
    /////////// delete first node ////////////
    if (pCurNode == (DLinkListNode *)tDLinkList) {
        pNextNode->pre = NULL;
    }
    //////////////////////////
    
    tDLinkList->length--;

    if (pDelNode == tDLinkList->slider) {
        tDLinkList->slider = pNextNode;      
    }
    
    if (tDLinkList->length == 0) {
        tDLinkList->header.next = NULL;
        tDLinkList->slider = NULL;
    }

    pDelNode->pre = NULL;
    pDelNode->next = NULL;
    return pDelNode;
}

DLinkListNode *DLinkList_Reset(DLinkList *list)
{
    TDLinkList *tDLinkList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tDLinkList = (TDLinkList *)list;
    tDLinkList->slider = tDLinkList->header.next;

    return tDLinkList->slider;
}

DLinkListNode *DLinkList_Current(DLinkList *list)
{
    TDLinkList *tDLinkList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tDLinkList = (TDLinkList *)list;

    return tDLinkList->slider;
}

DLinkListNode *DLinkList_Pre(DLinkList *list)
{
    TDLinkList *tDLinkList = NULL;
    DLinkListNode *sliderCur = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tDLinkList = (TDLinkList *)list;
    if (tDLinkList->slider != NULL) {
        sliderCur = tDLinkList->slider; 
        tDLinkList->slider = sliderCur->pre;
    }

    return sliderCur;
}

DLinkListNode *DLinkList_Next(DLinkList *list)
{
    TDLinkList *tDLinkList = NULL;
    DLinkListNode *sliderCur = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tDLinkList = (TDLinkList *)list;
    if (tDLinkList->slider != NULL) {
        sliderCur = tDLinkList->slider;
        tDLinkList->slider = sliderCur->next;
    }

    return sliderCur;
}

DLinkListNode *DLinkList_DeleteNode(DLinkList *list, DLinkListNode *node)
{
    int i;
    TDLinkList *tDLinkList = NULL;
    DLinkListNode *pCurNode = NULL;
    DLinkListNode *pDelNode = NULL;

    if (list == NULL || node == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tDLinkList = (TDLinkList *)list;
    pCurNode = (DLinkListNode *)tDLinkList; 
    for (i = 0; i < tDLinkList->length; i++) {
         if (node == pCurNode->next) {
                pDelNode = pCurNode->next; 
                break;
         }  
         pCurNode = pCurNode->next;
    }

    if (pDelNode != NULL) {
        DLinkList_Delete(list, i); 
    }

    return pDelNode;
}

