#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//二叉树的二叉链表示法
typedef struct _BitNode {
    int data;
    struct _BitNode *lchild, *rchild;
}BitNode;

void preOrder(BitNode *root)
{
    if (root == NULL) {
        return ; 
    } 

    printf("%d ", root->data);

    preOrder(root->lchild);
    preOrder(root->rchild);
}

void inOrder(BitNode *root)
{
    if (root == NULL) {
        return ; 
    }

    inOrder(root->lchild);
    printf("%d ", root->data);
    inOrder(root->rchild);
}

void postOrder(BitNode *root)
{
    if (root == NULL) {
        return ; 
    }

    postOrder(root->lchild);
    postOrder(root->rchild);
    printf("%d ", root->data);

}

void countLeaf(BitNode *root, int *leafSum)
{
    if (root != NULL) {
        if (!root->lchild && !root->rchild) {
            (*leafSum)++; 
        } 

        if (root->lchild != NULL) {
            countLeaf(root->lchild, leafSum); 
        }

        if (root->rchild != NULL) {
            countLeaf(root->rchild, leafSum); 
        }
    }
}

void test_link2btree(void)
{
    BitNode *p1, *p2, *p3, *p4, *p5;
    /*
       p1 = (BitNode *)calloc(1, sizeof(BitNode));
       p2 = (BitNode *)calloc(1, sizeof(BitNode));
       p3 = (BitNode *)calloc(1, sizeof(BitNode));
       p4 = (BitNode *)calloc(1, sizeof(BitNode));
       p5 = (BitNode *)calloc(1, sizeof(BitNode));

       p1->data = 1;
       p2->data = 2;
       p3->data = 3;
       p4->data = 4;
       p5->data = 5;

       p1->lchild = &p2;
       p1->rchild = &p3;
       p2->lchild = &p4;
       p3->lcihld = &p5;
       */

    BitNode t1, t2, t3, t4, t5;
    memset(&t1, 0, sizeof(BitNode));
    memset(&t2, 0, sizeof(BitNode));
    memset(&t3, 0, sizeof(BitNode));
    memset(&t4, 0, sizeof(BitNode));
    memset(&t5, 0, sizeof(BitNode));

    t1.data = 1;
    t2.data = 2;
    t3.data = 3;
    t4.data = 4;
    t5.data = 5;

    t1.lchild = &t2;
    t1.rchild = &t3;
    t2.lchild = &t4;
    t3.lchild = &t5;

    printf("----------- PreOrder Travesal --------\n");
    preOrder(&t1); 
    printf("\n----------- InOrder Travesal --------\n");
    inOrder(&t1);
    printf("\n----------- PostOrder Travesal --------\n");
    postOrder(&t1);

    int leafSum = 0;

    printf("\n\n");
    countLeaf(&t1, &leafSum);
    printf("LeafSum: %d\n", leafSum);
}

int binTreeDepth(BitNode *root)
{ 
    int leftDepth = 0;
    int rightDepth = 0;
    int depthVal = 0;

    if (root == NULL) {
        depthVal = 0;     
        return depthVal;
    } 

    leftDepth = binTreeDepth(root->lchild);
    rightDepth = binTreeDepth(root->rchild);

    depthVal = 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
    return depthVal;
}

void test_binTreeDepth(void)
{
    BitNode t1, t2, t3, t4, t5;
    memset(&t1, 0, sizeof(BitNode));
    memset(&t2, 0, sizeof(BitNode));
    memset(&t3, 0, sizeof(BitNode));
    memset(&t4, 0, sizeof(BitNode));
    memset(&t5, 0, sizeof(BitNode));

    t1.data = 1;
    t2.data = 2;
    t3.data = 3;
    t4.data = 4;
    t5.data = 5;

    t1.lchild = &t2;
    t1.rchild = &t3;
    t2.lchild = &t4;
    t3.lchild = &t5;

    int depthVal =  binTreeDepth(&t1);
    printf("t1 depth: %d\n", depthVal);
}

BitNode *copyBinTree(BitNode *root)
{
    BitNode *newNode = NULL;
    BitNode *newLeftP = NULL;
    BitNode *newRightP = NULL;

    if (root == NULL) {
        return NULL;
    }

    if (root->lchild != NULL) {
        newLeftP = copyBinTree(root->lchild); 
    } else {
        newLeftP = NULL;
    }

    if (root->rchild != NULL) {
        newRightP = copyBinTree(root->rchild); 
    } else {
        newRightP = NULL; 
    }

    newNode = (BitNode *)calloc(1, sizeof(BitNode));
    if (newNode == NULL) {
        fprintf(stderr, "calloc error.\n");
        exit(1);
    }

    newNode->data = root->data;
    newNode->lchild = newLeftP;
    newNode->rchild = newRightP;

    return newNode;
}

void test_copyBinTree(void)
{
    BitNode t1, t2, t3, t4, t5;
    memset(&t1, 0, sizeof(BitNode));
    memset(&t2, 0, sizeof(BitNode));
    memset(&t3, 0, sizeof(BitNode));
    memset(&t4, 0, sizeof(BitNode));
    memset(&t5, 0, sizeof(BitNode));

    t1.data = 1;
    t2.data = 2;
    t3.data = 3;
    t4.data = 4;
    t5.data = 5;

    t1.lchild = &t2;
    t1.rchild = &t3;
    t2.lchild = &t4;
    t3.lchild = &t5;

    BitNode *newBinTree = copyBinTree(&t1);

    printf("Aftering copying ... :\n");
    printf("----------- PreOrder Travesal --------\n");
    preOrder(newBinTree); 
    printf("\n----------- InOrder Travesal --------\n");
    inOrder(newBinTree);
    printf("\n----------- PostOrder Travesal --------\n");
    postOrder(newBinTree);

    int leafSum = 0;

    printf("\n\n");
    countLeaf(newBinTree, &leafSum);
    printf("LeafSum: %d\n", leafSum);

    int depthVal =  binTreeDepth(newBinTree);
    printf("newBinTree depth: %d\n", depthVal);
//    free(newBinTree);
}

#define MAX_TREE_SIZE    100

//二叉树的双亲链表示法
typedef struct _BPNode
{
    int data;
    int parentPosition; //指向双亲的指针.数组下标
    char LRTag; //左右孩子标志域
}BPNode;

typedef struct _BPTree
{
    BPNode nodes[100]; //因为节点之间是分散的，需要把节点存储到数组中
    int num_node; //节点数目
    int root; //根节点的位置. 注意此域存储的是父节点在数组的下标
}BPTree;

void test_linkParentTree()
{
    BPTree tree;      

    //A节点[根节点]
    tree.nodes[0].parentPosition = 1000;

    //B节点
    tree.nodes[1].parentPosition = 0;
    tree.nodes[1].LRTag = 1;
    tree.nodes[1].data = 'B';

    //C节点
    tree.nodes[2].parentPosition = 0;
    tree.nodes[2].LRTag = 2;
    tree.nodes[2].data = 'C';
}

int main(void)
{
    //test_link2btree();
    //test_binTreeDepth();
    test_copyBinTree();
    return 0;
}
