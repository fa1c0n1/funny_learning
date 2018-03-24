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
	DDX_Control(pDX, IDC_MODULE_LIST, m_listCtrlThread);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_MODULE_LIST, &CThreadDlg::OnNMRClickModuleList)
	ON_COMMAND(ID_SUBMENU_THREAD_REFRESH, &CThreadDlg::OnSubmenuThreadRefresh)
	ON_COMMAND(ID_SUBMENU_THREAD_END, &CThreadDlg::OnSubmenuThreadEnd)
	ON_COMMAND(ID_SUBMENU_THREAD_SUSPEND, &CThreadDlg::OnSubmenuThreadSuspend)
	ON_COMMAND(ID_SUBMENU_THREAD_RESUME, &CThreadDlg::OnSubmenuThreadResume)
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

	m_listCtrlThread.SetExtendedStyle(m_listCtrlThread.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlThread.GetClientRect(&rect);
	m_listCtrlThread.AddColumns(2,
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
			m_listCtrlThread.AddItems(i, 2, szThreadID, szThreadPri);
			i++;
		}
	} while (Thread32Next(hThreadSnap, &te32));
	m_nThreadNum = i;

	CloseHandle(hThreadSnap);
}

void CThreadDlg::RefreshSelf()
{
	m_listCtrlThread.DeleteAllItems();
	ListProcessThread(m_dwOwnerPid);
}

void CThreadDlg::OnNMRClickModuleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPoint point;
	DWORD dwPos = GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU3);
	CMenu *popSubMenu = menu.GetSubMenu(0);

	//弹出子菜单
	popSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

//刷新线程信息
void CThreadDlg::OnSubmenuThreadRefresh()
{
	// TODO: Add your command handler code here
	RefreshSelf();
}

void CThreadDlg::OnSubmenuThreadEnd()
{
	// TODO: Add your command handler code here
	int nSelRowIdx = m_listCtrlThread.GetSelectionMark();
	CString strBuf(_T('\0', 512)); 
	CString strTid = m_listCtrlThread.GetItemText(nSelRowIdx, 0);
	strBuf = _T("确定要结束ID号为 '");
	strBuf += strTid;
	strBuf += _T("' 的线程吗? ");

	if (MessageBox(strBuf, _T("询问"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return;

	TCHAR buf[32] = {};
	DWORD dwTid = _ttol(strTid);

	HANDLE hThread = OpenThread(THREAD_TERMINATE, NULL, dwTid);
	if (hThread) {
		if (TerminateThread(hThread, 0)) {
			RefreshSelf();
		}
		else {
			MessageBox(_T("结束线程失败"), NULL, MB_OK | MB_ICONERROR);
		}
	}
	else {
		MessageBox(_T("结束线程失败"), NULL, MB_OK | MB_ICONERROR);
	}
}

void CThreadDlg::OnSubmenuThreadSuspend()
{
	// TODO: Add your command handler code here
	TCHAR buf[32] = {};
	int nSelRowIdx = m_listCtrlThread.GetSelectionMark();
	CString strTid = m_listCtrlThread.GetItemText(nSelRowIdx, 0);
	DWORD dwTid = _ttol(strTid);

	HANDLE hThread = OpenThread(THREAD_TERMINATE, NULL, dwTid);
	if (hThread) {
		if (SuspendThread(hThread) != -1) {
			RefreshSelf();
		}
		else {
			MessageBox(_T("挂起线程失败"), NULL, MB_OK | MB_ICONERROR);
		}
	}
	else {
		MessageBox(_T("挂起线程失败"), NULL, MB_OK | MB_ICONERROR);
	}
}


void CThreadDlg::OnSubmenuThreadResume()
{
	// TODO: Add your command handler code here
	TCHAR buf[32] = {};
	int nSelRowIdx = m_listCtrlThread.GetSelectionMark();
	CString strTid = m_listCtrlThread.GetItemText(nSelRowIdx, 0);
	DWORD dwTid = _ttol(strTid);

	HANDLE hThread = OpenThread(THREAD_TERMINATE, NULL, dwTid);
	if (hThread) {
		if (ResumeThread(hThread) != -1) {
			RefreshSelf();
		}
		else {
			MessageBox(_T("恢复线程失败"), NULL, MB_OK | MB_ICONERROR);
		}
	}
	else {
		MessageBox(_T("恢复线程失败"), NULL, MB_OK | MB_ICONERROR);
	}
}
