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

//��ʼ���ؼ�
void CHeapDlg::InitControl()
{
	CRect rect;

	m_listCtrlHeap.SetExtendedStyle(m_listCtrlHeap.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlHeap.GetClientRect(&rect);
	m_listCtrlHeap.AddColumns(4,
		_T("��ID"), rect.Width() / 4,
		_T("PID"), rect.Width() / 4,
		_T("��ַ"), rect.Width() / 4,
		_T("���С"), rect.Width() / 4);

	SetWindowText(_T("�ý��̶���Ŀ�϶࣬���ڼ��ٻ�ȡ�У� ���Ժ�..."));

	//�����ڹرհ�ť����Ϊ��Ч״̬
	CMenu *pMenu = GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
}

//����ָ�����̵Ķ�,����ʾ���б�ؼ�
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
	
	//����������Ϣ����Ͷ���Զ�����Ϣ
	::PostMessage(hWnd, WM_USER_TRAVERSEHEAP_FINISH, 0, (LPARAM)i);
	CloseHandle(hHeapSnap);
}

BOOL CHeapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	InitControl();

	//���Ҫ�����̵߳�����
	PHEAPPROCINFO pHeapProcInfo = new HEAPPROCINFO{};
	pHeapProcInfo->hwnd = m_hWnd;
	pHeapProcInfo->pListCtrl = &m_listCtrlHeap;
	pHeapProcInfo->dwPid = m_dwPid;
	//�������߳�ȥ�������̵Ķ�
	AfxBeginThread(TraverseHeapProc, (LPVOID)pHeapProcInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//���̴߳�����
UINT __cdecl CHeapDlg::TraverseHeapProc(LPVOID pParam)
{
	PHEAPPROCINFO pProcInfo = (PHEAPPROCINFO)pParam;
	ListProcessHeap(pProcInfo->hwnd, pProcInfo->dwPid, *pProcInfo->pListCtrl);

	//�����������ͷ�ǰ�洴���Ķ��ڴ�,����������ǰ�߳�
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
	StringCchPrintf(szTitle, _countof(szTitle), _T("[%s]���̶�(%d)"), m_processName, lParam);
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
			//���� Enter ���� Esc ���رյ�ǰ����
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

