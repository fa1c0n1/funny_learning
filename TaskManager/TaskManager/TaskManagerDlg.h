
// TaskManagerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "ProcessDlg.h"
#include "WindowDlg.h"


// CTaskManagerDlg dialog
class CTaskManagerDlg : public CDialogEx
{
// Construction
public:
	CTaskManagerDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CTaskManagerDlg();

// Dialog Data
	enum { IDD = IDD_TASKMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabAllWnd;
	CDialogEx *m_childTab[4];

	afx_msg void OnSize(UINT nType, int cx, int cy);

	void InitTabCtrl();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
