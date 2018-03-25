// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TaskManager.h"
#include "MyListCtrl.h"

// CMyListCtrl: 对 CListCtrl 的增强(用了可变参数)

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
END_MESSAGE_MAP()



// CMyListCtrl message handlers

//Param1: 要插入多少列
void CMyListCtrl::AddColumns(int nNum, ...)
{
	va_list vArgList;

	va_start(vArgList, nNum);

	for (int i = 0; i < nNum; i++) {
		TCHAR *pColName = va_arg(vArgList, TCHAR *); //列名
		int nWidth = va_arg(vArgList, int);          //列宽

		InsertColumn(i, pColName, LVCFMT_CENTER, nWidth, i);
	}

	va_end(vArgList);
}

/*
* Param1: 第几行
* Param2: 要在该行填写多少个数据
*/
void CMyListCtrl::AddItems(int nItemNum, int nCount, ...)
{
	va_list vArgList;

	va_start(vArgList, nCount);

	TCHAR *pText = va_arg(vArgList, TCHAR *);
	InsertItem(nItemNum, pText);
	for (int i = 1; i < nCount; i++) {
		pText = va_arg(vArgList, TCHAR *);
		SetItemText(nItemNum, i, pText);
	}

	va_end(vArgList);
}


