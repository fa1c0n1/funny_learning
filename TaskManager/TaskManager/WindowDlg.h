#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CWindowDlg dialog

class CWindowDlg : public CDialogEx
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

private:
	void InitControl();
	void ListWindow();
public:
	CMyListCtrl m_wndListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
