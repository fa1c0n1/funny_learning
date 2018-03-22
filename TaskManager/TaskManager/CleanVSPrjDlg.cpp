// CleanVSPrjDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "CleanVSPrjDlg.h"
#include "afxdialogex.h"


// CCleanVSPrjDlg dialog

IMPLEMENT_DYNAMIC(CCleanVSPrjDlg, CDialogEx)

CCleanVSPrjDlg::CCleanVSPrjDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCleanVSPrjDlg::IDD, pParent)
	, m_strToDelFiles(_T(""))
	, m_strDelFiles(_T(""))
{

}

CCleanVSPrjDlg::~CCleanVSPrjDlg()
{
}

void CCleanVSPrjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILEPATH_LIST, m_listCtrlFilePath);
	DDX_Text(pDX, IDC_TODEL_FILE_EDIT, m_strToDelFiles);
	DDX_Text(pDX, IDC_DEL_FILE_EDIT2, m_strDelFiles);
	DDX_Control(pDX, IDC_TODEL_FILE_EDIT, m_editToDelFile);
	DDX_Control(pDX, IDC_DEL_FILE_EDIT2, m_editDelFile);
}

BEGIN_MESSAGE_MAP(CCleanVSPrjDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GET_INFO_BUTTON, &CCleanVSPrjDlg::OnBnClickedGetInfoButton)
	ON_BN_CLICKED(IDC_START_DEL_BUTTON, &CCleanVSPrjDlg::OnBnClickedStartDelButton)
	ON_WM_DROPFILES()
	ON_NOTIFY(LVN_KEYDOWN, IDC_FILEPATH_LIST, &CCleanVSPrjDlg::OnLvnKeydownFilepathList)
	ON_BN_CLICKED(IDC_CLEAR_INFO_BUTTON, &CCleanVSPrjDlg::OnBnClickedClearInfoButton)
END_MESSAGE_MAP()


// CCleanVSPrjDlg message handlers


//��ȡҪɾ�����ļ�������ʾ����
void CCleanVSPrjDlg::OnBnClickedGetInfoButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strToDelFiles = _T("");
	for (int i = 0; i < m_listCtrlFilePath.GetItemCount(); i++) {
		CString strFilePath = m_listCtrlFilePath.GetItemText(i, 0);
		ShowDelFilePathList(strFilePath, m_editToDelFile, m_strToDelFiles);
	}
}

//��ʼɾ��������ʾ����
void CCleanVSPrjDlg::OnBnClickedStartDelButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strDelFiles = _T("");
	for (int i = 0; i < m_listCtrlFilePath.GetItemCount(); i++) {
		CString strFilePath = m_listCtrlFilePath.GetItemText(i, 0);
		ShowDelFilePathList(strFilePath, m_editDelFile, m_strDelFiles, true);
	}
}

void CCleanVSPrjDlg::InitControl()
{
	CRect rect;

	m_listCtrlFilePath.GetClientRect(&rect);
	m_listCtrlFilePath.SetExtendedStyle(m_listCtrlFilePath.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_listCtrlFilePath.AddColumns(1, _T("·���б�"), rect.Width() * 2);
}

//���Ʊ�������ʾ(��ɾ��)Ҫɾ�����ļ�
void CCleanVSPrjDlg::ShowDelFilePathList(CString strDir, CEdit &editCtrl, CString &strEditCtrl, bool bDelete)
{
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
						if (bDelete) {
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
						}
						else {
								strEditCtrl += _T("�ҵ�: ");
						}

						strEditCtrl += (strFullPath + _T("\r\n"));
						UpdateData(FALSE);
						editCtrl.LineScroll(editCtrl.GetLineCount());
					}
				}
			}
		}
		else
		{
			//�ݹ�����û��ļ��У��������û��ļ��� 
			if (!(finder.IsDots() || finder.IsHidden() || finder.IsSystem() || finder.IsTemporary() || finder.IsReadOnly()))
				ShowDelFilePathList(strFullPath + _T("/"), editCtrl, strEditCtrl, bDelete);
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
	CDialogEx::OnInitDialog();

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
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		m_listCtrlFilePath.AddItems(0, 1, szFileName);
	}

	CDialogEx::OnDropFiles(hDropInfo);
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



void CCleanVSPrjDlg::OnBnClickedClearInfoButton()
{
	// TODO: Add your control notification handler code here
	m_strToDelFiles = _T("");
	m_strDelFiles = _T("");
	UpdateData(FALSE);
}
