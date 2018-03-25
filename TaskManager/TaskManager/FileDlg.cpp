// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "FileDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>


// CFileDlg dialog

IMPLEMENT_DYNAMIC(CFileDlg, CBaseDialog)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CFileDlg::IDD, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE, m_mfcShellTreeCtrl);
	DDX_Control(pDX, IDC_FULLPATH_EDIT, m_editFullPath);
	DDX_Control(pDX, IDC_LIST1, m_listCtrlFiles);
}


BEGIN_MESSAGE_MAP(CFileDlg, CBaseDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE, &CFileDlg::OnTvnSelchangedMfcshelltree)
END_MESSAGE_MAP()


// CFileDlg message handlers

//初始化控件
void CFileDlg::InitControl()
{
	CRect rect;

	m_listCtrlFiles.SetExtendedStyle(m_listCtrlFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlFiles.GetClientRect(&rect);
	m_listCtrlFiles.AddColumns(5,
		_T("文件名"), rect.Width() / 5,
		_T("类型"), rect.Width() / 5,
		_T("大小"), rect.Width() / 5,
		_T("修改时间"), rect.Width() / 5,
		_T("创建时间"), rect.Width() / 5);

	//m_mfcShellTreeCtrl.Expand(m_mfcShellTreeCtrl.GetRootItem(), TVE_EXPAND);
}

void CFileDlg::RefreshSelf()
{
}


BOOL CFileDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//处理 Tree 控件的选中事件
void CFileDlg::OnTvnSelchangedMfcshelltree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString strFullPath;
	//获取当前选中的项
	HTREEITEM hTreeItem = m_mfcShellTreeCtrl.GetSelectedItem();
	//获取当前路径
	m_mfcShellTreeCtrl.GetItemPath(strFullPath);
	//展开当前目录
	m_mfcShellTreeCtrl.Expand(hTreeItem, TVE_EXPAND);
	m_editFullPath.SetWindowText(strFullPath);
	TraverseDirectory(strFullPath);
}

//遍历指定目录(不递归遍历)
void CFileDlg::TraverseDirectory(CString strPath)
{
	m_listCtrlFiles.DeleteAllItems();

	if (strPath == _T(""))
		return;

	CFileFind finder;
	BOOL isNotEmpty = finder.FindFile(strPath + _T("\\*"));//总文件夹，开始遍历 

	int i = 0;
	while (isNotEmpty) {
		isNotEmpty = finder.FindNextFile();//查找文件 
		CString strFileName = finder.GetFileName();
		CString strFileType;
		CString strFileSize(_T('\0'), 256);
		CString strLastModifyTime(_T('\0'), 256);
		CString strCreateTime(_T('\0'), 256);
		CTime ctCreateTime;
		CTime ctLastModifyTime;

		if (!finder.IsDots()) {
			//获取创建时间
			finder.GetCreationTime(ctCreateTime);
			//获取修改时间
			finder.GetLastWriteTime(ctLastModifyTime);
			strCreateTime = ctCreateTime.Format(_T("%Y/%m/%d %H:%M"));
			strLastModifyTime = ctLastModifyTime.Format(_T("%Y/%m/%d %H:%M"));

			//判断目录，这里仅简单区分目录和文件
			if (finder.IsDirectory()) {
				strFileType = _T("目录");
			}
			else {
				strFileType = _T("文件");
				ULONGLONG ullBytes = finder.GetLength();
				if (ullBytes < PER_KB_BYTES) {
					strFileSize.Format(_T("%d Bytes"), ullBytes);
				}
				else {
					ULONGLONG ullKBytes = ullBytes / PER_KB_BYTES;
					strFileSize.Format(_T("%lld"), ullKBytes);
					int nLen = strFileSize.GetLength();
					int nIdx = 0;

					if (nLen > 3) {  //长度大于3就要插入逗号分隔符
						int nRet = nLen / 3;
						int nRemainder = nLen % 3;
						int nCommaCnt = 0;  //逗号分隔符数

						if (nRemainder == 0) { //余数为0时，逗号数 = 商-1
							nCommaCnt = nRet - 1;
							nIdx += 3;
						}
						else {                 //余数不为0时，逗号数 = 商
							nCommaCnt = nRet;
							nIdx = nRemainder;
						}

						//插入逗号分隔符
						for (int i = 0; i < nCommaCnt; i++) {
							strFileSize.Insert(nIdx, _T(','));
							nIdx += 4;
						}
					}
					strFileSize.Append(_T(" KB"));
				}

			}

			m_listCtrlFiles.AddItems(i, 5, strFileName, strFileType,
				strFileSize, strLastModifyTime, strCreateTime);
			i++;
		}
	}
}

UINT CFileDlg::PER_GB_BYTES = 1024 * 1024 * 1024;
UINT CFileDlg::PER_MB_BYTES = 1024 * 1024;
UINT CFileDlg::PER_KB_BYTES = 1024;
