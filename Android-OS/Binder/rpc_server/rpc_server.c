/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "binder.h"

#define BINDER_HELLO_WORLD  1

int rpc_server_handler(struct binder_state *bs,
                   struct binder_transaction_data *txn,
                   struct binder_io *msg,
                   struct binder_io *reply)
{
	int (*handler)(struct binder_state *bs,
                   struct binder_transaction_data *txn,
                   struct binder_io *msg,
                   struct binder_io *reply);

	handler = (int (*)(struct binder_state *bs,
                   struct binder_transaction_data *txn,
                   struct binder_io *msg,
                   struct binder_io *reply))txn->target.ptr;

	printf("ser:rpc_server_handler call ...\n");
	return handler(bs, txn, msg, reply);
}



int hello_world(char *name)
{
	static int cnt = 0;
	printf("hello_world:cnt = %d, name = %s.\n", cnt++, name);
	return cnt;
}

int hello_world_service_handler(struct binder_state *bs,
                   struct binder_transaction_data *txn,
                   struct binder_io *msg,
                   struct binder_io *reply)
{	
    uint16_t *s;
	char name[512];
    int len;
    uint32_t handle;
    uint32_t strict_policy;
	int i;
	
    strict_policy = bio_get_uint32(msg);


    switch(txn->code) {
    case BINDER_HELLO_WORLD:
		s = bio_get_string16(msg, &len);
		if (s == NULL) {
			return -1;
		}
		for (i = 0; i < len; i++)
			name[i] = s[i];
		name[i] = '\0';
		
		i = hello_world(name);
		bio_put_uint32(reply, i);
        break;

    default:
        printf("unknown code %d\n", txn->code);
        return -1;
    }

    return 0;
}


int svcmgr_publish(struct binder_state *bs, void *target, const char *name, void *ptr)
{
    unsigned status;
    unsigned iodata[512/4];
    struct binder_io msg, reply;

    bio_init(&msg, iodata, sizeof(iodata), 4);
    bio_put_uint32(&msg, 0);  // strict mode header
    bio_put_string16_x(&msg, SVC_MGR_NAME);
    bio_put_string16_x(&msg, name);
    bio_put_obj(&msg, ptr);

    if (binder_call(bs, &msg, &reply, target, SVC_MGR_ADD_SERVICE))
        return -1;

    status = bio_get_uint32(&reply);

    binder_done(bs, &msg, &reply);

    return status;
}

unsigned token;

int main(int argc, char **argv)
{
	int ret;
    int fd;
    struct binder_state *bs;
    void *svcmgr = BINDER_SERVICE_MANAGER;

    bs = binder_open(128*1024);

	printf("rpc binder server run ......");
	ret = svcmgr_publish(bs, svcmgr, "hello_world", hello_world_service_handler);
	if (ret) {
		printf("failed to publish hello_world service\n");
		return -1;
	}

	binder_loop(bs, rpc_server_handler);
	
    return 0;
}
