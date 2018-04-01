

#include "stdafx.h"
#include "ClientSocket.h"
#include <cstring>

CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}


bool CClientSocket::ConnectServer(char *szIp, WORD port)
{
	//1.初始化套接字动态库
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	//检查返回的库版本是否为 2.2
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2) {
		WSACleanup();
		return false;
	}

	//2.创建套接字
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}

	//3.设置服务器地址
	SOCKADDR_IN servAddr = {};
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(szIp);
	servAddr.sin_port = htons(port);

	int nRet = connect(m_sClient, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR) {
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}

	return true;
}

char *CClientSocket::Recv()
{
	CHATSEND ct = {};

	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL)) {
		return nullptr;
	}

	CHATSEND mmm = {};
    DWORD nSize = sizeof(mmm);
	nSize = sizeof(mmm.m_content);
	nSize = sizeof(mmm.m_type);

	m_pObjChatRecv = &ct;
	switch (ct.m_type)
	{
	case ANONYMOUS:
		return RecvForAnonymous();
	case CHAT:
		return RecvForChat();
	case ONE2ONE:
		return RecvForOne2One();
	case REGISTER:
		return RecvForRegister();
	case LOGIN:
		return RecvForLogin();
	case ADDFRIEND:
		return RecvForAddFriend();
	case SEARCHUSER:
		return RecvForSearchUser();
	case FILETRANS:
		break;
	case MSGRECORD:
		return RecvForGetMsgRecord();
	case UPDATEUSER:
		return RecvForUpdateUserList();
	default:
		break;
	}

	return nullptr;
}

bool CClientSocket::Send(CHATPURPOSE purpose, char *bufSend, DWORD dwLen)
{
	switch (purpose)
	{
	case ANONYMOUS:
		SendForAnonymous(bufSend, dwLen);
		break;
	case CHAT:
		SendForChat(bufSend, dwLen);
		break;
	case ONE2ONE:
		SendForOne2One(bufSend, dwLen);
		break;
	case REGISTER:
		SendForRegister(bufSend, dwLen);
		break;
	case LOGIN:
		SendForLogin(bufSend, dwLen);
		break;
	case ADDFRIEND:
		SendForAddFriend(bufSend, dwLen);
		break;
	case SEARCHUSER:
		SendForSearchUser(bufSend, dwLen);
		break;
	case FILETRANS:
		break;
	case MSGRECORD:
		SendForGetMsgRecord(bufSend, dwLen);
		break;
	default:
		break;
	}

	return true;
}

bool CClientSocket::Close()
{
	WSACleanup();
	closesocket(m_sClient);
	return true;
}

char *CClientSocket::RecvForAnonymous()
{
	sprintf_s(m_bufRecv, BUFMSG, "%s加入聊天室! \n", m_pObjChatRecv->m_content.any.buf);
	return m_bufRecv;
}

char *CClientSocket::RecvForChat()
{
	//解密
	for (int i = 0; i < m_pObjChatRecv->m_content.chat.dwLen; i++) {
		m_pObjChatRecv->m_content.chat.buf[i] ^= 15;
	}

	strcpy_s(m_bufRecv, m_pObjChatRecv->m_content.chat.buf);
	return m_bufRecv;
}

char *CClientSocket::RecvForOne2One()
{
	m_pObjOne2One = new CHATONE2ONE{};
	memcpy_s(m_pObjChatRecv, sizeof(CHATONE2ONE), &m_pObjChatRecv->m_content.o2o, sizeof(CHATONE2ONE));
	return nullptr;
}

char *CClientSocket::RecvForUpdateUserList()
{
	//新用户加入，更新到用户列表窗口
	m_pObjUpdate = new CHATUPDATEUSER;
	memcpy_s(m_pObjChatRecv, sizeof(CHATUPDATEUSER), &m_pObjChatRecv->m_content.upd, sizeof(CHATUPDATEUSER));
	return nullptr;
}

char *CClientSocket::RecvForRegister()
{
	if (!strcmp(m_pObjChatRecv->m_content.buf, "注册成功!"))
		return "注册成功!";
	else
		return nullptr;
}

char *CClientSocket::RecvForLogin()
{
	if (!strcmp(m_pObjChatRecv->m_content.login.szName, "登录成功!"))
		return "登录成功!";
	else
		return nullptr;
}

char *CClientSocket::RecvForAddFriend()
{
	MessageBoxA(NULL, m_pObjChatRecv->m_content.adf.szName, "添加好友", MB_OK);
	return nullptr;
}

char *CClientSocket::RecvForSearchUser()
{
	MessageBoxA(NULL, m_pObjChatRecv->m_content.seu.szName, "搜索用户", MB_OK);
	return nullptr;
}

char *CClientSocket::RecvForGetMsgRecord()
{
	if (!strcmp(m_pObjChatRecv->m_content.rec.szFrom, "~~~end~~~"))
		SetEvent(m_hEvent);
	else
		m_vecMsgRecord.push_back(m_pObjChatRecv->m_content.rec);

	return nullptr;
}

void CClientSocket::SendForAnonymous(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { ANONYMOUS };
	int nLen = sizeof(ct.m_content.chat.buf);
	strcpy_s(ct.m_content.chat.buf, nLen, bufSend);
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForChat(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { CHAT };
	strcpy_s(ct.m_content.chat.buf, m_szName);
	strcat_s(ct.m_content.chat.buf, ":");
	strcat_s(ct.m_content.chat.buf, bufSend);
	ct.m_content.chat.dwLen = strlen(ct.m_content.chat.buf) + 1;

	//加密
	for (int i = 0; i < ct.m_content.any.dwLen; i++) {
		ct.m_content.chat.buf[i] ^= 15;
	}

	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForOne2One(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { ONE2ONE };
	char *nextToken = nullptr;
	char *szContext = strtok_s(bufSend, ":", &nextToken);
	memcpy_s(ct.m_content.o2o.szName, nextToken - bufSend, bufSend, nextToken - bufSend);
	memcpy_s(ct.m_content.o2o.szContent, strlen(nextToken), nextToken, strlen(nextToken));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForRegister(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { REGISTER };
	char *pPwd = nullptr;
	strtok_s(bufSend, ":", &pPwd);
	memcpy_s(ct.m_content.reg.szName, _countof(ct.m_content.reg.szName), bufSend, strlen(bufSend));
	memcpy_s(ct.m_content.reg.szPwd, _countof(ct.m_content.reg.szPwd), pPwd, strlen(pPwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForLogin(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { LOGIN };
	char *pPwd = nullptr;
	strtok_s(bufSend, ":", &pPwd);
	memcpy_s(ct.m_content.login.szName, pPwd - bufSend, bufSend, pPwd - bufSend);
	memcpy_s(ct.m_content.login.szPwd, strlen(pPwd), pPwd, strlen(pPwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForAddFriend(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { ADDFRIEND };
	char *pFriend = nullptr;
	strtok_s(bufSend, ":", &pFriend);
	memcpy_s(ct.m_content.adf.szName, pFriend - bufSend, bufSend, pFriend - bufSend);
	memcpy_s(ct.m_content.adf.szFriendName, strlen(pFriend), pFriend, strlen(pFriend));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForSearchUser(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { SEARCHUSER };
	memcpy_s(ct.m_content.seu.szName, dwLen, bufSend, dwLen);
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForGetMsgRecord(char *bufSend, DWORD dwLen)
{
	CHATSEND ct = { MSGRECORD };
	// 服务器保存有当前客户端的名称，所以查询所有的聊天记录只需要发送个消息类型就可以了
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}
