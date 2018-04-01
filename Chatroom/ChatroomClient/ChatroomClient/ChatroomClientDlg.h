
// ChatroomClientDlg.h : header file
//

#pragma once

#include "ClientSocket.h"

// CChatroomClientDlg dialog
class CChatroomClientDlg : public CDialogEx
{
// Construction
public:
	CChatroomClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHATROOMCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoginButton();
	afx_msg void OnBnClickedRegisterButton();
	afx_msg void OnBnClickedAnonymousButton();

public:
	CClientSocket m_sClient;
	CString m_strUsername;
	CString m_strPwd;
};
