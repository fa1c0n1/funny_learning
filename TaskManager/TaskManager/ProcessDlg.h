#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "BaseDialog.h"

// CProcessDlg dialog

class CProcessDlg : public CBaseDialog
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
	CMyListCtrl m_listCtrlProcess;

	virtual BOOL OnInitDialog();
	virtual void RefreshSelf();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickProcessList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSubmenuProcess(UINT uID);


private:
	void InitControl();
	void ListProcess();
public:
	afx_msg void OnSubmenuEndProcess();
};
