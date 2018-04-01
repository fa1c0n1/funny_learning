// ChatMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatroomClient.h"
#include "ChatMainDlg.h"
#include "afxdialogex.h"
#include "One2OneDlg.h"
#include "Comm.h"
#include "SearchFriendDlg.h"


// CChatMainDlg dialog

IMPLEMENT_DYNAMIC(CChatMainDlg, CDialogEx)

CChatMainDlg::CChatMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatMainDlg::IDD, pParent)
	, m_strShow(_T(""))
	, m_strSend(_T(""))
{

}

CChatMainDlg::CChatMainDlg(CClientSocket *pClientSocket, CWnd *pParent)
	: CDialogEx(CChatMainDlg::IDD, pParent), m_pClientSocket(pClientSocket)
{
}

CChatMainDlg::~CChatMainDlg()
{
}

void CChatMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ONLINE_LIST, m_listCtrlUser);
	DDX_Text(pDX, IDC_CHAT_HISTORY_EDIT, m_strShow);
	DDX_Text(pDX, IDC_CHAT_MSG_EDIT, m_strSend);
}


BEGIN_MESSAGE_MAP(CChatMainDlg, CDialogEx)
	ON_MESSAGE(WM_SHSOCKET, &CChatMainDlg::OnShsocket)
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CChatMainDlg::OnBnClickedSendButton)
	ON_NOTIFY(NM_DBLCLK, IDC_ONLINE_LIST, &CChatMainDlg::OnNMDblclkOnlineList)
	ON_NOTIFY(NM_RCLICK, IDC_ONLINE_LIST, &CChatMainDlg::OnNMRClickOnlineList)
	ON_COMMAND(ID_SUBMENU_ADDFRIEND, &CChatMainDlg::OnSubmenuAddfriend)
	ON_COMMAND(ID_SUBMENU_SEARCHFRIEND, &CChatMainDlg::OnSubmenuSearchfriend)
	ON_COMMAND(ID_SUBMENU_SEARCHRECORD, &CChatMainDlg::OnSubmenuSearchrecord)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChatMainDlg message handlers


BOOL CChatMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTitle(m_pClientSocket->m_szName);
	SetWindowText(strTitle.GetBuffer());

	ModifyStyleEx(0, WS_EX_APPWINDOW);
	m_listCtrlUser.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listCtrlUser.InsertColumn(0, _T("在线列表"), 0, 100);

	//使用消息选择模型绑定 socket 和 当前对话框
	WSAAsyncSelect(m_pClientSocket->m_sClient, m_hWnd, WM_SHSOCKET, FD_READ | FD_CLOSE);
	m_pClientSocket->Send(ANONYMOUS, m_pClientSocket->m_szName, strlen(m_pClientSocket->m_szName) + 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


afx_msg LRESULT CChatMainDlg::OnShsocket(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	WORD wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrCode = WSAGETSELECTERROR(lParam);

	//先判断是否有网络错误事件发生，有则跳过
	if (wErrCode) {
		exit(0);
		MessageBox(_T("网络错误"));
		CDialogEx::OnClose();
	}

	switch (wEvent)
	{
	case FD_READ:
	{
		char *szRecv = m_pClientSocket->Recv();
		if (szRecv == nullptr) {
			if (m_pClientSocket->m_pObjUpdate) {
				
			}
			else if (m_pClientSocket->m_pObjOne2One) {

			} 

			UpdateData(TRUE);
			m_strShow += szRecv;
			m_strShow += "\r\n";
			UpdateData(FALSE);
		}
	}
	default:
		break;
	}

	return 0;
}

void CChatMainDlg::ChatForOne2One(CHATONE2ONE &objOne2One)
{
	CString strName(objOne2One.szName);
	CString strContent(objOne2One.szContent);
	if (m_map.find(strName) == m_map.end()) {
		COne2OneDlg *pOne2OneDlg = new COne2OneDlg;
		pOne2OneDlg->Create(IDD_ONE2ONE_DIALOG, this);
		pOne2OneDlg->SetWindowTextW(strName.GetBuffer());
		m_map[strName] = pOne2OneDlg;
		pOne2OneDlg->m_strShow += strName + _T(":") + strContent;
		pOne2OneDlg->m_strShow += _T("\r\n");
		pOne2OneDlg->UpdateData(FALSE);
		pOne2OneDlg->ShowWindow(SW_SHOW);
	}
	else {
		COne2OneDlg *pOne2OneDlg = (COne2OneDlg*)m_map[strName];
		pOne2OneDlg->UpdateData(TRUE);
		pOne2OneDlg->m_strShow += strName + _T(":") + strContent;
		pOne2OneDlg->m_strShow += _T("\r\n");
		pOne2OneDlg->UpdateData(FALSE);
		pOne2OneDlg->ShowWindow(SW_SHOW);
	}
}

void CChatMainDlg::OnBnClickedSendButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
		return;

	CStringA str = CW2A(m_strSend.GetBuffer(), CP_THREAD_ACP);
	m_pClientSocket->Send(CHAT, str.GetBuffer(), str.GetLength() + 1);
	m_strShow += _T("你说: ");
	m_strShow += m_strSend;
	m_strShow += _T("\r\n");
	m_strSend.Empty();
	UpdateData(FALSE);
}


void CChatMainDlg::OnNMDblclkOnlineList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//双击空白区域
	if (pNMItemActivate->iItem == -1)
		return;


}


void CChatMainDlg::OnNMRClickOnlineList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//如果是匿名聊天，则右键失效
	if (!m_bLogin)
		return;

	m_dwNameIndex = pNMItemActivate->iItem;
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	CMenu *pSubMenu = menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
}


void CChatMainDlg::OnSubmenuAddfriend()
{
	// TODO: Add your command handler code here
	CString strFriend = m_listCtrlUser.GetItemText(m_dwNameIndex, 0);
	CString strUser;
	GetWindowText(strUser);
	strUser += _T(":");
	strUser += strFriend;
	CStringA strSend = CW2A(strUser.GetBuffer(), CP_THREAD_ACP);
	m_pClientSocket->Send(ADDFRIEND, strSend.GetBuffer(), strSend.GetLength() + 1);
}


void CChatMainDlg::OnSubmenuSearchfriend()
{
	// TODO: Add your command handler code here
	CSearchFriendDlg searchFriendDlg;
	searchFriendDlg.DoModal();

	if (searchFriendDlg.m_strFriendName.IsEmpty())
		return;

	CStringA strSearch = CW2A(searchFriendDlg.m_strFriendName.GetBuffer(), CP_THREAD_ACP);
	m_pClientSocket->Send(SEARCHUSER, strSearch.GetBuffer(), strSearch.GetLength() + 1);

}


void CChatMainDlg::OnSubmenuSearchrecord()
{
	// TODO: Add your command handler code here
	
	//防止上次获取的消息记录还未返回就重新申请，设置事件对象同步
	DWORD dwRet = WaitForSingleObject(m_pClientSocket->m_hEvent, 100);
	if (dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT)
		return;

	m_pClientSocket->Send(MSGRECORD, NULL, NULL);
	m_pClientSocket->m_vecMsgRecord.clear();
	//设置定时器等待消息返回
	SetTimer(0x1001, 1000, NULL);
}


void CChatMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0x1001) {
		DWORD dwRet = WaitForSingleObject(m_pClientSocket->m_hEvent, 100);
		if (dwRet == WAIT_OBJECT_0) {
			KillTimer(nIDEvent);
			SetEvent(m_pClientSocket->m_hEvent);
			if (m_pChatRecordDlg) {
				//更新数据
				m_pChatRecordDlg->UpdateList();
				//显示窗口
				m_pChatRecordDlg->ShowWindow(SW_SHOW);
			}
			else {
				m_pChatRecordDlg = new CChatRecordDlg;
				m_pChatRecordDlg->Create(IDD_CHAT_RECORD_DIALOG, this);
				m_pChatRecordDlg->ShowWindow(SW_SHOW);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
