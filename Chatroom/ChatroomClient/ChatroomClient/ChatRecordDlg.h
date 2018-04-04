#pragma once

#include "afxcmn.h"
#include "ChatProtocol.h"
#include <vector>

using std::vector;

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
	virtual BOOL OnInitDialog();
	void UpdateRecord(vector<CHATMSGRECORD> &vtChatRecord);

	static UINT __cdecl UpdateListProc(LPVOID pParam);
	static void UpdateList(CChatRecordDlg *pChatRecordDlg, vector<CHATMSGRECORD> &vtChatRecord);
};

typedef struct _RECORDPROCINFO {
	CChatRecordDlg *pChatRecordDlg;
	vector<CHATMSGRECORD> *pVtRecord;
} RECORDPROCINFO, *PRECORDPROCINFO;
