// ThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "ThreadDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <strsafe.h>

// CThreadDlg dialog

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CThreadDlg::IDD, pParent), m_nThreadNum(0)
{

}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULE_LIST, m_threadListCtrl);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadDlg message handlers


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	TCHAR szTitle[128] = {};

	InitControl();
	ListProcessThread(m_dwOwnerPid);
	StringCchPrintf(szTitle, _countof(szTitle), _T("[%s]进程线程(%d)"), m_processName, m_nThreadNum);
	SetWindowText(szTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CThreadDlg::InitControl()
{
	CRect rect;

	m_threadListCtrl.SetExtendedStyle(m_threadListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_threadListCtrl.GetClientRect(&rect);
	m_threadListCtrl.AddColumns(2,
		_T("线程ID"), rect.Width() / 2,
		_T("优先级"), rect.Width() / 2);
}

void CThreadDlg::ListProcessThread(DWORD dwOwnerPid)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32 = {};
	TCHAR szThreadID[32] = {};
	TCHAR szThreadPri[32] = {};

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &te32))
	{
		//TODO:
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return;
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	int i = 0;
	do {
		if (te32.th32OwnerProcessID == dwOwnerPid) {
			StringCchPrintf(szThreadID, _countof(szThreadID), _T("%d"), te32.th32ThreadID);
			StringCchPrintf(szThreadPri, _countof(szThreadPri), _T("%d"), te32.tpBasePri);
			m_threadListCtrl.AddItems(i, 2, szThreadID, szThreadPri);
			i++;
		}
	} while (Thread32Next(hThreadSnap, &te32));
	m_nThreadNum = i;

	CloseHandle(hThreadSnap);
}
