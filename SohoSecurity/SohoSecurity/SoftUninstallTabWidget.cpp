#include "stdafx.h"
#include "SoftUninstallTabWidget.h"

SoftUninstallTabWidget::SoftUninstallTabWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	QStringList headers;
	headers << "软件名称" << "安装时间" << "软件厂商" << "软件卸载路径";
	ui.tableWgSoftMgr->setColumnCount(4);
	ui.tableWgSoftMgr->setHorizontalHeaderLabels(headers);
	ui.tableWgSoftMgr->setColumnWidth(0, ui.tableWgSoftMgr->width() / 6);
	ui.tableWgSoftMgr->setColumnWidth(1, ui.tableWgSoftMgr->width() / 6);
	ui.tableWgSoftMgr->setColumnWidth(2, ui.tableWgSoftMgr->width() / 6);
	ui.tableWgSoftMgr->setColumnWidth(3, ui.tableWgSoftMgr->width() / 6 * 3);

	ui.tableWgSoftMgr->setContextMenuPolicy(Qt::CustomContextMenu);
	m_nTimerId = startTimer(1000);
}

SoftUninstallTabWidget::~SoftUninstallTabWidget()
{

}

void SoftUninstallTabWidget::timerEvent(QTimerEvent *tEvent)
{
	if (tEvent->timerId() == m_nTimerId) {
		killTimer(m_nTimerId);
		listSoftWare();
	}
}

void SoftUninstallTabWidget::onTableWidgetCustomContextMenuRequest(QPoint pos)
{
	QTableWidgetItem *pCurItem = ui.tableWgSoftMgr->itemAt(pos);
	if (pCurItem == nullptr)
		return;

	QMenu *pPopMenu = new QMenu(ui.tableWgSoftMgr);
	QAction *pActUninstall = pPopMenu->addAction("卸载");

	connect(pActUninstall, &QAction::triggered, this, &SoftUninstallTabWidget::onUninstallSoft);
	pPopMenu->exec(QCursor::pos());

	delete pActUninstall; pActUninstall = nullptr;
	delete pPopMenu; pPopMenu = nullptr;
}

void SoftUninstallTabWidget::onUninstallSoft()
{
	QTableWidgetItem *pUnsPathItem = ui.tableWgSoftMgr->selectedItems().at(3);
	QString strUnsPath = pUnsPathItem->text();
	TCHAR szUnsPath[MAX_PATH] = {};
	strUnsPath.toWCharArray(szUnsPath);
	ShellExecute(NULL, TEXT("open"), szUnsPath, NULL, NULL, SW_NORMAL);
}
				
#define MAX_BUF     1024

void SoftUninstallTabWidget::listSoftWare()
{
	HKEY rootKey = HKEY_LOCAL_MACHINE;
	LPCTSTR lpSubKey =
		TEXT("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	HKEY hkResult = 0;           //接收将要打开的键的句柄

	//1.打开一个已经存在的注册表键
	LONG lRet = RegOpenKeyEx(
		rootKey,               //根键句柄
		lpSubKey,              //子键路径
		0,                     //保留，必须为0
		KEY_ENUMERATE_SUB_KEYS | \
		KEY_QUERY_VALUE,       //指定打开键句柄的权限
		&hkResult              //子键句柄
		);

	//2.循环遍历 Uninstall 目录下的子键
	DWORD dwIndex = 0;
	while (true) {
		DWORD dwKeyLen = MAX_BUF;
		TCHAR szNewKeyName[MAX_PATH] = {};           //注册表项名称

		ZeroMemory(&m_softInfo, sizeof(SOFTINFO));

		LONG lRet = RegEnumKeyEx(
			hkResult,                 //子键句柄
			dwIndex,                  //指定被枚举键下子键的索引，从0开始
			szNewKeyName,             //指向一块内存，保存子键的名称
			&dwKeyLen,                //指向szKeyName内存的大小
			0,                        //保留，必须为0
			NULL,                     //用于获取子键值的类型
			NULL,                     //指向一块内存，用于获取子键值数据
			NULL                      //上一个参数所指向的内存大小
			);

		if (lRet == ERROR_NO_MORE_ITEMS)
			break;

		//2.1 通过得到的子键名称重新组合成新的子键路径
		TCHAR szMidReg[MAX_PATH] = {};
		swprintf_s(szMidReg, TEXT("%s%s%s"), lpSubKey, TEXT("\\"), szNewKeyName);

		//2.2 打开新的子键, 获取其句柄
		HKEY hkValueKey = 0;           //子键句柄
		RegOpenKeyEx(rootKey, szMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);

		//2.3 获取键值
		DWORD dwNameLen = MAX_BUF;         //指向szBuffer内存的大小
		//获取软件名称
		DWORD dwType = 0;
		RegQueryValueEx(hkValueKey, TEXT("DisplayName"), 0, &dwType,
			(LPBYTE)m_softInfo.szSoftName, &dwNameLen);

		QString strSoftName = QString::fromWCharArray(m_softInfo.szSoftName);
		if (strSoftName.isEmpty()) {
			dwIndex++;
			continue;
		}

		int nRowCnt = ui.tableWgSoftMgr->rowCount();
		ui.tableWgSoftMgr->setRowCount(nRowCnt + 1);

		ui.tableWgSoftMgr->setItem(nRowCnt, 0, new QTableWidgetItem(strSoftName));

		dwNameLen = MAX_BUF;     //如果没有重新赋值，下一次将获取不到信息
		//获取软件安装时间
		RegQueryValueEx(hkValueKey, TEXT("InstallDate"), 0, &dwType,
			(LPBYTE)m_softInfo.szSoftDate, &dwNameLen);

		QString strDate = QString::fromWCharArray(m_softInfo.szSoftDate);
		if (!strDate.isEmpty()) {
			strDate.insert(4, tr("-"));
			strDate.insert(7, tr("-"));
		}
		QTableWidgetItem *pSoftDateItem = new QTableWidgetItem(strDate);
		pSoftDateItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWgSoftMgr->setItem(nRowCnt, 1, pSoftDateItem);

		dwNameLen = MAX_BUF;     
		//获取软件发布厂商
		RegQueryValueEx(hkValueKey, TEXT("Publisher"), 0, &dwType,
			(LPBYTE)m_softInfo.szSoftVenRel, &dwNameLen);
		ui.tableWgSoftMgr->setItem(nRowCnt, 2, new QTableWidgetItem(QString::fromWCharArray(m_softInfo.szSoftVenRel)));

		dwNameLen = MAX_BUF;     
		//获取卸载路径
		RegQueryValueEx(hkValueKey, TEXT("UninstallString"), 0, &dwType,
			(LPBYTE)m_softInfo.szSoftUnsPath, &dwNameLen);
		ui.tableWgSoftMgr->setItem(nRowCnt, 3, new QTableWidgetItem(QString::fromWCharArray(m_softInfo.szSoftUnsPath)));

		dwIndex++;
	}
}
