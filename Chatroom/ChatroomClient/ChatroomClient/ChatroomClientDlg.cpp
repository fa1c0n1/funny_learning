
// ChatroomClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatroomClient.h"
#include "ChatroomClientDlg.h"
#include "afxdialogex.h"
#include "Comm.h"
#include "ChatMainDlg.h"

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


// CChatroomClientDlg dialog



CChatroomClientDlg::CChatroomClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatroomClientDlg::IDD, pParent)
	, m_strUsername(_T(""))
	, m_strPwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatroomClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_strUsername);
	DDX_Text(pDX, IDC_USERNAME_EDIT2, m_strPwd);
}

BEGIN_MESSAGE_MAP(CChatroomClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOGIN_BUTTON, &CChatroomClientDlg::OnBnClickedLoginButton)
	ON_BN_CLICKED(IDC_REGISTER_BUTTON, &CChatroomClientDlg::OnBnClickedRegisterButton)
	ON_BN_CLICKED(IDC_ANONYMOUS_BUTTON, &CChatroomClientDlg::OnBnClickedAnonymousButton)
END_MESSAGE_MAP()


// CChatroomClientDlg message handlers

BOOL CChatroomClientDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatroomClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatroomClientDlg::OnPaint()
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
HCURSOR CChatroomClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatroomClientDlg::OnBnClickedLoginButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strUsername.IsEmpty() || m_strPwd.IsEmpty()) {
		MessageBox(_T("用户名或密码不能为空!"));
		return;
	}

	if (!m_sClient.ConnectServer(SERVER_IP, SERVER_PORT)) {
		MessageBox(_T("连接服务器失败!"), _T("登录失败"), MB_OK | MB_ICONWARNING);
		return;
	}

	CString strSend = m_strUsername;
	strSend += _T(":") + m_strPwd;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_sClient.Send(LOGIN, str.GetBuffer(), str.GetLength() + 1);
	char *pRet = m_sClient.Recv();
	if (pRet == nullptr) {
		m_sClient.Close();
		return;
	}

	CStringA strShowName = CW2A(m_strUsername.GetBuffer(), CP_THREAD_ACP);
	strcpy_s(m_sClient.m_szName, strShowName.GetBuffer());
	ShowWindow(SW_HIDE);
	CChatMainDlg chatMainDlg(&m_sClient);
	chatMainDlg.m_bLogin = true;
	INT_PTR nRet = chatMainDlg.DoModal();

	if (nRet == 9) { 
		ShowWindow(SW_SHOW);
	}
	else {
		EndDialog(0);
		m_sClient.Close();
	}
}

void CChatroomClientDlg::OnBnClickedRegisterButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strUsername.IsEmpty() || m_strPwd.IsEmpty()) {
		MessageBox(_T("用户名和密码不能为空"), _T("提示"));
		return;
	}

	if (!m_sClient.ConnectServer(SERVER_IP, SERVER_PORT)) {
		MessageBox(_T("连接服务器失败"), _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	CString strSend = m_strUsername;
	strSend += _T(":") + m_strPwd;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_sClient.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1);

	//等待注册结果
	char *pRet = m_sClient.Recv();
	if (pRet == nullptr)
		;
		//MessageBox(_T("注册失败!"));
	else
		MessageBox(_T("注册成功!"));

	m_sClient.Close();
}

//匿名按钮被点击
void CChatroomClientDlg::OnBnClickedAnonymousButton()
{
	// TODO: Add your control notification handler code here
	if (!m_sClient.ConnectServer(SERVER_IP, SERVER_PORT)) {
		MessageBox(_T("连接服务器失败!"), _T("Error"), MB_OK | MB_ICONWARNING);
		return;
	}

	EndDialog(0);
	CChatMainDlg chatMainDlg(&m_sClient);
	chatMainDlg.m_bLogin = false;
	chatMainDlg.DoModal();
	m_sClient.Close();
}


void CChatroomClientDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	OnBnClickedLoginButton();
}
