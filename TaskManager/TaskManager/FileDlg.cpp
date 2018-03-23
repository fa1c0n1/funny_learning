// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "FileDlg.h"
#include "afxdialogex.h"


// CFileDlg dialog

IMPLEMENT_DYNAMIC(CFileDlg, CBaseDialog)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CFileDlg::IDD, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileDlg, CBaseDialog)
END_MESSAGE_MAP()


// CFileDlg message handlers


void CFileDlg::RefreshSelf()
{
	//TODO:
}
