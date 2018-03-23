#pragma once

#include "BaseDialog.h"
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
};
