#ifndef _ERRHANDLE_H
#define _ERRHANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

void exit_usage(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void exit_err(char *cmd, char *msg)
{
    fprintf(stderr, "%s \"%s\": %s\n", cmd, msg, strerror(errno));
    exit(EXIT_FAILURE);
}

#endif  // _ERRHANDLE_H
