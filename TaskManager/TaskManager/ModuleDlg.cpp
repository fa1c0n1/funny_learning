// ModuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "ModuleDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>
#include "Comm.h"
#include <TlHelp32.h>

// CModuleDlg dialog

IMPLEMENT_DYNAMIC(CModuleDlg, CDialogEx)

CModuleDlg::CModuleDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CModuleDlg::IDD, pParent), m_nModuleNum(0)
{

}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULE_LIST, m_listCtrlModule);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialogEx)
END_MESSAGE_MAP()


// CModuleDlg message handlers

void CModuleDlg::ListProcessModule(DWORD dwPid)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = {};
	TCHAR szModuleBaseAddr[32] = {};
	TCHAR szModuleSize[64] = {};

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		return;
	}

	me32.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hModuleSnap, &me32)) {
		//TODO:
		CloseHandle(hModuleSnap);
		return;
	}
	else {
		int i = 0;
		do
		{
			StringCchPrintf(szModuleBaseAddr, _countof(szModuleBaseAddr), _T("0x%016X"), (LONG64)me32.modBaseAddr);
			StringCchPrintf(szModuleSize, _countof(szModuleSize), _T("0x%016X"), me32.modBaseSize);
			int n = sizeof(LONG64);
			m_listCtrlModule.AddItems(i, 3, me32.szExePath, szModuleBaseAddr, szModuleSize);
			i++;
		} while (Module32Next(hModuleSnap, &me32));

		m_nModuleNum = i;

		CloseHandle(hModuleSnap);
	}
}

BOOL CModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	TCHAR szTitle[128] = {};

	InitControl();
	ListProcessModule(m_dwPid);
	StringCchPrintf(szTitle, _countof(szTitle), _T("[%s]进程模块(%d)"), m_processName, m_nModuleNum);
	SetWindowText(szTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModuleDlg::InitControl()
{
	CRect rect;

	m_listCtrlModule.SetExtendedStyle(m_listCtrlModule.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlModule.GetClientRect(&rect);
	m_listCtrlModule.AddColumns(3,
		_T("模块路径"), rect.Width() / 3,
		_T("基地址"), rect.Width() / 3, 
		_T("大小"), rect.Width() / 3);
}
