#ifndef _UTMPLIB_H
#define _UTMPLIB_H

#include <utmp.h>

extern int utmp_open(mode_t mode);
extern struct utmp *utmp_next(void);
extern void utmp_close(void);
extern int utmp_seek(int rec_offset, int base);

#endif
