#pragma once


// CSearchFriendDlg dialog

class CSearchFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchFriendDlg)

public:
	CSearchFriendDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchFriendDlg();

// Dialog Data
	enum { IDD = IDD_SEARCHFRIEND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFriendName;
	afx_msg void OnBnClickedSearchButton();
};
