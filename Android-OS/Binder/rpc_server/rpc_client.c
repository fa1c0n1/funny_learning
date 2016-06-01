/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "binder.h"

struct binder_state *g_bs;
uint32_t g_hello_world_handle;


#define BINDER_HELLO_WORLD  1


void *svcmgr_lookup(struct binder_state *bs, void *target, const char *name)
{
    void *ptr;
    unsigned iodata[512/4];
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
	unsigned iodata[512/4];
	struct binder_io msg, reply;
	int ret;
	bio_init(&msg, iodata, sizeof(iodata), 4);
	bio_put_uint32(&msg, 0);
    bio_put_string16_x(&msg, name);
	if (binder_call(g_bs, &msg, &reply, g_hello_world_handle, BINDER_HELLO_WORLD))
		return 0;
	ret = bio_get_uint32(&reply);
	binder_done(g_bs, &msg, &reply);
	return ret;

}


int main(int argc, char **argv)
{
    int fd;
	int ret = 0;
    struct binder_state *bs;
    void *svcmgr = BINDER_SERVICE_MANAGER;
	uint32_t handle;

    bs = binder_open(128*1024);
	printf("ser:binder_open:bs = 0x%x.\n", bs);
	if (!bs) {
        fprintf(stderr, "failed to open binder driver\n");
        return -1;
    }
	g_bs = bs;
	handle = svcmgr_lookup(bs, svcmgr, "hello_world");
	if (!handle) {
        printf( "failed to get hello service\n");
        return -1;
	}	
	g_hello_world_handle = handle;

	ret = hello_world("abcdefg");
	
	printf("hello_world = %d.\n", ret);
    return 0;
}
