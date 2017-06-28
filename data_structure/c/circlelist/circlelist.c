#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circlelist.h"

typedef struct {
    CircleListNode header;
    CircleListNode *slider;
    int length;
}TCircleList;

CircleList *CircleList_Create(void)
{
    TCircleList *tCircleList = NULL;

    tCircleList = (TCircleList *)calloc(1, sizeof(TCircleList));
    if (tCircleList == NULL) {
        fprintf(stderr, "calloc error.\n");
        return NULL;
    }

    tCircleList->header.next = NULL;
    tCircleList->slider = NULL;
    tCircleList->length = 0;

    return tCircleList;
}

void CircleList_Destroy(CircleList *list)
{
    if (list != NULL) {
        free(list);
        list = NULL;
    }
}

void CircleList_Clear(CircleList *list)
{
    TCircleList *tCircleList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return ;
    }

    tCircleList = (TCircleList *)list;
    tCircleList->header.next = NULL;
    tCircleList->slider = NULL;
    tCircleList->length = 0;
}

int CircleList_Length(CircleList *list)
{
    TCircleList *tCircleList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return ;
    }

    tCircleList = (TCircleList *)list;
    return tCircleList->length;
}

int CircleList_Insert(CircleList *list, CircleListNode *node, int pos)
{
    int i;
    TCircleList *tCircleList = NULL;
    CircleListNode *pCurNode = NULL;

    if (list == NULL || node == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n");
        return -1;
    }

    tCircleList = (TCircleList *)list;
    pCurNode = (CircleListNode *)tCircleList; 
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }
    node->next = pCurNode->next;
    pCurNode->next = node;

    if (tCircleList->length == 0) {
        tCircleList->slider = node; 
    }

    tCircleList->length++;

    ////////////////////////////////
    if (pCurNode == (CircleListNode *)tCircleList) {
        CircleListNode *pLastNode = CircleList_Get(list, CircleList_Length(list)-1); 
        pLastNode->next = pCurNode->next;
    }
    ////////////////////////////////

    return 0;
}

CircleListNode *CircleList_Get(CircleList *list, int pos)
{
    int i;
    TCircleList *tCircleList = NULL;
    CircleListNode *pCurNode = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    } 

    tCircleList = (TCircleList *)list;
    pCurNode = (CircleListNode *)tCircleList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }

    return pCurNode->next;
}

CircleListNode *CircleList_Delete(CircleList *list, int pos)
{
    int i;
    TCircleList *tCircleList = NULL;
    CircleListNode *pCurNode = NULL;
    CircleListNode *pDelNode = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    } 

    tCircleList = (TCircleList *)list;
    if (tCircleList->length == 0) {
        fprintf(stderr, "The circlelist's length is zero.\n");
        return NULL; 
    }

    if (pos > tCircleList->length-1) {
        fprintf(stderr, "The position is out of bound.\n");
        return NULL;
    }

    pCurNode = (CircleListNode *)tCircleList;
    for (i = 0; i < pos; i++) {
        pCurNode = pCurNode->next; 
    }
    pDelNode = pCurNode->next;  
    /////////// delete first node ////////////
    if (pCurNode == (CircleListNode *)tCircleList) {
        CircleListNode *pLastNode = CircleList_Get(list, CircleList_Length(list)-1);
        pLastNode->next = pDelNode->next; 
    }
    //////////////////////////
    pCurNode->next = pDelNode->next;
    
    tCircleList->length--;

    if (pDelNode == tCircleList->slider) {
        tCircleList->slider = pDelNode->next;      
    }
    
    if (tCircleList->length == 0) {
        tCircleList->header.next = NULL;
        tCircleList->slider = NULL;
    }

    pDelNode->next = NULL;
    return pDelNode;
}

CircleListNode *CircleList_Reset(CircleList *list)
{
    TCircleList *tCircleList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tCircleList = (TCircleList *)list;
    tCircleList->slider = tCircleList->header.next;

    return tCircleList->slider;
}

CircleListNode *CircleList_Current(CircleList *list)
{
    TCircleList *tCircleList = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tCircleList = (TCircleList *)list;

    return tCircleList->slider;
}

CircleListNode *CircleList_Next(CircleList *list)
{
    TCircleList *tCircleList = NULL;
    CircleListNode *sliderCur = NULL;

    if (list == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tCircleList = (TCircleList *)list;
    if (tCircleList->slider != NULL) {
        sliderCur = tCircleList->slider;
        tCircleList->slider = sliderCur->next;
    }

    return sliderCur;
}

CircleListNode *CircleList_DeleteNode(CircleList *list, CircleListNode *node)
{
    int i;
    TCircleList *tCircleList = NULL;
    CircleListNode *pCurNode = NULL;
    CircleListNode *pDelNode = NULL;

    if (list == NULL || node == NULL) {
        fprintf(stderr, "Invalid params.\n");
        return NULL;
    }

    tCircleList = (TCircleList *)list;
    pCurNode = (CircleListNode *)tCircleList; 
    for (i = 0; i < tCircleList->length; i++) {
         if (node == pCurNode->next) {
                pDelNode = pCurNode->next; 
                break;
         }  
         pCurNode = pCurNode->next;
    }

    if (pDelNode != NULL) {
        CircleList_Delete(list, i); 
    }

    return pDelNode;
}

