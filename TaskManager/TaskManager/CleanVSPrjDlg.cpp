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


//获取要删除的文件，并显示出来
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

//开始删除，并显示出来
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

	m_listCtrlFilePath.AddColumns(1, _T("路径列表"), rect.Width() * 2);
}

//递推遍历并显示(并删除)要删除的文件
void CCleanVSPrjDlg::ShowDelFilePathList(CString strDir, CEdit &editCtrl, CString &strEditCtrl, bool bDelete)
{
	CFileFind finder;
	BOOL isNotEmpty = finder.FindFile(strDir + _T("*.*"));//总文件夹，开始遍历 
	while (isNotEmpty)
	{
		isNotEmpty = finder.FindNextFile();//查找文件 
		CString strFullPath = finder.GetFilePath();//获取文件的路径，可能是文件夹，可能是文件 
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
								strEditCtrl += _T("删除成功: ");
							}
							catch (CFileException* pEx)
							{
								strEditCtrl += _T("无法删除: ");
								pEx->Delete();
							}
						}
						else {
								strEditCtrl += _T("找到: ");
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
			//递归遍历用户文件夹，跳过非用户文件夹 
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

//实现文件拖拽添加到List控件
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

//按Delete键，将列表框选中的子项删除
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
