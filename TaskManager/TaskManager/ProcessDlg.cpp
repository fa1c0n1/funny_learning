// ProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include "ModuleDlg.h"
#include "ThreadDlg.h"
#include "HeapDlg.h"
#include "BaseDialog.h"
#include <strsafe.h>
#include <TlHelp32.h>

// CProcessDlg dialog

IMPLEMENT_DYNAMIC(CProcessDlg, CBaseDialog)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/) 
: CBaseDialog(CProcessDlg::IDD, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrlProcess);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CBaseDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcessDlg::OnNMRClickProcessList)
	ON_COMMAND(ID_SUBMENU_ENUM_MODULES, &CProcessDlg::OnSubmenuEnumModules)
	ON_COMMAND(ID_SUBMENU_ENUM_THREAD, &CProcessDlg::OnSubmenuEnumThread)
	ON_COMMAND(ID_SUBMENU_ENUM_HEAP, &CProcessDlg::OnSubmenuEnumHeap)
	ON_COMMAND(ID_SUBMENU_REFRESH_PROCESS, &CProcessDlg::OnSubmenuRefreshProcess)
	ON_COMMAND(ID_SUBMENU_END_PROCESS, &CProcessDlg::OnSubmenuEndProcess)
END_MESSAGE_MAP()


// CProcessDlg message handlers

void CProcessDlg::InitControl()
{
	CRect rect;

	m_listCtrlProcess.SetExtendedStyle(m_listCtrlProcess.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlProcess.GetClientRect(&rect);
	m_listCtrlProcess.AddColumns(2,
		_T("映像名称"), rect.Width() / 2,
		_T("PID"), rect.Width() / 2);
}

void CProcessDlg::ListProcess()
{
	HANDLE hProcessSnap = 0;
	PROCESSENTRY32 pe32 = {};
	TCHAR szPidBuf[32] = { 0 };
	int i = 0;

	if ((hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hProcessSnap, &pe32);

	if (!bRet) {
		//TODO:	
		CloseHandle(hProcessSnap);
		return;
	}
	else {
		do {
			if (pe32.th32ProcessID == GetCurrentProcessId())
				continue;

			StringCchPrintf(szPidBuf, _countof(szPidBuf), _T("%d"), pe32.th32ProcessID);
			m_listCtrlProcess.AddItems(i, 2, pe32.szExeFile, szPidBuf);
			i++;
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);
	}
}


BOOL CProcessDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();
	ListProcess(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect clientRect;
	GetClientRect(&clientRect);

	clientRect.top += 10;
	clientRect.bottom -= 10;
	clientRect.left += 10;
	clientRect.right -= 10;

	if (m_listCtrlProcess)
		m_listCtrlProcess.MoveWindow(&clientRect);
}


void CProcessDlg::OnNMRClickProcessList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CPoint point;
	DWORD dwPos = GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu *popSubMenu = menu.GetSubMenu(0);

	//弹出子菜单
	popSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CProcessDlg::OnSubmenuEnumModules()
{
	// TODO: Add your command handler code here
	int nSelRowIdx = m_listCtrlProcess.GetSelectionMark();
	TCHAR buf[32] = {};

	CString strPid = m_listCtrlProcess.GetItemText(nSelRowIdx, 1);
	DWORD dwPid = _ttol(strPid);

	CModuleDlg moduleDlg;
	moduleDlg.m_dwPid = dwPid;
	moduleDlg.m_processName = m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
	INT_PTR nRet = moduleDlg.DoModal();

	if (nRet == IDCANCEL)
		return;
}

void CProcessDlg::OnSubmenuEnumThread()
{
	// TODO: Add your command handler code here
	int nSelRowIdx = m_listCtrlProcess.GetSelectionMark();
	TCHAR buf[32] = {};

	CString strPid = m_listCtrlProcess.GetItemText(nSelRowIdx, 1);
	DWORD dwPid = _ttol(strPid);

	CThreadDlg threadDlg;
	threadDlg.m_dwOwnerPid = dwPid;
	threadDlg.m_processName = m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
	INT_PTR nRet = threadDlg.DoModal();

	if (nRet == IDCANCEL)
		return;
}

void CProcessDlg::OnSubmenuEnumHeap()
{
	// TODO: Add your command handler code here
	int nSelRowIdx = m_listCtrlProcess.GetSelectionMark();
	TCHAR buf[32] = {};

	CString strPid = m_listCtrlProcess.GetItemText(nSelRowIdx, 1);
	DWORD dwPid = _ttol(strPid);

	CHeapDlg heapDlg;
	heapDlg.m_dwPid = dwPid;
	heapDlg.m_processName = m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
	INT_PTR nRet = heapDlg.DoModal();

	if (nRet == IDCANCEL)
		return;
}


void CProcessDlg::OnSubmenuRefreshProcess()
{
	// TODO: Add your command handler code here
	RefreshSelf();
}

void CProcessDlg::OnSubmenuEndProcess()
{
	// TODO: Add your command handler code here
	int nSelRowIdx = m_listCtrlProcess.GetSelectionMark();
	CString strBuf(_T('\0', 512)); 
	strBuf = _T("确定要结束 '");
	strBuf += m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
	strBuf += _T("' 吗? ");

	if (MessageBox(strBuf, _T("询问"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return;

	TCHAR buf[32] = {};
	CString strPid = m_listCtrlProcess.GetItemText(nSelRowIdx, 1);
	DWORD dwPid = _ttol(strPid);

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, NULL, dwPid);
	if (hProcess) { //
		if (TerminateProcess(hProcess, 0)) {
			m_listCtrlProcess.DeleteItem(nSelRowIdx);
		}
		else {
			MessageBox(_T("结束进程失败"), NULL, MB_OK | MB_ICONERROR);
		}
	}
	else {
		MessageBox(_T("结束进程失败"), NULL, MB_OK | MB_ICONERROR);
	}
}

void CProcessDlg::RefreshSelf()
{
	m_listCtrlProcess.DeleteAllItems();
	ListProcess();
}