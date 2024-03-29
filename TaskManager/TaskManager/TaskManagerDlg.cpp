
// TaskManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "TaskManagerDlg.h"
#include "afxdialogex.h"
#include "ProcessDlg.h"
#include "WindowDlg.h"
#include "FileDlg.h"
#include "CleanVSPrjDlg.h"
#include "Comm.h"
#include <strsafe.h>
#include <PowrProf.h>

#pragma comment(lib, "PowrProf.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CTaskManagerDlg dialog

CTaskManagerDlg::CTaskManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTaskManagerDlg::~CTaskManagerDlg()
{
	for (int i = 0; i < _countof(m_childTab); i++) {
		delete m_childTab[i];
		m_childTab[i] = nullptr;
	}
}

void CTaskManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabAllWnd);
}

BEGIN_MESSAGE_MAP(CTaskManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTaskManagerDlg::OnTcnSelchangeTab1)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_UPDATE_CPUUSAGE, &CTaskManagerDlg::OnUserUpdateCpuUsage)
	ON_MESSAGE(WM_USER_UPDATE_MEMUSAGE, &CTaskManagerDlg::OnUserUpdateMemUsage)
	ON_COMMAND_RANGE(ID_SUBMENU_LOCKSCREEN, ID_SUBMENU_SHUTDOWN, &CTaskManagerDlg::OnSubmenuManage)
END_MESSAGE_MAP()

// CTaskManagerDlg message handlers

BOOL CTaskManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here	
	//创建子线程去获取CPU、内存的使用率，并通过while循环，每隔一秒就刷新底部状态栏
	CreateThread(NULL, 0, UsageProc, (LPVOID)this, 0, NULL);
	InitControl();
	SetHotKey();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTaskManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTaskManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTaskManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTaskManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect clientRect;
	GetClientRect(&clientRect);

	clientRect.top += 10;
	clientRect.bottom -= 10;
	clientRect.left += 10;
	clientRect.right -= 10;

	if (m_tabAllWnd);
		//m_tabAllWnd.MoveWindow(&clientRect);
}

//初始化控件
void CTaskManagerDlg::InitControl()
{
	m_menuMain.LoadMenu(IDR_MENU2);
	this->SetMenu(&m_menuMain);
	m_menuMain.Detach();

	m_tabAllWnd.InsertItem(0, _T("进程"));
	m_tabAllWnd.InsertItem(1, _T("窗口"));
	m_tabAllWnd.InsertItem(2, _T("文件"));
	m_tabAllWnd.InsertItem(3, _T("清理VS工程"));

	m_childTab[0] = new CProcessDlg;
	m_childTab[1] = new CWindowDlg;
	m_childTab[2] = new CFileDlg;
	m_childTab[3] = new CCleanVSPrjDlg;
	m_childTab[0]->Create(IDD_PROCESS_DIALOG, &m_tabAllWnd);
	m_childTab[1]->Create(IDD_WINDOW_DIALOG, &m_tabAllWnd);
	m_childTab[2]->Create(IDD_FILE_DIALOG, &m_tabAllWnd);
	m_childTab[3]->Create(IDD_CLEANVS_DIALOG, &m_tabAllWnd);

	CRect tabRect;
	m_tabAllWnd.GetClientRect(&tabRect);
	tabRect.DeflateRect(1, 21, -1, -1);

	m_childTab[0]->MoveWindow(&tabRect);
	m_childTab[0]->ShowWindow(SW_SHOW);

	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)); //设置状态栏数目

	CRect rect;
	GetClientRect(&rect);
	//设置各栏长度
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_CPU_USAGE, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_MEM_USAGE, SBPS_NORMAL, 150);
	//绘制状态栏
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

//处理Tab控件标签的点击切换事件
void CTaskManagerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSel = m_tabAllWnd.GetCurSel();

	for (int i = 0; i < _countof(m_childTab); i++) {
		m_childTab[i]->ShowWindow(SW_HIDE);
	}

	CRect rect = {};
	m_tabAllWnd.GetClientRect(&rect);
	rect.DeflateRect(1, 21, -1, -1);
	m_childTab[nSel]->MoveWindow(&rect);
	m_childTab[nSel]->ShowWindow(SW_SHOW);
	m_childTab[nSel]->RefreshSelf();
}

UINT BASED_CODE CTaskManagerDlg::indicators[] =
{
	ID_INDICATOR_CPU_USAGE,
	ID_INDICATOR_MEM_USAGE
};

double CTaskManagerDlg::FILETIME2Double(const _FILETIME &fileTime)
{
	return double(fileTime.dwHighDateTime * 4.294967296e9)
		+ double(fileTime.dwLowDateTime);
}

//获取CPU的使用率
int CTaskManagerDlg::GetCpuUsage()
{
	_FILETIME idleTime, kernelTime, userTime;   //空闲时间，内核时间，用户时间
	
	//获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	//等待 1000 毫秒, 使用内核对象等待会更精确
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);

	//获取新时间
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	//将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);

	//计算出使用率
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / 
		(dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime) * 100.0);
}

//获取物理内存使用率
DWORD CTaskManagerDlg::GetMemoryUsage()
{
	MEMORYSTATUS memStatus;
	::GlobalMemoryStatus(&memStatus);
	return memStatus.dwMemoryLoad;
}

//子线程处理函数
DWORD WINAPI CTaskManagerDlg::UsageProc(LPVOID lpParam)
{
	CTaskManagerDlg *pCurDlg = (CTaskManagerDlg *)lpParam;
	int nCpuUsage = 0;
	DWORD dwMemLoad = 0;
	while (true) {
		nCpuUsage = GetCpuUsage();
		::PostMessage(pCurDlg->m_hWnd, WM_USER_UPDATE_CPUUSAGE, NULL, (LPARAM)nCpuUsage);
		Sleep(100);
		dwMemLoad = GetMemoryUsage();
		//投递数据到消息队列，让主线程更新底部状态栏
		::PostMessage(pCurDlg->m_hWnd, WM_USER_UPDATE_MEMUSAGE, NULL, (LPARAM)dwMemLoad);
	}

	return 0;
}

//更新底部状态栏中的CPU使用率
afx_msg LRESULT CTaskManagerDlg::OnUserUpdateCpuUsage(WPARAM wParam, LPARAM lParam)
{
	TCHAR szCpuUsage[32] = {};
	StringCchPrintf(szCpuUsage, _countof(szCpuUsage), _T("CPU Usage: %d%%"), lParam);
	m_wndStatusBar.SetPaneText(0, szCpuUsage);
	return 0;
}

//更新底部状态栏中的内存使用率
afx_msg LRESULT CTaskManagerDlg::OnUserUpdateMemUsage(WPARAM wParam, LPARAM lParam)
{
	TCHAR szMemUsage[32] = {};
	StringCchPrintf(szMemUsage, _countof(szMemUsage), _T("Physical Memory: %d%%"), lParam);
	m_wndStatusBar.SetPaneText(1, szMemUsage);
	return 0;
}

//提升权限
void CTaskManagerDlg::PrivilegeEscalation()
{
	// TODO: Add your command handler code here
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();  //获取该进程的伪句柄
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = {};
	LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//调用函数提升权限
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
}

//处理"控制"子菜单的事件
void CTaskManagerDlg::OnSubmenuManage(UINT uID)
{
	int nRet = 0;
	PrivilegeEscalation();
	switch (uID)
	{
	case ID_SUBMENU_LOCKSCREEN: //锁屏
		LockWorkStation();
		break;
	case ID_SUBMENU_LOGOFF:     //注销
		nRet = MessageBox(_T("确定要注销吗?"), _T("询问"), MB_OKCANCEL | MB_ICONQUESTION);
		if (nRet == IDOK)
			ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
		break;
	case ID_SUBMENU_HIBERNATE:   //休眠
		nRet = MessageBox(_T("确定要休眠吗?"), _T("询问"), MB_OKCANCEL | MB_ICONQUESTION);
		if (nRet == IDOK)
			SetSuspendState(TRUE, FALSE, FALSE);
		break;
	case ID_SUBMENU_SLEEP:       //睡眠
		nRet = MessageBox(_T("确定要睡眠吗?"), _T("询问"), MB_OKCANCEL | MB_ICONQUESTION);
		if (nRet == IDOK)
			SetSuspendState(FALSE, FALSE, FALSE);
		break;
	case ID_SUBMENU_RESTART:      //重启
		nRet = MessageBox(_T("确定要重启电脑吗?"), _T("询问"), MB_OKCANCEL | MB_ICONQUESTION);
		if (nRet == IDOK)
			ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
		break;
	case ID_SUBMENU_SHUTDOWN:     //关机
		nRet = MessageBox(_T("确定要关机吗?"), _T("询问"), MB_OKCANCEL | MB_ICONQUESTION);
		if (nRet == IDOK)
			ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
		break;
	default:
		break;
	}
}

//设置菜单快捷键和全局热键
void CTaskManagerDlg::SetHotKey()
{
	//设置当前窗口的"控制"菜单的子菜单快捷键
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MENU2));

	//设置全局的隐藏/显示的热键
	//最后一个参数，字母的话一定要大写，可参见帮助文档，字母键就是对应的大写字母的ASCII码
	::RegisterHotKey(m_hWnd, 0x1412, MOD_CONTROL | MOD_ALT | MOD_SHIFT, 'H'); 
}

//响应前面设置的全局热键
BOOL CTaskManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//处理"控制"子菜单的快捷键
	if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		return TRUE;

	//处理全局热键: Ctrl+Alt+Shift+H  --> 隐藏/显示窗口
	if (pMsg->message == WM_HOTKEY && pMsg->wParam == 0x1412) {
		if (IsWindowVisible())
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOW);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
