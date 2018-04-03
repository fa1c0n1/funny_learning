#pragma once

#include <windows.h>

#define BUFMSG       2048    //一次接收消息的最大字节数
#define BUFNAME      64

//--- 头部: 通讯目的，包括匿名登录，聊天，1VS1聊天，注册，登录，添加好友，搜索好友，文件传输，查看聊天记录
typedef enum {
	ANONYMOUS = 1,
	CHAT,
	ONE2ONE,
	REGISTER,
	LOGIN,
	ADDFRIEND,
	SEARCHUSER,
	FILETRANS,
	MSGRECORD,
	UPDATEUSER,
	UPDATEFRIEND
} CHATPURPOSE;

//--- 聊天内容

//匿名聊天结构体
typedef struct _CHATANONYMOUS {
	DWORD dwLen;
	char buf[1024];
} CHATANONYMOUS;

//聊天结构体
typedef struct _CHATCHAT {
	DWORD dwLen;
	char buf[1024];
} CHATCHAT;

//1VS1聊天结构体
typedef struct _CHATONE2ONE {
	char szNameFrom[64];
	char szNameTo[64];
	char szContent[1024];
} CHATONE2ONE;

//注册用户结构体
typedef struct _CHATREGISTER {
	char szName[64];
	char szPwd[64];
} CHATREGISTER;

//用户登录结构体
typedef struct _CHATLOGIN {
	char szName[64];
	char szPwd[64];
} CHATLOGIN;

//添加好友结构体
typedef struct _CHATADDFRIEND {
	bool bRetAdd;       //服务器的添加结果
	char szName[64];
	char szFriendName[64];
	char buf[1024];     //服务器的返回信息
} CHATADDFRIEND;

//搜索用户结构体
typedef struct _CHATSEARCHUSER {
	char szName[64];
} CHATSEARCHUSER;

//文件传输结构体
typedef struct _CHATFILETRANS {
	
} CHATFILETRANS;

//查看聊天记录结构体
typedef struct _CHATMSGRECORD {
	char szFrom[64];
	char szTo[64];
	char szContent[1024];
} CHATMSGRECORD;

//更新在线用户列表
typedef struct _CHATUPDATEUSER {
	bool bAdd;
	DWORD dwLen;
	char buf[64];
} CHATUPDATEUSER;

typedef struct _CHATUPDATEFRIEND {
	bool bAdd;
	char szFriendName[64];
} CHATUPDATEFRIEND;

//--- 发送内容
typedef struct _CHATSEND {
	CHATPURPOSE m_type;
	union {
		char buf[BUFMSG];
		CHATANONYMOUS any;
		CHATCHAT chat;
		CHATONE2ONE o2o;
		CHATREGISTER reg;
		CHATLOGIN login;
		CHATADDFRIEND adf;
		CHATSEARCHUSER seu;
		CHATFILETRANS trs;
		CHATMSGRECORD rec;
		CHATUPDATEUSER upd;
		CHATUPDATEFRIEND updf;
	} m_content;
} CHATSEND;


