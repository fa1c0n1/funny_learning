// CleanVSPrjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "CleanVSPrjDlg.h"
#include "afxdialogex.h"


// CCleanVSPrjDlg dialog

IMPLEMENT_DYNAMIC(CCleanVSPrjDlg, CBaseDialog)

CCleanVSPrjDlg::CCleanVSPrjDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CCleanVSPrjDlg::IDD, pParent)
	, m_strToDelFiles(_T(""))
	, m_strDelFiles(_T(""))
{

}

CCleanVSPrjDlg::~CCleanVSPrjDlg()
{
}

void CCleanVSPrjDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILEPATH_LIST, m_listCtrlFilePath);
	DDX_Text(pDX, IDC_TODEL_FILE_EDIT, m_strToDelFiles);
	DDX_Text(pDX, IDC_DEL_FILE_EDIT2, m_strDelFiles);
	DDX_Control(pDX, IDC_TODEL_FILE_EDIT, m_editToDelFile);
	DDX_Control(pDX, IDC_DEL_FILE_EDIT2, m_editDelFile);
	DDX_Control(pDX, IDC_GET_INFO_BUTTON, m_btnGetInfo);
	DDX_Control(pDX, IDC_START_DEL_BUTTON, m_btnStartDel);
}

BEGIN_MESSAGE_MAP(CCleanVSPrjDlg, CBaseDialog)
	ON_BN_CLICKED(IDC_GET_INFO_BUTTON, &CCleanVSPrjDlg::OnBnClickedGetInfoButton)
	ON_BN_CLICKED(IDC_START_DEL_BUTTON, &CCleanVSPrjDlg::OnBnClickedStartDelButton)
	ON_WM_DROPFILES()
	ON_NOTIFY(LVN_KEYDOWN, IDC_FILEPATH_LIST, &CCleanVSPrjDlg::OnLvnKeydownFilepathList)
	ON_MESSAGE(WM_GETINFO_FINISH, &CCleanVSPrjDlg::OnGetinfoFinish)
	ON_MESSAGE(WM_DELFILE_FINISH, &CCleanVSPrjDlg::OnDelfileFinish)
END_MESSAGE_MAP()


// CCleanVSPrjDlg message handlers



void CCleanVSPrjDlg::InitControl()
{
	CRect rect;

	m_listCtrlFilePath.GetClientRect(&rect);
	m_listCtrlFilePath.SetExtendedStyle(m_listCtrlFilePath.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listCtrlFilePath.AddColumns(1, _T("·���б�"), rect.Width() * 2);
	m_btnStartDel.EnableWindow(FALSE);
}

//���Ʊ�������ʾ(��ɾ��)Ҫɾ�����ļ�
void CCleanVSPrjDlg::ShowDelFilePathList(CString strDir, HWND hWnd, CEdit &editCtrl, 
	CString &strEditCtrl, vector<CString> &vtToDelFiles, bool bDelete)
{
	if (bDelete) { //Ҫɾ��
		for (CString strFullPath : vtToDelFiles) {
			try
			{
				CFile::Remove(strFullPath);
				strEditCtrl += _T("ɾ���ɹ�: ");
			}
			catch (CFileException* pEx)
			{
				strEditCtrl += _T("�޷�ɾ��: ");
				pEx->Delete();
			}

			strEditCtrl += (strFullPath + _T("\r\n"));
			Sleep(10);
			editCtrl.SetWindowText(strEditCtrl);
			editCtrl.LineScroll(editCtrl.GetLineCount());
		}
	}
	else { //����ȡ��Ϣ
		CFileFind finder;
		BOOL isNotEmpty = finder.FindFile(strDir + _T("*.*"));//���ļ��У���ʼ���� 
		while (isNotEmpty)
		{
			isNotEmpty = finder.FindNextFile();//�����ļ� 
			CString strFullPath = finder.GetFilePath();//��ȡ�ļ���·�����������ļ��У��������ļ� 
			CString strFileName = finder.GetFileName();

			if (!(finder.IsDirectory())) {
				for (int j = 0; j < _countof(m_szDelSuffix); j++) {
					int nDotIdx = strFullPath.ReverseFind(_T('.'));
					if (nDotIdx != -1) {
						int nIdx = strFullPath.Find(m_szDelSuffix[j], nDotIdx);
						if (nIdx != -1) {
							vtToDelFiles.push_back(strFullPath);
							strEditCtrl += _T("�ҵ�: ");
							strEditCtrl += (strFullPath + _T("\r\n"));
							Sleep(10);
							editCtrl.SetWindowText(strEditCtrl);
							editCtrl.LineScroll(editCtrl.GetLineCount());
						}
					}
				}
			}
			else
			{
				//�ݹ�����û��ļ��У��������û��ļ��� 
				if (!(finder.IsDots() || finder.IsHidden() || finder.IsSystem() || finder.IsTemporary() || finder.IsReadOnly()))
					ShowDelFilePathList(strFullPath + _T("/"), hWnd, editCtrl, strEditCtrl, vtToDelFiles, bDelete);
			}
		}
	}
}

//.obj\.tlog\.lastbuildstate\.idb\.pdb\.pch\.res\.ilk\.exe\.sdf\.ipch\.log
TCHAR *CCleanVSPrjDlg::m_szDelSuffix[] = {
	_T(".obj"), _T(".tlog"), _T(".lastbuildstate"), _T(".idb"), _T(".pdb"),
	_T(".pch"), _T(".res"), _T(".ilk"), _T(".exe"), _T(".sdf"), _T(".ipch"), _T(".log")
};

BOOL CCleanVSPrjDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//ʵ���ļ���ק��ӵ�List�ؼ�
void CCleanVSPrjDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	int nFileCnt = DragQueryFile(hDropInfo, -1, NULL, 0);

	for (int i = 0; i < nFileCnt; i++) {
		TCHAR szFileName[MAX_PATH] = {};
		LVFINDINFO info = {};
		int nIdx;
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);

		info.flags = LVFI_STRING;
		info.psz = szFileName;

		//��Ҫ�ظ����
		if ((nIdx = m_listCtrlFilePath.FindItem(&info)) != -1)
			continue;

		m_listCtrlFilePath.AddItems(0, 1, szFileName);
	}

	CBaseDialog::OnDropFiles(hDropInfo);
}

//��Delete�������б��ѡ�е�����ɾ��
void CCleanVSPrjDlg::OnLvnKeydownFilepathList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pLVKeyDow->wVKey == VK_DELETE) {
		POSITION pos = m_listCtrlFilePath.GetFirstSelectedItemPosition();

		while (pos) {
			int nIdx = m_listCtrlFilePath.GetNextSelectedItem(pos);
			m_listCtrlFilePath.DeleteItem(nIdx);
			pos = m_listCtrlFilePath.GetFirstSelectedItemPosition();
		}
	}
}

//��ȡҪɾ�����ļ�������ʾ����
void CCleanVSPrjDlg::OnBnClickedGetInfoButton()
{
	// TODO: Add your control notification handler code here
	m_vtToDelFiles.clear();
	m_btnGetInfo.EnableWindow(FALSE);
	m_btnStartDel.EnableWindow(FALSE);
	UpdateData(TRUE);
	m_strToDelFiles = _T("");

	PINFOPARAM pInfoParam = new INFOPARAM {};
	pInfoParam->hWnd = m_hWnd;
	pInfoParam->pListCtrl = &m_listCtrlFilePath;
	pInfoParam->pEdit = &m_editToDelFile;
	pInfoParam->pStrEdit = &m_strToDelFiles;
	pInfoParam->pVtFiles = &m_vtToDelFiles;
	pInfoParam->bDelete = false;
	pInfoParam->uMsgFinish = WM_GETINFO_FINISH;

	HANDLE hThread = CreateThread(NULL, 0, GetInfoThreadProc, (LPVOID)pInfoParam, 0, NULL);
}

//��ʼɾ��������ʾ����
void CCleanVSPrjDlg::OnBnClickedStartDelButton()
{
	m_btnGetInfo.EnableWindow(FALSE);
	m_btnStartDel.EnableWindow(FALSE);
	UpdateData(TRUE);
	m_strDelFiles = _T("");

	PINFOPARAM pInfoParam = new INFOPARAM{};
	pInfoParam->hWnd = m_hWnd;
	pInfoParam->pListCtrl = &m_listCtrlFilePath;
	pInfoParam->pEdit = &m_editDelFile;
	pInfoParam->pStrEdit = &m_strDelFiles;
	pInfoParam->pVtFiles = &m_vtToDelFiles;
	pInfoParam->bDelete = true;
	pInfoParam->uMsgFinish = WM_DELFILE_FINISH;

	HANDLE hThread = CreateThread(NULL, 0, GetInfoThreadProc, (LPVOID)pInfoParam, 0, NULL);
}

DWORD WINAPI CCleanVSPrjDlg::GetInfoThreadProc(LPVOID lpParam)
{
	PINFOPARAM pInfoParam = (PINFOPARAM)lpParam;
	HWND hWnd = pInfoParam->hWnd;
	CMyListCtrl *pListCtrl = pInfoParam->pListCtrl;
	CEdit *pEdit = pInfoParam->pEdit;
	CString *pStrEdit = pInfoParam->pStrEdit;
	vector<CString> *pVtFiles = pInfoParam->pVtFiles;
	UINT uMsgFinish = pInfoParam->uMsgFinish;
	bool bDelete = pInfoParam->bDelete;

	int nCnt = pListCtrl->GetItemCount();
	if (bDelete) {
		nCnt = 1;
	}

	for (int i = 0; i < nCnt; i++) {
		CString strFilePath = pListCtrl->GetItemText(i, 0);
		ShowDelFilePathList(strFilePath, hWnd, *pEdit, *pStrEdit, *pVtFiles, bDelete);
	}
	::PostMessage(hWnd, uMsgFinish, 0, 0);

	if (pInfoParam) {
		delete pInfoParam;
		pInfoParam = nullptr;
	}

	return 0;
}

afx_msg LRESULT CCleanVSPrjDlg::OnGetinfoFinish(WPARAM wParam, LPARAM lParam)
{
	if (m_vtToDelFiles.size() > 0) {
		m_strToDelFiles += _T("��ȡ���.\r\n");
		m_btnStartDel.EnableWindow(TRUE);
	}
	else {
		m_strToDelFiles += _T("û�д�ɾ�����ļ�.\r\n");
		m_btnStartDel.EnableWindow(FALSE);
	}

	m_editToDelFile.SetWindowText(m_strToDelFiles);
	m_editToDelFile.LineScroll(m_editToDelFile.GetLineCount());
	m_btnGetInfo.EnableWindow(TRUE);
	return 0;
}

afx_msg LRESULT CCleanVSPrjDlg::OnDelfileFinish(WPARAM wParam, LPARAM lParam)
{
	m_strDelFiles += _T("\r\nɾ�����.");
	m_editDelFile.SetWindowTextW(m_strDelFiles);
	m_editDelFile.LineScroll(m_editDelFile.GetLineCount());
	m_btnGetInfo.EnableWindow(TRUE);
	m_btnStartDel.EnableWindow(FALSE);
	return 0;
}

void CCleanVSPrjDlg::RefreshSelf()
{
}
