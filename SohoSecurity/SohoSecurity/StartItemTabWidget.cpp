#include "stdafx.h"
#include "StartItemTabWidget.h"

StartItemTabWidget::StartItemTabWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QStringList headers;
	headers << "Command" << "����";
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

		HKEY hkResult = 0;           //���ս�Ҫ�򿪵ļ��ľ��
		//1.��һ����
		LONG lRet = RegOpenKeyEx(
			rootKey,                    //�������
			lpSubKey,  //�Ӽ�·��
			0,                          //����������Ϊ0
			KEY_ENUMERATE_SUB_KEYS | \
			KEY_QUERY_VALUE |\
			KEY_WOW64_64KEY,            //ָ���򿪼������Ȩ��
			&hkResult                   //�Ӽ����
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

			//ö�ټ���
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

			//��ѯ��ֵ
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
