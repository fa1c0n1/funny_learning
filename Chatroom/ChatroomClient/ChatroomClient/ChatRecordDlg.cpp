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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

UINT __cdecl CChatRecordDlg::UpdateListProc(LPVOID pParam)
{
	PRECORDPROCINFO pRecordProcInfo = (PRECORDPROCINFO)pParam;
	vector<CHATMSGRECORD> vtTmp = *pRecordProcInfo->pVtRecord;
	UpdateList(pRecordProcInfo->pChatRecordDlg, vtTmp);

	if (pRecordProcInfo) {
		delete pRecordProcInfo;
		pRecordProcInfo = nullptr;
	}

	AfxEndThread(0);
	return 0;
}

void CChatRecordDlg::UpdateRecord(vector<CHATMSGRECORD> &vtChatRecord)
{
	PRECORDPROCINFO pRecordProcInfo = new RECORDPROCINFO{};
	pRecordProcInfo->pChatRecordDlg = this;
	pRecordProcInfo->pVtRecord = &vtChatRecord;
	//�������߳�ȥ��ʾ�����¼
	AfxBeginThread(UpdateListProc, (LPVOID)pRecordProcInfo);
}

void CChatRecordDlg::UpdateList(CChatRecordDlg *pChatRecordDlg, vector<CHATMSGRECORD> &vtChatRecord)
{
	//ɾ��֮ǰ�������¼
	CListCtrl &listCtrl = pChatRecordDlg->m_listCtrlRecord;
	DWORD dwCount = vtChatRecord.size();
	CString strTmp;

	for (DWORD i = 0; i < dwCount; i++) {
		//���췢��
		strTmp = vtChatRecord[i].szFrom;
		listCtrl.InsertItem(i, strTmp.GetBuffer());
		//������շ�
		strTmp = vtChatRecord[i].szTo;
		listCtrl.SetItemText(i, 1, strTmp.GetBuffer());
		//��������
		strTmp = vtChatRecord[i].szContent;
		listCtrl.SetItemText(i, 2, strTmp.GetBuffer());
	}
}

