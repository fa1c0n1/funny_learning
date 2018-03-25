// HeapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "HeapDlg.h"
#include "afxdialogex.h"
#include "Comm.h"
#include <strsafe.h>
#include <TlHelp32.h>

// CHeapDlg dialog

IMPLEMENT_DYNAMIC(CHeapDlg, CDialogEx)

CHeapDlg::CHeapDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CHeapDlg::IDD, pParent), m_nHeapNum(0), m_bForbitClose(true)
{

}

CHeapDlg::~CHeapDlg()
{
}

void CHeapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAP_LIST, m_listCtrlHeap);
}


BEGIN_MESSAGE_MAP(CHeapDlg, CDialogEx)
	ON_MESSAGE(WM_USER_TRAVERSEHEAP_FINISH, &CHeapDlg::OnUserTraverseHeapFinish)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CHeapDlg message handlers

//初始化控件
void CHeapDlg::InitControl()
{
	CRect rect;

	m_listCtrlHeap.SetExtendedStyle(m_listCtrlHeap.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlHeap.GetClientRect(&rect);
	m_listCtrlHeap.AddColumns(4,
		_T("堆ID"), rect.Width() / 4,
		_T("PID"), rect.Width() / 4,
		_T("地址"), rect.Width() / 4,
		_T("块大小"), rect.Width() / 4);

	SetWindowText(_T("该进程堆数目较多，正在加速获取中， 等稍后..."));

	//将窗口关闭按钮设置为无效状态
	CMenu *pMenu = GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
}

//遍历指定进程的堆,并显示在列表控件
void CHeapDlg::ListProcessHeap(HWND hWnd, DWORD dwPid, CMyListCtrl &listCtrl)
{
	HEAPLIST32 heapList = {};
	TCHAR szHeapID[32] = {};
	TCHAR szPID[32] = {};
	TCHAR szHeapAddr[32] = {};
	TCHAR szHeapSize[64] = {};
	int i = 0;

	HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPid);

	if (hHeapSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		::PostMessage(hWnd, WM_USER_TRAVERSEHEAP_FINISH, 0, (LPARAM)i);
		return ;
	}

	heapList.dwSize = sizeof(HEAPLIST32);
	if (Heap32ListFirst(hHeapSnap, &heapList)) {
		i = 0;
		do {
			HEAPENTRY32 he = {};
			ZeroMemory(&he, sizeof(HEAPENTRY32));
			he.dwSize = sizeof(HEAPENTRY32);

			if (Heap32First(&he, dwPid, heapList.th32HeapID)) {
				do {
					StringCchPrintf(szHeapID, _countof(szHeapID), _T("%d"), he.th32HeapID);
					StringCchPrintf(szPID, _countof(szPID), _T("%d"), he.th32ProcessID);
					StringCchPrintf(szHeapAddr, _countof(szHeapAddr), _T("0x%016X"), he.dwAddress);
					StringCchPrintf(szHeapSize, _countof(szHeapSize), _T("0x%016X"), he.dwBlockSize);
					listCtrl.AddItems(i, 4, szHeapID, szPID, szHeapAddr, szHeapSize);
					he.dwSize = sizeof(HEAPENTRY32);
					i++;
				} while (Heap32Next(&he) && i <= 1000);
			}

			heapList.dwSize = sizeof(HEAPLIST32);
		} while (Heap32ListNext(hHeapSnap, &heapList) && i <= 1000);
	}
	
	//结束后，向消息队列投递自定义消息
	::PostMessage(hWnd, WM_USER_TRAVERSEHEAP_FINISH, 0, (LPARAM)i);
	CloseHandle(hHeapSnap);
}

BOOL CHeapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	InitControl();

	//填充要传入线程的数据
	PHEAPPROCINFO pHeapProcInfo = new HEAPPROCINFO{};
	pHeapProcInfo->hwnd = m_hWnd;
	pHeapProcInfo->pListCtrl = &m_listCtrlHeap;
	pHeapProcInfo->dwPid = m_dwPid;
	//开启子线程去遍历进程的堆
	AfxBeginThread(TraverseHeapProc, (LPVOID)pHeapProcInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//子线程处理函数
UINT __cdecl CHeapDlg::TraverseHeapProc(LPVOID pParam)
{
	PHEAPPROCINFO pProcInfo = (PHEAPPROCINFO)pParam;
	ListProcessHeap(pProcInfo->hwnd, pProcInfo->dwPid, *pProcInfo->pListCtrl);

	//遍历结束后，释放前面创建的堆内存,并结束掉当前线程
	if (pProcInfo) {
		delete pProcInfo;
		pProcInfo = nullptr;
	}

	AfxEndThread(0);
	return 0;
}


afx_msg LRESULT CHeapDlg::OnUserTraverseHeapFinish(WPARAM wParam, LPARAM lParam)
{
	TCHAR szTitle[128] = {};
	StringCchPrintf(szTitle, _countof(szTitle), _T("[%s]进程堆(%d)"), m_processName, lParam);
	SetWindowText(szTitle);
	CMenu *pMenu = GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
	m_bForbitClose = false;
	return 0;
}


BOOL CHeapDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bForbitClose) {
		if (pMsg->message == WM_KEYDOWN) {
			//屏蔽 Enter 键和 Esc 键关闭当前窗口
			switch (pMsg->wParam)
			{
			case VK_RETURN:
				return TRUE;
			case VK_ESCAPE:
				return TRUE;
			default:
				break;
			}
		}
		else if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

