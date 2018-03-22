#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CModuleDlg dialog

class CModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModuleDlg)

public:
	CModuleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModuleDlg();

// Dialog Data
	enum { IDD = IDD_MODULE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_moduleListCtrl;
	DWORD m_dwPid;
	CString m_processName;
	int m_nModuleNum;
	virtual BOOL OnInitDialog();

private:
	void InitControl();
	void ListProcessModule(DWORD dwPid);
};
