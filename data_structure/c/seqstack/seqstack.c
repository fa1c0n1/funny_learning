#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "seqstack.h"
#include "seqlist.h"

//Creating SeqStack is the same as creating SeqList
SeqStack *SeqStack_Create(int capacity)
{
    return SeqList_Create(capacity);
}

//Destroying SeqStack is the same as destroying SeqList 
void SeqStack_Destroy(SeqStack *stack)
{
    SeqList_Destroy(stack);
}

//Clearing SeqStack is the same as clearing SeqList 
void SeqStack_Clear(SeqStack *stack)
{
    SeqList_Clear(stack);
}

//Pushing item to SeqStack is the same as appending to SeqList
int SeqStack_Push(SeqStack *stack, void* item)
{
    return SeqList_Insert(stack, item, SeqList_Length(stack));
}

//Poping is the same as deleting a member at the end of SeqList
void *SeqStack_Pop(SeqStack *stack)
{
    return SeqList_Delete(stack, SeqList_Length(stack)-1);
}

//To get top of the SeqStack is the same as getting the last member of the SeqList
void *SeqStack_Top(SeqStack *stack)
{
    return SeqList_Get(stack, SeqList_Length(stack)-1);
}

//To get size of the SeqStack is the same as getting length of the SeqList
int SeqStack_Size(SeqStack *stack)
{
    return SeqList_Length(stack);
}

//To get capacity of the SeqStack is the same as getting capacity of the SeqList
int SeqStack_Capacity(SeqStack *stack)
{
    return SeqList_Capacity(stack);
}
