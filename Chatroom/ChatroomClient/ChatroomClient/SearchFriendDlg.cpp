// SearchFriendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatroomClient.h"
#include "SearchFriendDlg.h"
#include "afxdialogex.h"
#include "ChatMainDlg.h"


// CSearchFriendDlg dialog

IMPLEMENT_DYNAMIC(CSearchFriendDlg, CDialogEx)

CSearchFriendDlg::CSearchFriendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchFriendDlg::IDD, pParent)
	, m_strSearchName(_T(""))
{

}

CSearchFriendDlg::~CSearchFriendDlg()
{
}

void CSearchFriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strSearchName);
}


BEGIN_MESSAGE_MAP(CSearchFriendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, &CSearchFriendDlg::OnBnClickedSearchButton)
END_MESSAGE_MAP()


// CSearchFriendDlg message handlers


void CSearchFriendDlg::OnBnClickedSearchButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialogEx::OnOK();	
}
