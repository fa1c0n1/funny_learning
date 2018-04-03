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
	DDX_Control(pDX, IDC_FRIEND_LIST, m_listCtrlFriend);
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
	ON_NOTIFY(NM_DBLCLK, IDC_FRIEND_LIST, &CChatMainDlg::OnNMDblclkFriendList)
END_MESSAGE_MAP()


// CChatMainDlg message handlers


BOOL CChatMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString strTitle(m_pClientSocket->m_szName);
	strTitle.Insert(0, _T("当前用户: "));
	SetWindowText(strTitle.GetBuffer());

	ModifyStyleEx(0, WS_EX_APPWINDOW);
	m_listCtrlUser.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	CRect rect;
	m_listCtrlUser.GetClientRect(&rect);
	m_listCtrlUser.InsertColumn(0, _T("聊天室在线列表"), 0, rect.Width());

	m_listCtrlFriend.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listCtrlFriend.InsertColumn(0, _T("好友列表"), 0, rect.Width());

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
		MessageBox(_T("网络错误"));
		EndDialog(0);
	}

	switch (wEvent)
	{
	case FD_READ:
	{
		char *szRecv = m_pClientSocket->Recv();
		if (szRecv == nullptr) {
			if (m_pClientSocket->m_pObjUpdate) {
				InsertOrDeleteUser(*m_pClientSocket->m_pObjUpdate);
				delete m_pClientSocket->m_pObjUpdate;
				m_pClientSocket->m_pObjUpdate = nullptr;
			}
			else if (m_pClientSocket->m_pObjUpdateFriend) {
				InsertFriend(*m_pClientSocket->m_pObjUpdateFriend);
				delete m_pClientSocket->m_pObjUpdateFriend;
				m_pClientSocket->m_pObjUpdateFriend = nullptr;
			}
			else if (m_pClientSocket->m_pObjOne2One) {
				ChatForOne2One(*m_pClientSocket->m_pObjOne2One);
				delete m_pClientSocket->m_pObjOne2One;
				m_pClientSocket->m_pObjOne2One = nullptr;
			}
			else if (m_pClientSocket->m_pObjAddFriend) {
				InsertFriend(*m_pClientSocket->m_pObjAddFriend);
				delete m_pClientSocket->m_pObjAddFriend;
				m_pClientSocket->m_pObjAddFriend = nullptr;
			}

			return 0;
		}

		UpdateData(TRUE);
		m_strShow += szRecv;
		m_strShow += "\r\n";
		UpdateData(FALSE);
	}
	case FD_CLOSE:
	{
		/*MessageBox(_T("与服务器间的连接已断开"), _T("提示"));
		m_pClientSocket->Close();*/
		//EndDialog(0);
	}
	default:
		break;
	}

	return 0;
}

void CChatMainDlg::ChatForOne2One(CHATONE2ONE &objOne2One)
{
	CString strFromName(objOne2One.szNameTo);
	CString strToName(objOne2One.szNameFrom);
	CString strContent(objOne2One.szContent);
	CString strTitle = _T("私聊-") + strToName;
	COne2OneDlg *pOne2OneDlg = nullptr;

	if (m_map.find(strToName) == m_map.end()) {
		pOne2OneDlg = new COne2OneDlg;
		pOne2OneDlg->Create(IDD_ONE2ONE_DIALOG, this);
		pOne2OneDlg->m_strFromName = strFromName;
		pOne2OneDlg->m_strToName = strToName;
		m_map[strToName] = pOne2OneDlg;
		pOne2OneDlg->m_strShow += strToName + _T(":") + strContent;
		pOne2OneDlg->m_strShow += _T("\r\n");
		pOne2OneDlg->UpdateData(FALSE);
		pOne2OneDlg->ShowWindow(SW_SHOW);
	}
	else {
		pOne2OneDlg = (COne2OneDlg*)m_map[strToName];
		pOne2OneDlg->UpdateData(TRUE);
		pOne2OneDlg->m_strShow += strToName + _T(":") + strContent;
		pOne2OneDlg->m_strShow += _T("\r\n");
		pOne2OneDlg->UpdateData(FALSE);
		pOne2OneDlg->ShowWindow(SW_SHOW);
	}
	pOne2OneDlg->SetWindowTextW(strTitle.GetBuffer());
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

	int nSelIdx = pNMItemActivate->iItem;
	//双击空白区域
	if (nSelIdx == -1)
		return;

	OpenOne2OneDialog(m_listCtrlUser, nSelIdx);
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
	CString strUser(m_pClientSocket->m_szName);
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

	if (searchFriendDlg.m_strSearchName.IsEmpty())
		return;

	CString strSelfName(m_pClientSocket->m_szName);
	if (searchFriendDlg.m_strSearchName == strSelfName) {
		MessageBox(_T("不能搜索本人"), _T("提示"));
		return;
	}

	CStringA strSearchA = CW2A(searchFriendDlg.m_strSearchName.GetBuffer(), CP_THREAD_ACP);
	m_pClientSocket->Send(SEARCHUSER, strSearchA.GetBuffer(), strSearchA.GetLength() + 1);
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

void CChatMainDlg::InsertOrDeleteUser(CHATUPDATEUSER &objChatUpdateUser)
{
	CString strTmp = CA2W(objChatUpdateUser.buf, CP_THREAD_ACP);
	if (objChatUpdateUser.bAdd) {
		m_listCtrlUser.InsertItem(0, strTmp);
	}
	else {
		LVFINDINFO info = {};
		info.flags = LVFI_STRING;
		info.psz = strTmp;
		int nItemIdx = m_listCtrlUser.FindItem(&info);
		m_listCtrlUser.DeleteItem(nItemIdx);
	}
}

void CChatMainDlg::InsertFriend(CHATADDFRIEND &objChatAddFriend)
{
	CString strTmp = CA2W(objChatAddFriend.szFriendName, CP_THREAD_ACP);
	m_listCtrlFriend.InsertItem(0, strTmp);
}

void CChatMainDlg::InsertFriend(CHATUPDATEFRIEND &objChatUpdateFriend)
{
	CString strTmp = CA2W(objChatUpdateFriend.szFriendName, CP_THREAD_ACP);
	m_listCtrlFriend.InsertItem(0, strTmp);
}

void CChatMainDlg::OnNMDblclkFriendList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelIdx = pNMItemActivate->iItem;
	//双击空白区域
	if (nSelIdx == -1)
		return;

	OpenOne2OneDialog(m_listCtrlFriend, nSelIdx);
}

void CChatMainDlg::OpenOne2OneDialog(CListCtrl &listCtrl, int nSelIndex)
{
	COne2OneDlg *pOne2OneDlg = nullptr;
	CString strSel = listCtrl.GetItemText(nSelIndex, 0);

	if (m_map.find(strSel) == m_map.end()) {
		pOne2OneDlg = new COne2OneDlg;
		pOne2OneDlg->Create(IDD_ONE2ONE_DIALOG, this);
		CString strTitle(_T("私聊-"));
		strTitle += strSel;
		pOne2OneDlg->SetWindowText(strTitle.GetBuffer());
		pOne2OneDlg->m_strFromName = m_pClientSocket->m_szName;
		pOne2OneDlg->m_strToName = strSel;
		m_map[strSel] = pOne2OneDlg;
	}
	else {
		pOne2OneDlg = (COne2OneDlg *)m_map[strSel];
	}

	pOne2OneDlg->ShowWindow(SW_SHOW);
}
