#include "stdafx.h"
#include "StartItemTabWidget.h"

StartItemTabWidget::StartItemTabWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "Command" << "名称";
	ui.tableWgStartItemMgr->setColumnCount(1);
	ui.tableWgStartItemMgr->setHorizontalHeaderLabels(headers);
	ui.tableWgStartItemMgr->setColumnWidth(0, ui.tableWgStartItemMgr->width() / 2);
	//ui.tableWgStartItemMgr->setColumnWidth(1, ui.tableWgStartItemMgr->width() / 2);
	//ui.tableWgSoftMgr->setColumnWidth(2, ui.tableWgSoftMgr->width() / 6);
	//ui.tableWgSoftMgr->setColumnWidth(3, ui.tableWgSoftMgr->width() / 6 * 3);

	ui.tableWgStartItemMgr->setContextMenuPolicy(Qt::CustomContextMenu);
	m_nTimerId = startTimer(1000);
}

StartItemTabWidget::~StartItemTabWidget()
{
}

void StartItemTabWidget::timerEvent(QTimerEvent *tEvent)
{
	if (tEvent->timerId() == m_nTimerId) {
		killTimer(m_nTimerId);
		listStartItem();
	}
}

#define REG_BOOT_START_ITEM_HKCU         3
#define MAX_BUF                          1024
void StartItemTabWidget::listStartItem()
{
	HKEY rootKey = HKEY_LOCAL_MACHINE;
	TCHAR RegBootStartItemInHKCU[REG_BOOT_START_ITEM_HKCU][MAX_PATH] = {
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		TEXT("Software\\Microsoft\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run")
	};

	for (int i = 0; i < REG_BOOT_START_ITEM_HKCU; i++) {

		LPCTSTR lpSubKey = RegBootStartItemInHKCU[i];

		HKEY hkResult = 0;           //接收将要打开的键的句柄
		//1.打开一个键
		LONG lRet = RegOpenKeyEx(
			rootKey,                    //根键句柄
			lpSubKey,  //子键路径
			0,                          //保留，必须为0
			KEY_ENUMERATE_SUB_KEYS | \
			KEY_QUERY_VALUE |\
			KEY_WOW64_64KEY,            //指定打开键句柄的权限
			&hkResult                   //子键句柄
			);

		if (lRet != ERROR_SUCCESS)
			continue;

		DWORD dwType = REG_SZ | REG_EXPAND_SZ;
		DWORD dwIndex = 0;

		while (true) {
			DWORD dwBufSize = MAX_BUF;
			DWORD dwKeySize = MAX_BUF;
			TCHAR szKeyBuf[MAX_PATH] = {};
			TCHAR szValBuf[MAX_PATH] = {};

			//枚举键名
			LONG lRet = RegEnumValue(
				hkResult,                 
				dwIndex,                 
				szKeyBuf,               
				&dwBufSize,            
				0,                    
				&dwType,             
				(LPBYTE)szValBuf,   
				&dwKeySize         
				);

			//查询键值
			int nRet = RegQueryValueEx(hkResult, szKeyBuf, 0, &dwType,
				(LPBYTE)szValBuf, &dwKeySize);

			if (nRet != ERROR_SUCCESS)
				break;

			QString strVal = QString::fromWCharArray(szValBuf);
			if (strVal.isEmpty()) {
				dwIndex++;
				continue;
			}

			int nRowCnt = ui.tableWgStartItemMgr->rowCount();
			ui.tableWgStartItemMgr->setRowCount(nRowCnt + 1);
			ui.tableWgStartItemMgr->setItem(nRowCnt, 0, new QTableWidgetItem(strVal));
			dwIndex++;
		}
	}
}
