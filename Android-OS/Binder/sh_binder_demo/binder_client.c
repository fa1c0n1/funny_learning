/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "binder.h"

struct binder_state *g_bs;
uint32_t g_handle;


enum binder_handle
{
    BINDER_HELLO_WORLD = 0,
    BINDER_ADD_OPER,
    BINDER_JOINT_STR,
};

void *svcmgr_lookup(struct binder_state *bs, void *target, const char *name)
{
    void *ptr;
    unsigned iodata[512 / 4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_string16_x(&msg, SVC_MGR_NAME);
    bio_put_string16_x(&msg, name);

    if (binder_call(bs, &msg, &reply, target, SVC_MGR_CHECK_SERVICE))
        return 0;

    ptr = bio_get_ref(&reply);

    if (ptr)
        binder_acquire(bs, ptr);

    binder_done(bs, &msg, &reply);

    return ptr;
}


int hello_world(char *name)
{
    unsigned iodata[512 / 4];
    struct binder_io msg, reply;
    int ret;
    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);
    bio_put_string16_x(&msg, name);
    if (binder_call(g_bs, &msg, &reply, g_handle, BINDER_HELLO_WORLD))
        return 0;
    ret = bio_get_uint32(&reply);
    binder_done(g_bs, &msg, &reply);
    return ret;
}

int add_oper(int x, int y)
{
    unsigned iodata[512 / 4];
    struct binder_io msg, reply;
    int ret;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);
    bio_put_uint32(&msg, x);
    bio_put_uint32(&msg, y);
    if(binder_call(g_bs, &msg, &reply, g_handle, BINDER_ADD_OPER)) 
        return 0;
    ret = bio_get_uint32(&reply);
    binder_done(g_bs, &msg, &reply);
    return ret;
}

char *joint_str(char *str1, char *str2)
{
    unsigned iodata[512 / 4];
    struct binder_io msg, reply;
    uint16_t *tmp;
    char result[512];
    int i, len;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);
    bio_put_string16_x(&msg, str1);
    bio_put_string16_x(&msg, str2);
    if(binder_call(g_bs, &msg, &reply, g_handle, BINDER_JOINT_STR))
        return 0;
    tmp = bio_get_string16(&reply, &len);
    for(i = 0; i < len; i++) {
        result[i] = tmp[i];
    }
    result[i] = '\0';

    binder_done(g_bs, &msg, &reply);
    return result;
}


int main(int argc, char **argv)
{
    int fd;
    int ret = 0;
    struct binder_state *bs;
    void *svcmgr = BINDER_SERVICE_MANAGER;
    uint32_t handle;

    bs = binder_open(128 * 1024);
    printf("ser:binder_open:bs = 0x%x.\n", bs);
    if (!bs) {
        fprintf(stderr, "failed to open binder driver\n");
        return -1;
    }
    g_bs = bs;
    // handle = svcmgr_lookup(bs, svcmgr, "hello_world");
    handle = svcmgr_lookup(bs, svcmgr, "add_oper");
    if (!handle) {
        printf( "failed to get add_oper service\n");
        return -1;
    }
    g_handle = handle;

    // ret = hello_world("Hello Binder!");
    // ret = add_oper(3, 5);

    // printf("hello_world = %d.\n", ret);
    // printf("a+b=%d\n", ret);
    printf("result=%s\n", joint_str("abcd", "efg"));
    return 0;
}
