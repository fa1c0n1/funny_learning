#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ChatProtocol.h"
#include <WinSock2.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

using std::vector;

class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();

	bool ConnectServer(char *szIp, WORD port);
	char *Recv();
	bool Send(CHATPURPOSE purpose, char *bufSend, DWORD dwLen);
	bool Close();

private:
	char *RecvForAnonymous();
	char *RecvForChat();
	char *RecvForOne2One();
	char *RecvForUpdateUserList();
	char *RecvForRegister();
	char *RecvForLogin();
	char *RecvForAddFriend();
	char *RecvForSearchUser();
	char *RecvForGetMsgRecord();

	void SendForAnonymous(char *bufSend, DWORD dwLen);
	void SendForChat(char *bufSend, DWORD dwLen);
	void SendForOne2One(char *bufSend, DWORD dwLen);
	void SendForRegister(char *bufSend, DWORD dwLen);
	void SendForLogin(char *bufSend, DWORD dwLen);
	void SendForAddFriend(char *bufSend, DWORD dwLen);
	void SendForSearchUser(char *bufSend, DWORD dwLen);
	void SendForGetMsgRecord(char *bufSend, DWORD dwLen);

public:
	HANDLE m_hEvent;
	vector<CHATMSGRECORD> m_vecMsgRecord;
	SOCKET m_sClient;
	CHATUPDATEUSER *m_pObjUpdate = nullptr;
	CHATONE2ONE *m_pObjOne2One = nullptr;
	char m_szName[BUFNAME];
	CHATSEND *m_pObjChatRecv = nullptr;

private:
	char m_bufRecv[BUFMSG];
};

