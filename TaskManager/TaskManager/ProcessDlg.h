#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CProcessDlg dialog

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessDlg();

// Dialog Data
	enum { IDD = IDD_PROCESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_processListCtrl;

	void InitControl();
	void ListProcess();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickProcessList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSubmenuEnumModules();
	afx_msg void OnSubmenuEnumThread();
	afx_msg void OnSubmenuEnumHeap();
	afx_msg void OnSubmenuRefreshProcess();
};
