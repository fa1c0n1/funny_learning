#include <stdio.h>
#include <string.h>
#include "linkstack.h"

typedef struct _BitNode {
    int data;
    struct _BitNode *lchild, *rchild;
}BitNode, *BitTree;

//一直向左走，找到中序遍历的起点
BitNode *goLeft(BitNode *root, LinkStack *stack)
{
    if (root == NULL) {
        return NULL;
    }

    //判断root有没有左孩子，没有就把root return
    //如果有，则root入栈，在往左走
    while (root->lchild != NULL) {
        LinkStack_Push(stack, (void *)root);
        root = root->lchild;
    }

    return root;
}

void inOrderNotRecurse(BitNode *root)
{
    BitNode *tNode = NULL;

    LinkStack *stack = LinkStack_Create();
    tNode = goLeft(root, stack);

    while (tNode != NULL) {
        printf("%c ", tNode->data);

        //如果tNode有右子树，重复步骤1
        if (tNode->rchild != NULL) {
            tNode = goLeft(tNode->rchild, stack);
        } else if (LinkStack_Size(stack) > 0) {
            //如果tNode没有右子树且栈不为空，根据栈顶指示回退,访问栈顶元素
            tNode = (BitNode *)LinkStack_Pop(stack);
        } else {
            //如果tNode没有右子树且栈为空
            tNode = NULL;
        }
    }

    LinkStack_Destroy(stack);
}

int main()
{
    BitNode t1, t2, t3, t4, t5, t6, t7, t8, t9;

    memset(&t1, 0, sizeof(BitNode));
    memset(&t2, 0, sizeof(BitNode));
    memset(&t3, 0, sizeof(BitNode));
    memset(&t4, 0, sizeof(BitNode));
    memset(&t5, 0, sizeof(BitNode));
    memset(&t6, 0, sizeof(BitNode));
    memset(&t7, 0, sizeof(BitNode));
    memset(&t8, 0, sizeof(BitNode));
    memset(&t9, 0, sizeof(BitNode));

    t1.data = 'A'; 
    t2.data = 'B';
    t3.data = 'E';
    t4.data = 'C';
    t5.data = 'F';
    t6.data = 'D';
    t7.data = 'G';
    t8.data = 'H';
    t9.data = 'I';

    t1.lchild = &t2;
    t1.rchild = &t3;
    t2.rchild = &t4;
    t3.rchild = &t5;
    t4.lchild = &t6;
    t5.lchild = &t7;
    t7.lchild = &t8;
    t7.rchild = &t9;

    inOrderNotRecurse(&t1);
    printf("\n");

    return 0;
}
