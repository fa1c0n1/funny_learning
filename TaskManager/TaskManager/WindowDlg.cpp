// WindowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "WindowDlg.h"
#include "afxdialogex.h"


// CWindowDlg dialog

IMPLEMENT_DYNAMIC(CWindowDlg, CDialogEx)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowDlg::IDD, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDOW_LIST, m_wndListCtrl);

	RefreshSelf();
}


BEGIN_MESSAGE_MAP(CWindowDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWindowDlg message handlers


BOOL CWindowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();
	ListWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWindowDlg::InitControl()
{
	CRect rect;

	m_wndListCtrl.SetExtendedStyle(m_wndListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_wndListCtrl.GetClientRect(&rect);
	m_wndListCtrl.AddColumns(1,
		_T("窗口名"), rect.Width());
}

BOOL CALLBACK EnumWndProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	static int nCnt = 0;
	TCHAR szWndName[1024] = {};

	CMyListCtrl *pWndList = (CMyListCtrl *)lParam;

	GetWindowText(hwnd, szWndName, _countof(szWndName));
	if (IsWindowVisible(hwnd) == TRUE && wcslen(szWndName) > 0) {
		//窗口没有被隐藏且窗口标题长度不为0, 则显示窗口信息
		pWndList->AddItems(nCnt, 1, szWndName);
		nCnt++;
	}

	return TRUE;
}

void CWindowDlg::ListWindow()
{
	EnumWindows(&EnumWndProc, (LPARAM)&m_wndListCtrl);
}

void CWindowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect clientRect;
	GetClientRect(&clientRect);

	clientRect.top += 10;
	clientRect.bottom -= 10;
	clientRect.left += 10;
	clientRect.right -= 10;

	if (m_wndListCtrl)
		m_wndListCtrl.MoveWindow(&clientRect);
}

void CWindowDlg::RefreshSelf()
{
	m_wndListCtrl.DeleteAllItems();
	ListWindow();
}
