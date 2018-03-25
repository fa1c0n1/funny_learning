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
	ON_COMMAND_RANGE(ID_SUBMENU_REFRESH_PROCESS, ID_SUBMENU_ENUM_HEAP, &CProcessDlg::OnSubmenuProcess)
	ON_COMMAND(ID_SUBMENU_END_PROCESS, &CProcessDlg::OnSubmenuEndProcess)
END_MESSAGE_MAP()

// CProcessDlg message handlers

void CProcessDlg::InitControl()
{
	CRect rect;

	m_listCtrlProcess.SetExtendedStyle(m_listCtrlProcess.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlProcess.GetClientRect(&rect);
	m_listCtrlProcess.AddColumns(3,
		_T("映像名称"), rect.Width() / 4,
		_T("PID"), rect.Width() / 4,
		_T("PPID"), rect.Width() / 4);
	m_listCtrlProcess.InsertColumn(4, _T("路径"), LVCFMT_LEFT, rect.Width() / 4 * 3, 4);
}

void CProcessDlg::ListProcess()
{
	HANDLE hProcessSnap = 0;
	PROCESSENTRY32 pe32 = {};
	TCHAR szPidBuf[64] = { 0 };
	TCHAR szPPidBuf[64] = { 0};
	CString strProcessFullPath(_T('\0'), MAX_PATH);
	
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
			if (pe32.th32ProcessID == GetCurrentProcessId()) {
				continue;
			}
			else if (pe32.th32ProcessID == 0) {
				strProcessFullPath = _T("无法获取");
			}
			else {
				//获取进程对应程序的路径
				HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
				if (hModuleSnap == INVALID_HANDLE_VALUE) {
					strProcessFullPath = _T("无法获取");
				}
				else {
					MODULEENTRY32 me32 = {};
					me32.dwSize = sizeof(MODULEENTRY32);
					if (!Module32First(hModuleSnap, &me32)) {
						//TODO:
						strProcessFullPath = _T("无法获取");
					}
					else {
						strProcessFullPath = me32.szExePath;
					}
					CloseHandle(hModuleSnap);
				}
			}

			StringCchPrintf(szPidBuf, _countof(szPidBuf), _T("%d"), pe32.th32ProcessID);
			StringCchPrintf(szPPidBuf, _countof(szPPidBuf), _T("%d"), pe32.th32ParentProcessID);
			m_listCtrlProcess.AddItems(i, 4, pe32.szExeFile, szPidBuf, szPPidBuf, strProcessFullPath);
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

void CProcessDlg::RefreshSelf()
{
	m_listCtrlProcess.DeleteAllItems();
	ListProcess();
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

	int nSelRowIdx = m_listCtrlProcess.GetSelectionMark();
	TCHAR buf[32] = {};
	CString strPid = m_listCtrlProcess.GetItemText(nSelRowIdx, 1);
	DWORD dwPid = _ttol(strPid);
	if (dwPid == 0)
		return;

	CPoint point;
	DWORD dwPos = GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu *popSubMenu = menu.GetSubMenu(0);

	//弹出子菜单
	popSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CProcessDlg::OnSubmenuProcess(UINT uID)
{
	int nSelRowIdx = m_listCtrlProcess.GetSelectionMark();
	TCHAR buf[32] = {};
	CString strPid = m_listCtrlProcess.GetItemText(nSelRowIdx, 1);
	DWORD dwPid = _ttol(strPid);
	INT_PTR nRet = 0;

	switch (uID)
	{
	case ID_SUBMENU_REFRESH_PROCESS:
		RefreshSelf();
		break;
	case ID_SUBMENU_ENUM_MODULES: {
		CModuleDlg moduleDlg;
		moduleDlg.m_dwPid = dwPid;
		moduleDlg.m_processName = m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
		nRet = moduleDlg.DoModal();

		if (nRet == IDCANCEL)
			return;
		break;
	}
	case ID_SUBMENU_ENUM_THREAD: {
		CThreadDlg threadDlg;
		threadDlg.m_dwOwnerPid = dwPid;
		threadDlg.m_processName = m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
		nRet = threadDlg.DoModal();

		if (nRet == IDCANCEL)
			return;
		break;
	}
	case ID_SUBMENU_ENUM_HEAP: {
		CHeapDlg heapDlg;
		heapDlg.m_dwPid = dwPid;
		heapDlg.m_processName = m_listCtrlProcess.GetItemText(nSelRowIdx, 0);
		nRet = heapDlg.DoModal();

		if (nRet == IDCANCEL)
			return;
		break;
	}
	default:
		break;
	}
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

