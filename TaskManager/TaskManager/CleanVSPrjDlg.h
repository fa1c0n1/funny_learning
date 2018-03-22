#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "afxwin.h"

// CCleanVSPrjDlg dialog

class CCleanVSPrjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanVSPrjDlg)

public:
	CCleanVSPrjDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCleanVSPrjDlg();

// Dialog Data
	enum { IDD = IDD_CLEANVS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_listCtrlFilePath;
	CEdit m_editToDelFile;
	CEdit m_editDelFile;
	CString m_strToDelFiles;
	CString m_strDelFiles;
	afx_msg void OnBnClickedGetInfoButton();
	afx_msg void OnBnClickedStartDelButton();

private:
	void InitControl();
	void ShowDelFilePathList(CString strDir, CEdit &editCtrl, CString &strEditCtrl, bool bDelete = false);
	void RefreshSelf();

	static TCHAR *m_szDelSuffix[12];
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLvnKeydownFilepathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedClearInfoButton();
};
