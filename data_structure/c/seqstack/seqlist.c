#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "seqlist.h"

typedef struct {
    int capacity;
    int length;
    SeqListNode **node;     //SeqListNode *node[]
}TSeqList;


SeqList* SeqList_Create(int capacity)
{
    TSeqList *tSeqList = (TSeqList *)calloc(1, sizeof(TSeqList));
    if (tSeqList == NULL) {
        fprintf(stderr, "calloc failed: %s\n", strerror(errno));
        return NULL;
    }

    tSeqList->node = (SeqListNode **)calloc(capacity, sizeof(SeqListNode *));
    if (tSeqList->node == NULL) {
        fprintf(stderr, "calloc failed: %s\n", strerror(errno));
        return NULL;
    }

    tSeqList->capacity = capacity;
    tSeqList->length = 0;

    return tSeqList;
}

void SeqList_Destroy(SeqList *list)
{
    TSeqList *tSeqList = NULL;
    if (list == NULL) {
        return ; 
    }

    tSeqList = (TSeqList *)list;
    if (tSeqList->node != NULL) {
        free(tSeqList->node);
    }
    free(tSeqList);
}

void SeqList_Clear(SeqList *list)
{
    TSeqList *tSeqList = NULL;
    if (list == NULL) {
        return ; 
    }

    tSeqList = (TSeqList *)list;
    tSeqList->length = 0;
}

int SeqList_Length(SeqList *list)
{
    TSeqList *tSeqList = NULL;
    if (list == NULL) {
        return ; 
    }

    tSeqList = (TSeqList *)list;
    return tSeqList->length;
}

int SeqList_Capacity(SeqList *list)
{
    TSeqList *tSeqList = NULL;
    if (list == NULL) {
        return ; 
    }

    tSeqList = (TSeqList *)list;
    return tSeqList->capacity;
}

int SeqList_Insert(SeqList *list, SeqListNode *node, int pos)
{
    int i, ret = 0;

    if (list == NULL || node == NULL || pos < 0) {
        fprintf(stderr, "Invalid param.\n");
        ret = -1;
        goto END;
    }

    TSeqList *tSeqList = (TSeqList *)list;
    if (tSeqList->length >= tSeqList->capacity) {
        fprintf(stderr, "The capacity of seqlist is fully occupied\n"); 
        ret = -2;
        goto END;
    }

    if (pos >= tSeqList->capacity) {
        fprintf(stderr, "The param pos is bigger than capacity.\n"); 
        ret = -3;
        goto END;
    }

    //e.g, length=7, capacity=15, but pos=10
    if (pos > tSeqList->length && pos < tSeqList->capacity) {
        pos = tSeqList->length; 
    }

    for (i = tSeqList->length; i > pos; i--) {
        tSeqList->node[i] = tSeqList->node[i-1]; 
    }
    tSeqList->node[pos] = node;
    tSeqList->length++;

END:
    return ret;
}

SeqListNode *SeqList_Get(SeqList *list, int pos)
{
    SeqListNode *tNode = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid param.\n");
        goto END;
    }

    TSeqList *tSeqList = (TSeqList *)list;
    if (pos >= tSeqList->capacity) {
        fprintf(stderr, "The param pos is bigger than capacity.\n"); 
        goto END;
    }

    tNode = tSeqList->node[pos];

END:
    return tNode;
}

SeqListNode *SeqList_Delete(SeqList *list, int pos)
{
    int i;
    SeqListNode *tNode = NULL;
    TSeqList *tSeqList = NULL;

    if (list == NULL || pos < 0) {
        fprintf(stderr, "Invalid param.\n");
        goto END;
    }

    tSeqList = (TSeqList *)list;
    if (pos >= tSeqList->length) {
        fprintf(stderr, "The param pos is bigger than length.\n"); 
        goto END;
    }

    tNode = (SeqListNode *)tSeqList->node[pos];
    for (i = pos+1; i < tSeqList->length ; i++) {
        tSeqList->node[i-1] = tSeqList->node[i]; 
    }
    tSeqList->length--;

END:
    return tNode;
}
