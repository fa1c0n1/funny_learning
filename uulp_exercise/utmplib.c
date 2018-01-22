#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>

#define UTMP_RECNUM    16
#define UTMP_SIZE      (sizeof(struct utmp))
#define UTMP_NULL      ((struct utmp *)NULL)

static int fd_utmp;
static int curnum_recs;
static int totnum_recs;
static char buf[UTMP_RECNUM*UTMP_SIZE];

static int utmp_bufload(void);

int utmp_open(mode_t mode)
{
    fd_utmp = open(UTMP_FILE, mode);
    curnum_recs = totnum_recs = 0;
    return fd_utmp;
}

struct utmp *utmp_next(void)
{
    struct utmp *utbufp;

    if (fd_utmp == -1) {
        return UTMP_NULL; 
    }

    if (curnum_recs == totnum_recs && utmp_bufload() == 0) {
        return UTMP_NULL; 
    }

    utbufp = (struct utmp *)&buf[curnum_recs*UTMP_SIZE];
    curnum_recs++;

    return utbufp;
}

int utmp_seek(int rec_offset, int base)
{
    int newpos;

    if (base == SEEK_CUR) {
        rec_offset += (curnum_recs - totnum_recs);
    }
        
    newpos = lseek(fd_utmp, rec_offset*UTMP_SIZE, base);
    if (newpos == -1) {
        perror("lseek error");
        return -1;
    }

    //如果不让它重新获取缓冲区(即调用 utmp_bufload)的话,
    //当缓冲区的 utmp 结构取完后，再次执行 utmp_bufload 得到新的缓冲区，
    // 导致新的缓冲区中部分 utmp 结构是前一次 utmp_bufload 得到的，因为
    // 由于 lseek 的调用，文件指针已经改变。
    curnum_recs = totnum_recs;       /* force to call utmp_bufload */
    return newpos / UTMP_SIZE;
}

static int utmp_bufload(void)
{
    int nbytes;
    
    nbytes = read(fd_utmp, buf, sizeof(buf));
    totnum_recs = nbytes / UTMP_SIZE;
    curnum_recs = 0;

    return totnum_recs;
}

void utmp_close(void)
{
    if (fd_utmp != -1) {
        close(fd_utmp); 
    }
}
