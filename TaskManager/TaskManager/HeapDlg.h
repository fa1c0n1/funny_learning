#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CHeapDlg dialog

typedef struct _HEAPPROCINFO {
	HWND hwnd;    //µ±Ç°´°¿Ú¾ä±ú
	DWORD dwPid;
	CMyListCtrl *pListCtrl;
} HEAPPROCINFO, *PHEAPPROCINFO;

class CHeapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHeapDlg)

public:
	CHeapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHeapDlg();

// Dialog Data
	enum { IDD = IDD_HEAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_listCtrlHeap;
	DWORD m_dwPid;
	CString m_processName;
	int m_nHeapNum;
	bool m_bForbitClose;

private:
	void InitControl();
	static void ListProcessHeap(HWND hWnd, DWORD dwPid, CMyListCtrl &listCtrl);
	static UINT __cdecl TraverseHeapProc(LPVOID pParam);

public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnUserTraverseHeapFinish(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
