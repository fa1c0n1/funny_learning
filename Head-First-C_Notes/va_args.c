#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void print_ints(int args, ...) {
    va_list ap;
    va_start(ap, args);

    int i;
    for (i = 0; i < args; i++) {
        printf("argument: %d\n", va_arg(ap, int));
    }
    va_end(ap);
}

int main(void)
{
    print_ints(3, 79, 101, 32);
    return 0;
}
