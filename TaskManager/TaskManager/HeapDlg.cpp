// HeapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "HeapDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>
#include <TlHelp32.h>

// CHeapDlg dialog

IMPLEMENT_DYNAMIC(CHeapDlg, CDialogEx)

CHeapDlg::CHeapDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CHeapDlg::IDD, pParent), m_nHeapNum(0)
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
END_MESSAGE_MAP()


// CHeapDlg message handlers

void CHeapDlg::InitControl()
{
	CRect rect;

	m_listCtrlHeap.SetExtendedStyle(m_listCtrlHeap.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlHeap.GetClientRect(&rect);
	m_listCtrlHeap.AddColumns(3,
		_T("堆ID"), rect.Width() / 3,
		_T("地址"), rect.Width() / 3,
		_T("块大小"), rect.Width() / 3);
}

void CHeapDlg::ListProcessHeap(DWORD dwPid)
{
	HEAPLIST32 heapList = {};
	TCHAR szHeapID[32] = {};
	TCHAR szHeapAddr[32] = {};
	TCHAR szHeapSize[64] = {};

	HANDLE hHeapSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPid);

	if (hHeapSnap == INVALID_HANDLE_VALUE) {
		//TODO:
		return ;
	}

	heapList.dwSize = sizeof(HEAPLIST32);
	if (Heap32ListFirst(hHeapSnap, &heapList)) {
		int i = 0;
		do {
			HEAPENTRY32 he = {};
			ZeroMemory(&he, sizeof(HEAPENTRY32));
			he.dwSize = sizeof(HEAPENTRY32);

			if (Heap32First(&he, dwPid, heapList.th32HeapID)) {
				//printf("\nHeap ID: %d\n", heapList.th32HeapID);
				do {
					//printf("Block size: %d\n", he.dwBlockSize);
					StringCchPrintf(szHeapID, _countof(szHeapID), _T("%d"), he.th32HeapID);
					StringCchPrintf(szHeapAddr, _countof(szHeapAddr), _T("0x%016X"), he.dwAddress);
					StringCchPrintf(szHeapSize, _countof(szHeapSize), _T("0x%016X"), he.dwBlockSize);
					m_listCtrlHeap.AddItems(i, 3, szHeapID, szHeapAddr, szHeapSize);
					he.dwSize = sizeof(HEAPENTRY32);
					i++;
				} while (Heap32Next(&he));
			}

			heapList.dwSize = sizeof(HEAPLIST32);
		} while (Heap32ListNext(hHeapSnap, &heapList));

		m_nHeapNum = i;
	}
	
	CloseHandle(hHeapSnap);
}

BOOL CHeapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	TCHAR szTitle[128] = {};

	InitControl();
	ListProcessHeap(m_dwPid);
	StringCchPrintf(szTitle, _countof(szTitle), _T("[%s]进程堆(%d)"), m_processName, m_nHeapNum);
	SetWindowText(szTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
