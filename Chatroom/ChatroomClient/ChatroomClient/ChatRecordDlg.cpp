// ChatRecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatroomClient.h"
#include "ChatRecordDlg.h"
#include "afxdialogex.h"
#include "ChatMainDlg.h"


// CChatRecordDlg dialog

IMPLEMENT_DYNAMIC(CChatRecordDlg, CDialogEx)

CChatRecordDlg::CChatRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRecordDlg::IDD, pParent)
{

}

CChatRecordDlg::~CChatRecordDlg()
{
}

void CChatRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATRECORD_LIST, m_listCtrlRecord);
}


BEGIN_MESSAGE_MAP(CChatRecordDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChatRecordDlg message handlers


BOOL CChatRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_listCtrlRecord.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rect;
	m_listCtrlRecord.GetClientRect(&rect);
	m_listCtrlRecord.InsertColumn(0, _T("from"), 0, rect.Width() / 5);
	m_listCtrlRecord.InsertColumn(1, _T("to"), 0, rect.Width() / 5);
	m_listCtrlRecord.InsertColumn(2, _T("content"), 0, rect.Width() / 5 * 3);
	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CChatRecordDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}

void CChatRecordDlg::UpdateList()
{
	//删除之前的聊天记录
	m_listCtrlRecord.DeleteAllItems();
	CChatMainDlg *pParent = (CChatMainDlg*)GetParent();
	auto &vt = pParent->m_pClientSocket->m_vecMsgRecord;
	DWORD dwCount = vt.size();
	CString strTmp;

	for (DWORD i = 0; i < dwCount; i++) {
		//聊天发起方
		strTmp = vt[i].szFrom;
		m_listCtrlRecord.InsertItem(i, strTmp.GetBuffer());
		//聊天接收方
		strTmp = vt[i].szTo;
		m_listCtrlRecord.SetItemText(i, 1, strTmp.GetBuffer());
		//聊天内容
		strTmp = vt[i].szContent;
		m_listCtrlRecord.SetItemText(i, 2, strTmp.GetBuffer());
	}
	
}
