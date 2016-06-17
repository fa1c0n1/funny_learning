//
//  imsdk.c
//  IMSDKDemo
//
//  Created by owen on 16/5/9.
//  Copyright © 2016年 owen. All rights reserved.
//

#include "imsdk.h"
#include "dyad.h"
#include "http-client-c.h"
#include "parson.h"

onCallback _conn = NULL;
onCallback _disconn = NULL;
onCallback _data = NULL;
onCallback _err = NULL;

dyad_Stream *server = NULL;

static void onConnect(dyad_Event *e)
{
    if (_conn) {
        _conn(e->msg);
    }
}

static void onData(dyad_Event *e)
{
    if (_data) {
        _data(e->data);
    }
}

static void onError(dyad_Event *e)
{
    if (_err) {
        _err(e->msg);
    }
}

static void onClose(dyad_Event *e)
{
    if (server) {
        server = NULL;
    }
    
    if (_disconn) {
        _disconn(e->msg);
    }
}

void IMSDK_setCallback(onCallback conn, onCallback disconn, onCallback data,onCallback err)
{
    _conn = conn;
    _disconn = disconn;
    _data = data;
    _err = err;
}

struct _IPADDR
{
    char ip[32];
    uint port;
};


static struct _IPADDR* getIMServerIPAddr(const char *ip, uint port)
{
    struct _IPADDR *ia = (struct _IPADDR *)malloc(sizeof(struct _IPADDR));
    ia->port = 0;
    
    char strUrl[150];
    sprintf(strUrl, "http://%s:%d/%s",ip,port,"queryIMServer");
    
    struct http_response *hresp = http_get(strUrl, "User-agent:MyUserAgent\r\n");
    
    if (hresp) {
        /*JSON_Value *root = json_parse_string(hresp->body);
        
        JSON_Value *data = json_object_get_value(json_object(root), "data");
        const char *imip = json_object_get_string(json_object(data), "ip");
        uint import = json_object_get_number(json_object(data), "port");
        strcpy(ia->ip, imip);
        ia->port = import;
        
        json_value_free(root);*/
        
        
        JSON_Value *root = json_parse_string(hresp->body);
        const char *imip = json_object_get_string(json_object(root), "ip");
        uint import = json_object_get_number(json_object(root), "port");
        strcpy(ia->ip,imip);
        ia->port = import;
        
        json_value_free(root);
    }
    
    return ia;
}

//use in thread
void IMSDK_connect(const char *ip,uint16_t port)
{
    //step 1:
    //get im server throught ip and port
    //use ipaddr to connect imserver
    //step 2:
    //init dyad and event loop
    struct _IPADDR *ia = getIMServerIPAddr(ip, port);
    
    if (ia->port != 0) {
        dyad_init();
        
        server = dyad_newStream();
        dyad_addListener(server, DYAD_EVENT_CONNECT, onConnect, NULL);
        dyad_addListener(server, DYAD_EVENT_DATA, onData, NULL);
        dyad_addListener(server, DYAD_EVENT_CLOSE, onClose, NULL);
        dyad_addListener(server, DYAD_EVENT_ERROR, onError, NULL);
        
        dyad_connect(server, ia->ip, ia->port);
        
        while (dyad_getStreamCount() > 0) {
            dyad_update();
        }
        
        dyad_shutdown();
    }else{
        if (_err) {
            _err("not get im server ip and port...");
        }
    }
    free(ia);
}

void IMSDK_close()
{
    if (server) {
        dyad_close(server);
    }
    
    server = NULL;
}

int IMSDK_isconn()
{
    if (dyad_getStreamCount() > 0) {
        return 1;
    }
    return 0;
}

void IMSDK_send(struct IMSDK_JSON *json)
{
    if (server) {
        //dyad_write(s, data, size);
        JSON_Value *root_value = json_value_init_object();
        JSON_Object *root_object = json_value_get_object(root_value);
        
        char *seriallized_string = NULL;
        json_object_set_number(root_object, "cmd", json->cmd);
        json_object_set_number(root_object, "sender", json->sender);
        json_object_set_number(root_object, "receiver", json->receiver);
        json_object_set_number(root_object, "msgid", json->msgid);
        json_object_set_number(root_object, "msgscope", json->msgscope);
        
        json_object_set_number(root_object, "device", json->device);
        json_object_set_string(root_object, "token", json->token);
        json_object_set_string(root_object, "version", json->version);
        
        json_object_set_string(root_object, "body", json->body);
        json_object_set_number(root_object, "status", json->status);
        json_object_set_string(root_object, "chk", json->chk);
        
        seriallized_string = json_serialize_to_string(root_value);
        
        size_t len = strlen(seriallized_string);
        dyad_write(server, seriallized_string, (int)len);
        dyad_write(server,"\n",1);
        
        printf("%s\r",seriallized_string);
        
        json_free_serialized_string(seriallized_string);
        json_value_free(root_value);
    }
    else{
        if (_err) {
            _err("not connected...");
        }
    }
}