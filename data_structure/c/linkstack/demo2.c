#include <stdio.h>
#include <stdlib.h>
#include "linkstack.h"

int isNumber(char ch)
{
    return (ch >= '0' && ch <= '9');
}

int isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int toInt(char ch)
{
    return ch - '0';
}

int execute_compute(int left, int right, char op)
{
    int ret = 0;
    switch(op)
    {
        case '+': 
            ret = left + right;
            break;
        case '-':
            ret = left - right;
            break;
        case '*':
            ret = left * right;
            break;
        case '/':
            if (right == 0) {
                fprintf(stderr, "Divide zero error.\n");
                exit(1);
            }
            ret = left / right;
            break;
        default:
            break;
    }

    return ret;
}

int compute(const char *str)
{
    int i;
    int ret = 0;
    char *p = str;

    LinkStack *stack = LinkStack_Create();
    if (stack == NULL) {
        fprintf(stderr, "LinkStack_Create() error.\n");
        return -1;     
    }

    for (i = 0; p[i] != '\0'; i++) {
        if (isNumber(p[i])) {
            LinkStack_Push(stack, (void *)toInt(p[i])); 
        } else if (isOperator(p[i])) {
            int right = (int)LinkStack_Pop(stack); 
            int left = (int)LinkStack_Pop(stack);
            ret = execute_compute(left, right, p[i]);
            LinkStack_Push(stack, (void *)ret);
        } else {
            fprintf(stderr, "Invalid character.\n");
            ret = -1;
            break;
        }
    }

    return ret;
}

int priority(char ch)
{
    int ret = 0;

    if (ch == '+' || ch == '-') {
        ret = 1; 
    }

    if (ch == '*' || ch == '/') {
        ret = 2; 
    }

    return ret;
}

int isLeftParenthesis(char ch)
{
    return ch == '(';
}

int isRightParenthesis(char ch)
{
    return ch == ')';
}

void transform_express(const char *in, char *out)
{
    int i, ret = 0;
    LinkStack *stack = LinkStack_Create();

    for (i = 0; in[i] != '\0'; i++) {
        if (isNumber(in[i])) {
            strncat(out, &in[i], 1); 
        } else if (isLeftParenthesis(in[i])) {
            LinkStack_Push(stack, (void *)(int)in[i]);
        } else if (isRightParenthesis(in[i])) {
            while (!isLeftParenthesis((char)(int)LinkStack_Top(stack))) {
                char tch = (char)(int)LinkStack_Pop(stack);
                strncat(out, &tch, 1);
            } 

            LinkStack_Pop(stack);
        } else if (isOperator(in[i])) {
            while (priority(in[i]) <= priority((char)(int)LinkStack_Top(stack))) {
                char tch = (char)(int)LinkStack_Pop(stack);
                strncat(out, &tch, 1); 
            }

            LinkStack_Push(stack, (void *)(int)in[i]);
        }
    }

    while (LinkStack_Size(stack) > 0 && in[i] == '\0') {
        char tch = (char)(int)LinkStack_Pop(stack);
        strncat(out, &tch, 1); 
    }

    LinkStack_Destroy(stack);
}

int main(void)
{
    int ret;
    const char *express = "8+(3-1)*5";
//    const char *express = "9+(3-1)*5+8/2";

    char *outexp = (char *)calloc(1, strlen(express)+1);
    transform_express(express, outexp);
#if 1
    printf("outexp=%s\n", outexp);
    ret = compute(outexp);

    printf("%s = %d\n", express, ret);
#endif
    free(outexp);

    return 0;
}
