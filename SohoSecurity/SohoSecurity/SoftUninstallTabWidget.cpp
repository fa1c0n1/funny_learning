#include "stdafx.h"
#include "SoftUninstallTabWidget.h"

SoftUninstallTabWidget::SoftUninstallTabWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	QStringList headers;
	headers << "�������" << "��װʱ��" << "�������" << "���ж��·��";
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
	QAction *pActUninstall = pPopMenu->addAction("ж��");

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
	HKEY hkResult = 0;           //���ս�Ҫ�򿪵ļ��ľ��

	//1.��һ���Ѿ����ڵ�ע����
	LONG lRet = RegOpenKeyEx(
		rootKey,               //�������
		lpSubKey,              //�Ӽ�·��
		0,                     //����������Ϊ0
		KEY_ENUMERATE_SUB_KEYS | \
		KEY_QUERY_VALUE,       //ָ���򿪼������Ȩ��
		&hkResult              //�Ӽ����
		);

	//2.ѭ������ Uninstall Ŀ¼�µ��Ӽ�
	DWORD dwIndex = 0;
	while (true) {
		DWORD dwKeyLen = MAX_BUF;
		TCHAR szNewKeyName[MAX_PATH] = {};           //ע���������

		ZeroMemory(&m_softInfo, sizeof(SOFTINFO));

		LONG lRet = RegEnumKeyEx(
			hkResult,                 //�Ӽ����
			dwIndex,                  //ָ����ö�ټ����Ӽ�����������0��ʼ
			szNewKeyName,             //ָ��һ���ڴ棬�����Ӽ�������
			&dwKeyLen,                //ָ��szKeyName�ڴ�Ĵ�С
			0,                        //����������Ϊ0
			NULL,                     //���ڻ�ȡ�Ӽ�ֵ������
			NULL,                     //ָ��һ���ڴ棬���ڻ�ȡ�Ӽ�ֵ����
			NULL                      //��һ��������ָ����ڴ��С
			);

		if (lRet == ERROR_NO_MORE_ITEMS)
			break;

		//2.1 ͨ���õ����Ӽ�����������ϳ��µ��Ӽ�·��
		TCHAR szMidReg[MAX_PATH] = {};
		swprintf_s(szMidReg, TEXT("%s%s%s"), lpSubKey, TEXT("\\"), szNewKeyName);

		//2.2 ���µ��Ӽ�, ��ȡ����
		HKEY hkValueKey = 0;           //�Ӽ����
		RegOpenKeyEx(rootKey, szMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);

		//2.3 ��ȡ��ֵ
		DWORD dwNameLen = MAX_BUF;         //ָ��szBuffer�ڴ�Ĵ�С
		//��ȡ�������
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

		dwNameLen = MAX_BUF;     //���û�����¸�ֵ����һ�ν���ȡ������Ϣ
		//��ȡ�����װʱ��
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
		//��ȡ�����������
		RegQueryValueEx(hkValueKey, TEXT("Publisher"), 0, &dwType,
			(LPBYTE)m_softInfo.szSoftVenRel, &dwNameLen);
		ui.tableWgSoftMgr->setItem(nRowCnt, 2, new QTableWidgetItem(QString::fromWCharArray(m_softInfo.szSoftVenRel)));

		dwNameLen = MAX_BUF;     
		//��ȡж��·��
		RegQueryValueEx(hkValueKey, TEXT("UninstallString"), 0, &dwType,
			(LPBYTE)m_softInfo.szSoftUnsPath, &dwNameLen);
		ui.tableWgSoftMgr->setItem(nRowCnt, 3, new QTableWidgetItem(QString::fromWCharArray(m_softInfo.szSoftUnsPath)));

		dwIndex++;
	}
}
