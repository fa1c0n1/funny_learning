#pragma once

#include "ClientSocket.h"
#include "afxcmn.h"
#include "ChatRecordDlg.h"
#include <map>

using std::map;

// CChatMainDlg dialog

class CChatMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatMainDlg)

public:
	CChatMainDlg(CWnd* pParent = NULL);   // standard constructor
	CChatMainDlg(CClientSocket *pClientSocket, CWnd *pParent = NULL);
	virtual ~CChatMainDlg();

// Dialog Data
	enum { IDD = IDD_CHATMAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	bool m_bLogin = false;      //登录聊天还是匿名聊天
	DWORD m_dwNameIndex;
	CClientSocket *m_pClientSocket;
	CListCtrl m_listCtrlUser;
	CString m_strShow;
	CString m_strSend;

	map<CString, CDialogEx*> m_map;
	CChatRecordDlg *m_pChatRecordDlg = nullptr;
protected:
	afx_msg LRESULT OnShsocket(WPARAM wParam, LPARAM lParam);
	void ChatForOne2One(CHATONE2ONE &objOne2One);
public:
	afx_msg void OnBnClickedSendButton();
	afx_msg void OnNMDblclkOnlineList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickOnlineList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSubmenuAddfriend();
	afx_msg void OnSubmenuSearchfriend();
	afx_msg void OnSubmenuSearchrecord();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
