#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CThreadDlg dialog

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThreadDlg();

// Dialog Data
	enum { IDD = IDD_THREAD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_threadListCtrl;
	DWORD m_dwOwnerPid;
	CString m_processName;
	int m_nThreadNum;
	virtual BOOL OnInitDialog();

private:
	void InitControl();
	void ListProcessThread(DWORD dwOwnerPid);
};
