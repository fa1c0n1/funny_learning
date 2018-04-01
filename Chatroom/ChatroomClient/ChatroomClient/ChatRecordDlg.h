#pragma once
#include "afxcmn.h"


// CChatRecordDlg dialog

class CChatRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRecordDlg)

public:
	CChatRecordDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChatRecordDlg();

// Dialog Data
	enum { IDD = IDD_CHAT_RECORD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listCtrlRecord;
	void UpdateList();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
