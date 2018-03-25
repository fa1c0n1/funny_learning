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

//��ʼ���ؼ�
void CFileDlg::InitControl()
{
	CRect rect;

	m_listCtrlFiles.SetExtendedStyle(m_listCtrlFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES);
	m_listCtrlFiles.GetClientRect(&rect);
	m_listCtrlFiles.AddColumns(5,
		_T("�ļ���"), rect.Width() / 5,
		_T("����"), rect.Width() / 5,
		_T("��С"), rect.Width() / 5,
		_T("�޸�ʱ��"), rect.Width() / 5,
		_T("����ʱ��"), rect.Width() / 5);

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

//���� Tree �ؼ���ѡ���¼�
void CFileDlg::OnTvnSelchangedMfcshelltree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString strFullPath;
	//��ȡ��ǰѡ�е���
	HTREEITEM hTreeItem = m_mfcShellTreeCtrl.GetSelectedItem();
	//��ȡ��ǰ·��
	m_mfcShellTreeCtrl.GetItemPath(strFullPath);
	//չ����ǰĿ¼
	m_mfcShellTreeCtrl.Expand(hTreeItem, TVE_EXPAND);
	m_editFullPath.SetWindowText(strFullPath);
	TraverseDirectory(strFullPath);
}

//����ָ��Ŀ¼(���ݹ����)
void CFileDlg::TraverseDirectory(CString strPath)
{
	m_listCtrlFiles.DeleteAllItems();

	if (strPath == _T(""))
		return;

	CFileFind finder;
	BOOL isNotEmpty = finder.FindFile(strPath + _T("\\*"));//���ļ��У���ʼ���� 

	int i = 0;
	while (isNotEmpty) {
		isNotEmpty = finder.FindNextFile();//�����ļ� 
		CString strFileName = finder.GetFileName();
		CString strFileType;
		CString strFileSize(_T('\0'), 256);
		CString strLastModifyTime(_T('\0'), 256);
		CString strCreateTime(_T('\0'), 256);
		CTime ctCreateTime;
		CTime ctLastModifyTime;

		if (!finder.IsDots()) {
			//��ȡ����ʱ��
			finder.GetCreationTime(ctCreateTime);
			//��ȡ�޸�ʱ��
			finder.GetLastWriteTime(ctLastModifyTime);
			strCreateTime = ctCreateTime.Format(_T("%Y/%m/%d %H:%M"));
			strLastModifyTime = ctLastModifyTime.Format(_T("%Y/%m/%d %H:%M"));

			//�ж�Ŀ¼�������������Ŀ¼���ļ�
			if (finder.IsDirectory()) {
				strFileType = _T("Ŀ¼");
			}
			else {
				strFileType = _T("�ļ�");
				ULONGLONG ullBytes = finder.GetLength();
				if (ullBytes < PER_KB_BYTES) {
					strFileSize.Format(_T("%d Bytes"), ullBytes);
				}
				else {
					ULONGLONG ullKBytes = ullBytes / PER_KB_BYTES;
					strFileSize.Format(_T("%lld"), ullKBytes);
					int nLen = strFileSize.GetLength();
					int nIdx = 0;

					if (nLen > 3) {  //���ȴ���3��Ҫ���붺�ŷָ���
						int nRet = nLen / 3;
						int nRemainder = nLen % 3;
						int nCommaCnt = 0;  //���ŷָ�����

						if (nRemainder == 0) { //����Ϊ0ʱ�������� = ��-1
							nCommaCnt = nRet - 1;
							nIdx += 3;
						}
						else {                 //������Ϊ0ʱ�������� = ��
							nCommaCnt = nRet;
							nIdx = nRemainder;
						}

						//���붺�ŷָ���
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
