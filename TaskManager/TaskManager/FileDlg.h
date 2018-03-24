#pragma once

#include "BaseDialog.h"
#include "afxcmn.h"
#include "afxshelltreectrl.h"
#include "MyListCtrl.h"
#include "afxwin.h"
#include "afxshelllistctrl.h"
// CFileDlg dialog

class CFileDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileDlg();

// Dialog Data
	enum { IDD = IDD_FILE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void RefreshSelf();

private:
	void InitControl();
	void TraverseDirectory(CString strPath);

	static UINT PER_GB_BYTES;
	static UINT PER_MB_BYTES;
	static UINT PER_KB_BYTES;
public:
	CMFCShellTreeCtrl m_mfcShellTreeCtrl;
	CMyListCtrl m_listCtrlFiles;
	CEdit m_editFullPath;

	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedMfcshelltree(NMHDR *pNMHDR, LRESULT *pResult);
};
