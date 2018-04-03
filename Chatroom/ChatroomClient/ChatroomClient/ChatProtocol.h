#pragma once

#include <windows.h>

#define BUFMSG       2048    //һ�ν�����Ϣ������ֽ���
#define BUFNAME      64

//--- ͷ��: ͨѶĿ�ģ�����������¼�����죬1VS1���죬ע�ᣬ��¼����Ӻ��ѣ��������ѣ��ļ����䣬�鿴�����¼
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

//--- ��������

//��������ṹ��
typedef struct _CHATANONYMOUS {
	DWORD dwLen;
	char buf[1024];
} CHATANONYMOUS;

//����ṹ��
typedef struct _CHATCHAT {
	DWORD dwLen;
	char buf[1024];
} CHATCHAT;

//1VS1����ṹ��
typedef struct _CHATONE2ONE {
	char szNameFrom[64];
	char szNameTo[64];
	char szContent[1024];
} CHATONE2ONE;

//ע���û��ṹ��
typedef struct _CHATREGISTER {
	char szName[64];
	char szPwd[64];
} CHATREGISTER;

//�û���¼�ṹ��
typedef struct _CHATLOGIN {
	char szName[64];
	char szPwd[64];
} CHATLOGIN;

//��Ӻ��ѽṹ��
typedef struct _CHATADDFRIEND {
	bool bRetAdd;       //����������ӽ��
	char szName[64];
	char szFriendName[64];
	char buf[1024];     //�������ķ�����Ϣ
} CHATADDFRIEND;

//�����û��ṹ��
typedef struct _CHATSEARCHUSER {
	char szName[64];
} CHATSEARCHUSER;

//�ļ�����ṹ��
typedef struct _CHATFILETRANS {
	
} CHATFILETRANS;

//�鿴�����¼�ṹ��
typedef struct _CHATMSGRECORD {
	char szFrom[64];
	char szTo[64];
	char szContent[1024];
} CHATMSGRECORD;

//���������û��б�
typedef struct _CHATUPDATEUSER {
	bool bAdd;
	DWORD dwLen;
	char buf[64];
} CHATUPDATEUSER;

typedef struct _CHATUPDATEFRIEND {
	bool bAdd;
	char szFriendName[64];
} CHATUPDATEFRIEND;

//--- ��������
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


