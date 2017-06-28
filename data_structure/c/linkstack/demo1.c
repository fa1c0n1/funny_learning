#include <stdio.h>
#include "linkstack.h"

int check_syntax(const char *code)
{
    int i;
    LinkStack *stack = LinkStack_Create();
    char *p = code;
    char *tItem = NULL;

    for (i = 0; p[i] != '\0'; i++) {
        if (p[i] == '{') {
            LinkStack_Push(stack, (void *)&p[i]); 
        } else if (p[i] == '}') {
            tItem = LinkStack_Pop(stack);
            if (tItem == NULL) {
                return -1;
            }
        }
    }

    if (LinkStack_Size(stack) > 0) {
        return -1; 
    }

    LinkStack_Destroy(stack);

    return 0;
}

int main(void)
{
    int ret;
    
    const char *program1 = "\
        #include <stdio.h> \
        int main() \
        {\
            int a[4][4];\
            int (*p)[4];\
            p = a[0];\
            return 0;\
        }";

    ret = check_syntax(program1);
    if (ret != 0) {
        fprintf(stderr, "syntax error.\n");
    } else {
        printf("check pass.\n"); 
    }

    return ret;
}
