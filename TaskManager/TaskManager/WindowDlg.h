#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "BaseDialog.h"

// CWindowDlg dialog

class CWindowDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CWindowDlg)

public:
	CWindowDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWindowDlg();

// Dialog Data
	enum { IDD = IDD_WINDOW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void RefreshSelf();

private:
	void InitControl();
	void ListWindow();

	static int m_nWndCnt;
	static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam);

public:
	CMyListCtrl m_wndListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
