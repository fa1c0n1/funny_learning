/* Copyright 2008 The Android Open Source Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "binder.h"

typedef unsigned char uchar;

enum binder_handle
{
    BINDER_HELLO_WORLD = 0,
    BINDER_ADD_OPER,
    BINDER_JOINT_STR,
};


int rpc_server_handler(struct binder_state *bs,
                       struct binder_transaction_data *txn,
                       struct binder_io *msg,
                       struct binder_io *reply)
{
    int (*handler)(struct binder_state * bs,
                   struct binder_transaction_data * txn,
                   struct binder_io * msg,
                   struct binder_io * reply);

    handler = (int (*)(struct binder_state * bs,
                       struct binder_transaction_data * txn,
                       struct binder_io * msg,
                       struct binder_io * reply))txn->target.ptr;

    printf("ser:rpc_server_handler call ...\n");
    return handler(bs, txn, msg, reply);
}



int hello_world(char *name)
{
    static int cnt = 0;
    printf("hello_world:cnt = %d, name = %s.\n", cnt++, name);
    return cnt;
}

int add_oper(int x, int y)
{
    int sum;
    static int cnt = 0;
    sum = x + y;
    printf("add_oper:cnt = %d, sum = %d \n", cnt++, sum);
    return sum;
}

char *joint_str(char *str1, char *str2)
{
    static int cnt = 0;
    if(str1 == NULL || str2 == NULL)
        return NULL;

    char buf[strlen(str1) + strlen(str2) + 128];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, str1, strlen(str1));
    strncat(buf, "-", 1);
    strncat(buf, str2, strlen(str2));
    printf("joint_str:cnt = %d, result=%s\n", cnt++, buf);
    return buf; 
}

int binder_service_handler(struct binder_state *bs,
                                struct binder_transaction_data *txn,
                                struct binder_io *msg,
                                struct binder_io *reply)
{
    uint16_t *s;
    char *result;
    char name[512];
    char t_buf1[512], t_buf2[512];
    int len;
    uint32_t handle;
    uint32_t strict_policy;
    int i;
    int x, y;

    strict_policy = bio_get_uint32(msg);


    switch (txn->code) {
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
    case BINDER_ADD_OPER:
        x = (int)bio_get_uint32(msg);
        y = (int)bio_get_uint32(msg);
        i = add_oper(x, y);
        bio_put_uint32(reply, i);
        break;
    case BINDER_JOINT_STR:
        s = bio_get_string16(msg, &len);
        if (s == NULL) return -1;

        for(i = 0; i < len; i++) {
            t_buf1[i] = s[i];
        }
        t_buf1[i] = '\0';

        s = bio_get_string16(msg, &len);
        if (s == NULL) return -1;

        for(i = 0; i < len; i++) {
            t_buf2[i] = s[i];
        }
        t_buf2[i] = '\0';
        
        result = joint_str(t_buf1, t_buf2);
        bio_put_string16_x(reply, result);
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
    unsigned iodata[512 / 4];
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

#define PUBLISH_FUNC(name,handle)    ret = svcmgr_publish(bs, svcmgr, name, handle);\
        if(ret) {\
            fprintf(stderr, "failed to publish %s service\n", name);\
            return -1;\
        }

void publish_funcs(struct binder_state *bs, void *svcmgr)
{
    int ret;
    PUBLISH_FUNC("hello_world", binder_service_handler);
    PUBLISH_FUNC("add_oper", binder_service_handler);
    PUBLISH_FUNC("joint_str", binder_service_handler);
}

int main(int argc, char **argv)
{
    int ret;
    int fd;
    struct binder_state *bs;
    void *svcmgr = BINDER_SERVICE_MANAGER;

    bs = binder_open(128 * 1024);

    printf("rpc binder server run ......");
    publish_funcs(bs, svcmgr);

    binder_loop(bs, rpc_server_handler);

    return 0;
}
