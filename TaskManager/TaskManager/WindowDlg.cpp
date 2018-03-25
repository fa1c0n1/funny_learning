// WindowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "WindowDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>

int CWindowDlg::m_nWndCnt = 0;

// CWindowDlg dialog

IMPLEMENT_DYNAMIC(CWindowDlg, CBaseDialog)

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CWindowDlg::IDD, pParent)
{

}

CWindowDlg::~CWindowDlg()
{
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WINDOW_LIST, m_wndListCtrl);
}


BEGIN_MESSAGE_MAP(CWindowDlg, CBaseDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWindowDlg message handlers


BOOL CWindowDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//初始化控件
void CWindowDlg::InitControl()
{
	CRect rect;

	m_wndListCtrl.SetExtendedStyle(m_wndListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_wndListCtrl.GetClientRect(&rect);
	m_wndListCtrl.AddColumns(2,
		_T("窗口标题"), rect.Width() / 3 * 2,
		_T("窗口句柄"), rect.Width() / 3);
	m_wndListCtrl.InsertColumn(3, _T("窗口类名"), LVCFMT_LEFT, rect.Width() / 3, 3);
}

BOOL CALLBACK CWindowDlg::EnumWndProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	CMyListCtrl *pWndList = (CMyListCtrl *)lParam;
	TCHAR szWndName[1024] = {};
	
	//获取窗口标题
	::GetWindowText(hwnd, szWndName, _countof(szWndName));
	if (::IsWindowVisible(hwnd) == TRUE && wcslen(szWndName) > 0) {
		//窗口没有被隐藏且窗口标题长度不为0, 则显示窗口信息
		TCHAR szWndClassName[1024] = {0};
		TCHAR szWndHandle[32] = {0};

		//获取窗口的类名
		GetClassName(hwnd, szWndClassName, _countof(szWndClassName));
		//格式化窗口句柄
		StringCchPrintf(szWndHandle, _countof(szWndHandle), _T("0x%016X"), hwnd);
		pWndList->AddItems(m_nWndCnt, 3, szWndName, szWndHandle, szWndClassName);
		m_nWndCnt++;
	}

	return TRUE;
}

//遍历窗口
void CWindowDlg::ListWindow()
{
	m_nWndCnt = 0;
	EnumWindows(&EnumWndProc, (LPARAM)&m_wndListCtrl);
}

void CWindowDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

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

//刷新窗口列表
void CWindowDlg::RefreshSelf()
{
	m_wndListCtrl.DeleteAllItems();
	ListWindow();
}
