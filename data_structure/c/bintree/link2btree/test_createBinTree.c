#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _BitNode {
    int data;
    struct _BitNode *lchild, *rchild;
}BitNode, *BitTree;

void preOrder(BitNode *root)
{
    if (root == NULL) {
        return ; 
    } 

    printf("%d ", root->data);

    preOrder(root->lchild);
    preOrder(root->rchild);
}


BitNode *CreateBinTree() {
    char ch;
    BitNode *tNode = NULL;

    scanf("%c", &ch);
    if (ch == '#') {
        tNode = NULL; 
    } else {
        tNode = (BitNode *)calloc(1, sizeof(BitNode)); 
        int t = atoi(&ch);
        tNode->data = t;
        tNode->lchild = CreateBinTree();
        tNode->rchild = CreateBinTree();
    }

    return tNode;
}

void FreeBinTree(BitNode *root)
{
    if (root != NULL) {
        if (root->lchild != NULL) {
            FreeBinTree(root->lchild); 
        }

        if (root->rchild != NULL) {
            FreeBinTree(root->rchild); 
        }

        free(root);
        root = NULL;
    }
}

int main(void)
{
    BitNode *binTree = CreateBinTree();
    preOrder(binTree);
    printf("\n");
    FreeBinTree(binTree);

    return 0;
}
