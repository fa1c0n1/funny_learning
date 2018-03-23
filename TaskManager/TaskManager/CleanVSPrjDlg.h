#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "afxwin.h"
#include "BaseDialog.h"
#include <vector>

using std::vector;

// CCleanVSPrjDlg dialog

#define WM_GETINFO_REFRESH  (WM_USER+1)
#define WM_GETINFO_FINISH   (WM_USER+2)
#define WM_DELFILE_REFRESH  (WM_USER+3)
#define WM_DELFILE_FINISH   (WM_USER+4)

typedef struct _INFOPARAM {
	HWND hWnd;
	CMyListCtrl *pListCtrl;
	CEdit *pEdit;
	CString *pStrEdit;
	vector<CString> *pVtFiles;
	UINT uMsgFinish;
	bool bDelete;
} INFOPARAM, *PINFOPARAM;

class CCleanVSPrjDlg : public CBaseDialog
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

	virtual void RefreshSelf();

private:
	void InitControl();
	

	static DWORD WINAPI GetInfoThreadProc(LPVOID lpParam);
	static void ShowDelFilePathList(CString strDir, HWND hWnd, CEdit &editCtrl, 
		CString &strEditCtrl, vector<CString> &vtToDelFiles, bool bDelete = false);

	vector<CString> m_vtToDelFiles;
	static TCHAR *m_szDelSuffix[12];
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLvnKeydownFilepathList(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_btnClearInfo;
	CButton m_btnGetInfo;
	CButton m_btnStartDel;
protected:
	afx_msg LRESULT OnGetinfoFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDelfileFinish(WPARAM wParam, LPARAM lParam);
};
