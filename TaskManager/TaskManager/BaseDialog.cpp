// BaseDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "BaseDialog.h"
#include "afxdialogex.h"


// CBaseDialog dialog

IMPLEMENT_DYNAMIC(CBaseDialog, CDialogEx)

CBaseDialog::CBaseDialog(UINT nIDD, CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseDialog::IDD, pParent)
{

}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDialog, CDialogEx)
END_MESSAGE_MAP()


// CBaseDialog message handlers

void CBaseDialog::RefreshSelf()
{
}
