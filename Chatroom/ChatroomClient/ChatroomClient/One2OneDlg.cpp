// One2OneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatroomClient.h"
#include "One2OneDlg.h"
#include "afxdialogex.h"
#include "ChatMainDlg.h"


// COne2OneDlg dialog

IMPLEMENT_DYNAMIC(COne2OneDlg, CDialogEx)

COne2OneDlg::COne2OneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COne2OneDlg::IDD, pParent)
	, m_strShow(_T(""))
	, m_strSend(_T(""))
	, m_strFromName(_T(""))
	, m_strToName(_T(""))
{

}

COne2OneDlg::~COne2OneDlg()
{
}

void COne2OneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHAT_HISTORY_EDIT, m_strShow);
	DDX_Text(pDX, IDC_CHAT_MSG_EDIT, m_strSend);
	DDX_Control(pDX, IDC_CHAT_HISTORY_EDIT, m_editShow);
}


BEGIN_MESSAGE_MAP(COne2OneDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SEND_BUTTON, &COne2OneDlg::OnBnClickedSendButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// COne2OneDlg message handlers


void COne2OneDlg::OnBnClickedSendButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
		return;

	CString strOne2One(m_strFromName);
	strOne2One += _T(":") + m_strToName;
	strOne2One += _T(":") + m_strSend;
	CStringA str = CW2A(strOne2One.GetBuffer(), CP_THREAD_ACP);
	CChatMainDlg *pParent = (CChatMainDlg*)GetParent();
	pParent->m_pClientSocket->Send(ONE2ONE, str.GetBuffer(), str.GetLength() + 1);
	m_strShow += _T("你说: ");
	m_strShow += m_strSend;
	m_strShow += _T("\r\n");
	m_strSend.Empty();
	UpdateData(FALSE);
	m_editShow.LineScroll(m_editShow.GetLineCount());
}


void COne2OneDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


BOOL COne2OneDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//捕获 Alt+Enter 组合键事件，改为发送聊天内容
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_RETURN) {
		OnBnClickedSendButton();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void COne2OneDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//CDialogEx::OnOK();
}
