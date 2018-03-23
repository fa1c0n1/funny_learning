#pragma once


// CBaseDialog dialog

class CBaseDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDialog)

public:
	CBaseDialog(UINT nIDD, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBaseDialog();

// Dialog Data
	enum { IDD = IDD_BASEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual void RefreshSelf();
};
