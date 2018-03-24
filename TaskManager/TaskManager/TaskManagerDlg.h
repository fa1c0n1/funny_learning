
// TaskManagerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "BaseDialog.h"

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
	CMenu m_menuMain;
	HACCEL m_hAccel;
	CStatusBar m_wndStatusBar;
	CTabCtrl m_tabAllWnd;
	CBaseDialog *m_childTab[4];

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	void InitControl();

private:
	static UINT BASED_CODE indicators[2];

	static double FILETIME2Double(const _FILETIME &fileTime);
	static int GetCpuUsage();
	static DWORD GetMemoryUsage();
	static DWORD WINAPI UsageProc(LPVOID lpParam);

	void PrivilegeEscalation();
	void SetHotKey();

protected:
	afx_msg LRESULT OnUserUpdateCpuUsage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserUpdateMemUsage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSubmenueLockscreen();
	afx_msg void OnSubmenuLogoff();
	afx_msg void OnSubmenuHibernate();
	afx_msg void OnSubMenuSleep();
	afx_msg void OnSubmenuRestart();
	afx_msg void OnSubmenuShutdown();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
