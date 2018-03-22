#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CHeapDlg dialog

class CHeapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHeapDlg)

public:
	CHeapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHeapDlg();

// Dialog Data
	enum { IDD = IDD_HEAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_listCtrlHeap;
	DWORD m_dwPid;
	CString m_processName;
	int m_nHeapNum;

private:
	void InitControl();
	void ListProcessHeap(DWORD dwPid);
public:
	virtual BOOL OnInitDialog();
};
