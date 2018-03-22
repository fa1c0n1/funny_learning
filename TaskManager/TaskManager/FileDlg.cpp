// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "FileDlg.h"
#include "afxdialogex.h"


// CFileDlg dialog

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileDlg::IDD, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileDlg message handlers
