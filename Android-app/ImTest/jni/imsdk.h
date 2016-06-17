
//  Created by owen on 16/5/9.
//  Copyright © 2016年 owen. All rights reserved.
//

#ifndef imsdk_h
#define imsdk_h

#include <stdio.h>

enum IMSDK_COMMAND//消息类型
{
    CMD_LOGIN_REQ = 0,//登陆
    CMD_LOGIN_RSP,//登陆返回
    CMD_SEND_REQ,//发送消息
    CMD_SEND_RSPS//消息返回
};

enum IMSDK_DEVICE//设备
{
    DEVICE_INVALID = -1,
    DEVICE_IOS,
    DEVICE_ANDROID,
    DEVICE_WINDOWS,
    DEVICE_MAC
};

enum IMSDK_STATUS//状态
{
    STATUS_SUCCESS = 0,
    STATUS_CHK_FAILED,
    STATUS_FORMAT_FAILED,
    STATUS_LOGIN_FAILED,
    STATUS_UNLOGIN,
    STATUS_SERVICE_BUSY
};

enum IMSDK_MSGSCOPE//单人或群发
{
    MSGSCOPE_SINGLE = 0,
    MSGSCOPE_GROUP
};

typedef void (* onCallback)(const char *);

struct IMSDK_JSON {
    uint32_t cmd;//消息类型
    uint32_t sender;//发送人
    uint32_t receiver;//接收人
    uint32_t msgid;//userId
    uint8_t msgscope;//消息范围,单人或群发
    
    uint8_t device;//设备
    char token[33];//token
    char version[16];//版本
    
    char body[512];//消息内容
    uint8_t status;//消息状态
    char chk[32];//校验码
};

/*
 conn: 连接回调
 diconn: 断开连接回调
 data: 收到数据回调
 err: 出错回调
 */
void IMSDK_setCallback(onCallback conn, onCallback disconn, onCallback data,onCallback err);
void IMSDK_connect(const char *ip,uint16_t port);
void IMSDK_send(struct IMSDK_JSON *json);
void IMSDK_close();
int IMSDK_isconn();
#endif /* imsdk_h */
