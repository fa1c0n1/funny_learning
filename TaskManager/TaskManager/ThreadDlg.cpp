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
	ON_COMMAND_RANGE(ID_SUBMENU_THREAD_REFRESH, ID_SUBMENU_THREAD_END, &CThreadDlg::OnSubmenuThread)
END_MESSAGE_MAP()


// CThreadDlg message handlers


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();
	ListProcessThread(m_dwOwnerPid);
	UpdateTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//��ʼ���ؼ�
void CThreadDlg::InitControl()
{
	CRect rect;

	m_listCtrlThread.SetExtendedStyle(m_listCtrlThread.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlThread.GetClientRect(&rect);
	m_listCtrlThread.AddColumns(3,
		_T("�߳�ID"), rect.Width() / 3,
		_T("���ȼ�"), rect.Width() / 3,
		_T("״̬"), rect.Width() / 3);
}

//����ָ�����̵��߳�
void CThreadDlg::ListProcessThread(DWORD dwOwnerPid)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	HANDLE hThread;
	THREADENTRY32 te32 = {};
	TCHAR szThreadID[32] = {};
	TCHAR szThreadPri[32] = {};
	CString strThreadState(_T('\0'), 256);

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

			hThread = OpenThread(THREAD_SUSPEND_RESUME, NULL, te32.th32ThreadID);
			if (hThread) {
				//ͨ��SuspendThread�����ķ���ֵ(�������)���ж��̵߳�״̬
				int nSuspendCnt = SuspendThread(hThread);
				if (nSuspendCnt == 0) {
					ResumeThread(hThread);
					strThreadState = _T("����");
				}
				else if (nSuspendCnt > 0) {
					ResumeThread(hThread);
					strThreadState = _T("����");
				}
				else {
					strThreadState = _T("����");
				}
			}
			else {
				strThreadState = _T("����");
			}
			m_listCtrlThread.AddItems(i, 3, szThreadID, szThreadPri, strThreadState);
			i++;
		}
	} while (Thread32Next(hThreadSnap, &te32));
	m_nThreadNum = i;

	CloseHandle(hThreadSnap);
}

//��������
void CThreadDlg::RefreshSelf()
{
	m_listCtrlThread.DeleteAllItems();
	ListProcessThread(m_dwOwnerPid);
}

//���´��ڱ���
void CThreadDlg::UpdateTitle()
{
	TCHAR szTitle[128] = {};
	StringCchPrintf(szTitle, _countof(szTitle), _T("[%s]�����߳�(%d)"), m_processName, m_nThreadNum);
	SetWindowText(szTitle);
}

//�߳��б���Ӧ�Ҽ��������Ӳ˵�
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

	//�����Ӳ˵�
	popSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

//��Ӧ�Ҽ��Ӳ˵��ĸ��ֵ���¼�
void CThreadDlg::OnSubmenuThread(UINT uID)
{
	TCHAR buf[32] = {};
	int nSelRowIdx = m_listCtrlThread.GetSelectionMark();
	CString strTid = m_listCtrlThread.GetItemText(nSelRowIdx, 0);
	DWORD dwTid = _ttol(strTid);
	HANDLE hThread;

	switch (uID)
	{
	case ID_SUBMENU_THREAD_REFRESH:
		RefreshSelf();
		break;
	case ID_SUBMENU_THREAD_SUSPEND: {
		hThread = OpenThread(THREAD_SUSPEND_RESUME, NULL, dwTid);
		if (hThread) {
			if (SuspendThread(hThread) != -1)
				RefreshSelf();
			else
				MessageBox(_T("�����߳�ʧ��"), NULL, MB_OK | MB_ICONERROR);
			CloseHandle(hThread);
		}
		else {
			MessageBox(_T("�����߳�ʧ��"), NULL, MB_OK | MB_ICONERROR);
		}
		break;
	}
	case ID_SUBMENU_THREAD_RESUME: {
		hThread = OpenThread(THREAD_SUSPEND_RESUME, NULL, dwTid);
		if (hThread) {
			if (ResumeThread(hThread) != -1) {
				RefreshSelf();
			}
			else {
				MessageBox(_T("�ָ��߳�ʧ��"), NULL, MB_OK | MB_ICONERROR);
			}
			CloseHandle(hThread);
		}
		else {
			MessageBox(_T("�ָ��߳�ʧ��"), NULL, MB_OK | MB_ICONERROR);
		}
		break;
	}
	case ID_SUBMENU_THREAD_END: {
		CString strBuf(_T('\0', 512));
		CString strTid = m_listCtrlThread.GetItemText(nSelRowIdx, 0);
		strBuf = _T("ȷ��Ҫ����ID��Ϊ '");
		strBuf += strTid;
		strBuf += _T("' ���߳���? ");

		if (MessageBox(strBuf, _T("ѯ��"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
			return;

		hThread = OpenThread(THREAD_TERMINATE, NULL, dwTid);
		if (hThread) {
			if (TerminateThread(hThread, 0)) {
				m_nThreadNum--;
				UpdateTitle();
				RefreshSelf();
			}
			else
				MessageBox(_T("�����߳�ʧ��"), NULL, MB_OK | MB_ICONERROR);
			CloseHandle(hThread);
		}
		else {
			MessageBox(_T("�����߳�ʧ��"), NULL, MB_OK | MB_ICONERROR);
		}
		break;
	}
	default:
		break;
	}
}

