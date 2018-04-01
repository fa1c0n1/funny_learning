#pragma once


// COne2OneDlg dialog

class COne2OneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COne2OneDlg)

public:
	COne2OneDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COne2OneDlg();

// Dialog Data
	enum { IDD = IDD_ONE2ONE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strShow;
	CString m_strSend;
	afx_msg void OnBnClickedSendButton();
	afx_msg void OnClose();
};
