/*
 *  纸牌游戏：小猫钓鱼
 *
 *  游戏规则：
 *      将一副扑克牌平均分成两份, 每人拿一份.
 *      小哼先拿出手中第一张扑克牌放在桌上, 然后小哈也拿出手中第一张扑克牌, 
 *      并放在小哼刚打出的扑克牌的上面, 就像这样两人交替出牌。出牌时, 如果
 *      某人打出的牌与桌上某张牌的牌面相同, 即可将两张相同的牌及其中间所夹
 *      的牌全部取走, 并依次放到自己手中牌的末尾。当任意一人手中的牌全部出
 *      完时, 游戏结束, 对手获胜.
 *
 */

#include <stdio.h>

typedef struct {
    int data[1000];
    int head;
    int tail;
} Queue;

typedef struct {
    int data[10];
    int top;
} Stack;

int main(void)
{
    Queue q1, q2;
    Stack s;
    int book[10];
    int i, t;

    //初始化队列
    q1.head = 1; q1.tail = 1;
    q2.head = 1; q2.tail = 1;

    //初始化栈
    s.top = 0;
    //初始化用来标记的数组, 用来标记哪些牌已经在桌上
    for(i = 1; i <= 9; i++) {
        book[i] = 0;
    }

    //依次向队列插入6个数
    //小哼手上的6张牌
    for(i = 1; i <= 6; i++) {
        scanf("%d", &q1.data[q1.tail]);
        q1.tail++;
    }

    //小哈手上的6张牌
    for(i = 1; i <= 6; i++) {
        scanf("%d", &q2.data[q2.tail]);
        q2.tail++;
    }

    while(q1.head < q1.tail && q2.head < q2.tail) {
        t = q1.data[q1.head];  //小哼出一张牌
        //判断小哼当前打出的牌是否能赢牌
        if (book[t] == 0) {
            //小哼此轮没有赢牌 
            q1.head++;   //小哼已经打出一张牌, 所以要把打出的牌出队
            s.top++;
            s.data[s.top] = t; //再把打出的牌放到桌上, 即入栈
            book[t] = 1;  //标记桌上现在已经有牌面为t的牌
        } else {
            //小哼此轮可以赢牌
            q1.head++;   //小哼已经打出一张牌, 所以要把打出的牌出队
            q1.data[q1.tail] = t;   //紧接着把打出的牌放到手中牌的末尾
            q1.tail++;
            while(s.data[s.top] != t) { //把桌子可以赢得的牌依次放到手中牌的末尾
                book[s.data[s.top]] = 0;  //取消标记
                q1.data[q1.tail] = s.data[s.top];  //依次放入队尾
                q1.tail++;
                s.top--;  //栈中少了一张牌, 所以栈顶要减1
            }
       }

       t = q2.data[q2.head];   //小哈出一张牌
       //判断小哈当前打出的牌是否能赢牌
       if(book[t] == 0) {  //表明桌上没有牌面为t的牌
            //小哈此轮没有赢牌 
            q2.head++;   //小哈已经打出一张牌, 所以要把打出的牌出队
            s.top++;
            s.data[s.top] = t;  //再把打出的牌放到桌上, 即入栈
            book[t] = 1;  // 标记桌上现在已经有牌面为t的牌
       } else {
            //小哈此轮可以赢牌 
            q2.head++;  //小哈已经打出一张牌, 所以要把打出的牌出队
            q2.data[q2.tail] = t;  //紧接着把打出的牌放到手中牌的末尾
            q2.tail++;
            while(s.data[s.top] != t) { //把桌上可以赢得的牌依次放到手中牌的末尾
                book[s.data[s.top]] = 0;   //取消标记
                q2.data[q2.tail] = s.data[s.top];  //依次放入队尾
                q2.tail++;
                s.top--;
            }
       }
    }

    if(q2.head == q2.tail) {
        printf("小哼win\n");
        printf("小哼当前手中的牌是");

        for(i = q1.head; i <= q1.tail - 1; i++) {
            printf(" %d", q1.data[i]);
        }
        if(s.top > 0) {
            printf("\n桌上的牌是");
            for(i = 1; i <= s.top; i++) {
                printf(" %d", s.data[i]);
            }
        } else {
            printf("\n桌上已经没有牌了");
        }
    } else {
        printf("小哈win\n");
        printf("小哈当前手中的牌是");
        for(i = q2.head; i <= q2.tail - 1; i++) {
            printf(" %d", q2.data[i]);
        }
        if(s.top > 0) {
            printf("\n桌上的牌是");
            for(i = 1; i <= s.top; i++) {
                printf(" %d", s.data[i]);
            }
        } else {
            printf("\n桌上已经没有牌了");
        }
    }

    printf("\n");


    return 0;
}
